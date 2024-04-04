#pragma once
#include "c2.hpp"
#include <cstdio>
#include <cstring>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

class Imm;
class Type;
class Symbol;
class Value;
class Scope;

extern Type ty_void;
extern Type ty_bool;

extern Type ty_char;
extern Type ty_str;
extern Type ty_short;
extern Type ty_int;
extern Type ty_long;

extern Type ty_uchar;
extern Type ty_ushort;
extern Type ty_uint;
extern Type ty_ulong;

extern Type ty_float;
extern Type ty_double;
extern Type ty_ldouble;

class Imm
{
public:
  Imm(int i)
    : i32(i)
    , ty(&ty_int)
  {}

  Imm(unsigned int u)
    : u32(u)
    , ty(&ty_uint)
  {}

  Imm(float f)
    : f32(f)
    , ty(&ty_float)
  {}

  Imm(double d)
    : f64(d)
    , ty(&ty_double)
  {}

  Imm(const char* label, Type* ty)
    : ptr(strdup(label))
    , ty(ty)
  {}

  Imm(Type* ty, void* ptr)
    : ptr(ptr)
    , ty(ty)
  {}

  Type* getType() const
  {
    return ty;
  }

  int getInt() const
  {
    return i32;
  }

  float getFloat() const
  {
    return f32;
  }

  Imm* index(Imm* idx) const;

  Imm* add(Imm* imm) const;

private:
  union
  {
    int i32;
    unsigned int u32;
    float f32;
    double f64;
    void* ptr;
  };
  Type* ty;
};

class Type
{
public:
  enum TypeKind
  {
    TY_VOID,
    TY_BOOL,
    TY_CHAR,
    TY_SHORT,
    TY_INT,
    TY_LONG,
    TY_FLOAT,
    TY_DOUBLE,
    TY_LDOUBLE,
    TY_PTR,
    TY_ARRAY,
    TY_FUNC,
    TY_VLA,   // variable-length array
    TY_STRUCT,
    TY_UNION,
    TY_ENUM,
  };

public:
  Type(TypeKind kind, int size = 0, int align = 0, bool is_signed = false, bool is_atomic = false)
    : kind(kind)
    , size(size)
    , align(align)
    , is_signed(is_signed)
    , is_atomic(is_atomic)
  {}

  Type(const Type& t)
  {
    *this = t;
    origin = (Type*)&t;
  }

  static Type* enum_type()
  {
    Type* ty = new Type(TY_ENUM, 4, 4);
    return ty;
  }

  static Type* struct_type()
  {
    Type* ty = new Type(TY_STRUCT, 0, 1);
    return ty;
  }

  bool is_integer() const
  {
    return kind == TY_BOOL || kind == TY_CHAR || kind == TY_SHORT || kind == TY_INT ||
           kind == TY_LONG || kind == TY_ENUM;
  }

  bool is_flonum() const
  {
    return kind == TY_FLOAT || kind == TY_DOUBLE || kind == TY_LDOUBLE;
  }

  bool is_numeric() const
  {
    return is_integer() || is_flonum();
  }

  bool is_pointer() const
  {
    return kind == TY_PTR;
  }

  bool is_array() const
  {
    return kind == TY_ARRAY;
  }

  bool is_scalar() const
  {
    return is_numeric() || is_pointer();
  }

  bool is_aggregate() const
  {
    return kind == TY_STRUCT || kind == TY_UNION || kind == TY_ENUM;
  }

  bool is_function() const
  {
    return kind == TY_FUNC;
  }

  bool is_indexable() const
  {
    return is_pointer() || is_array();
  }

  static bool is_compatible(Type* t1, Type* t2);

  Type* pointer_to() const
  {
    Type* ty = new Type(TY_PTR, 8, 8);
    ty->base = (Type*)this;
    ty->is_signed = false;
    return ty;
  }

  Type* func_type() const
  {
    Type* ty = new Type(TY_FUNC, 1, 1);
    ty->return_ty = (Type*)this;
    return ty;
  }

  Type* array_of(int len) const
  {
    Type* ty = new Type(TY_ARRAY, size * len, align);
    ty->base = (Type*)this;
    ty->array_len = len;
    return ty;
  }

  void structure_add_member(Symbol* m)
  {
    members.push_back(m);
  }

  TypeKind getKind() const
  {
    return kind;
  }

  int getSize() const
  {
    return size;
  }

  Type* getBaseTy() const
  {
    return base;
  }

  const char* getName() const
  {
    return name;
  }

  void setName(const char* n)
  {
    name = n;
  }

private:
  // Declaration
  TypeKind kind;
  const char* name;

  int size;
  int align;

public:
  bool is_const;
  bool is_volatile;
  bool is_restrict;
  bool is_signed;
  bool is_atomic;
  bool is_inline;
  bool is_noreturn;
  bool is_static;
  bool is_extern;
  bool is_register;
  bool is_typedef;
  bool is_auto;
  bool is_thread_local;

private:
  Type* origin;

  // Pointer or array
  Type* base;
  int array_len;

  // structure type
  std::vector<Symbol*> members;
  bool is_packed;

  // function type
  Type* return_ty;

public:
  std::vector<Symbol*> params;
  bool hasEllipsis;
};

enum SymbolKind
{
  SY_TYPEDEF,
  SY_ABSTRACT_VARIABLE,   // used when we only need the type, e.g. function prototype
  SY_VARIABLE,
  SY_LABEL,
  SY_CASE_LABEL,
  SY_DEFAULT_LABEL,
  SY_CONTINUE,
  SY_BREAK,
  SY_ENUMERATION_CONSTANT,
  SY_FUNCNAME,
  SY_NONKIND,
};

class Symbol
{
public:
  Symbol(SymbolKind kind, const char* name)
    : kind(kind)
    , name(name)
  {}

  const char* getName() const
  {
    return name;
  }

  Type* getType() const
  {
    return typ;
  }

  void setType(Type* t)
  {
    typ = t;
  }

  std::shared_ptr<Value> getValue() const
  {
    return val;
  }

  void setValue(std::shared_ptr<Value> v)
  {
    val = v;
  }

  bool isVariable() const
  {
    return kind == SY_VARIABLE;
  }

  /*
    int x;  <- declaration
    int x = 1;  <- definition
  */
  bool with_value() const
  {
    return kind == SY_VARIABLE && val != nullptr;
  }

public:
  SymbolKind kind;

public:
  const char* name;   // name of the symbol
  void* node;         // AST node of the symbol

  // for VARIABLE
  Type* typ;   // type of the symbol
  int place;   // stack offset

  // for LABEL
  std::string label;   // label name for the symbol

  // for VARIABLE, ENUM, FUNCTION
  std::shared_ptr<Value> val;   // value of the symbol
};

class Register
{
public:
  // "%rax", etc.
  virtual const char* getName() const = 0;
  virtual int getSize() const = 0;
};

/**
 * Value class represents a value in the program.
 * It can be a variable, a constant, or a temporary value.
 */
class Value
{
public:
  enum class Storage
  {
    REGISTER,
    MEMORY,
    IMMEDIATE,
  };

  Value(Symbol* sym)
    : rvalue(false)
    , storage(Storage::MEMORY)
    , sym(sym)
  {}

  Value(std::shared_ptr<Register> reg)
    : rvalue(true)
    , storage(Storage::REGISTER)
    , reg(reg)
  {}

  Value(Imm* imm)
    : rvalue(true)
    , storage(Storage::IMMEDIATE)
    , imm(imm)
  {}

  bool isLValue() const
  {
    return !rvalue;
  }

  bool isRValue() const
  {
    return rvalue;
  }

  bool isInRegister() const
  {
    return storage == Storage::REGISTER;
  }

  std::shared_ptr<Register> getRegister() const
  {
    return reg;
  }

  bool isInMemory() const
  {
    return storage == Storage::MEMORY;
  }

  bool isImmediate() const
  {
    return storage == Storage::IMMEDIATE;
  }

  Imm* getImm() const
  {
    return imm;
  }

  const char* getAsmName() const
  {
    static char buf[256];

    if (isInMemory()) {
      sprintf(buf, "-%x(%%rbp)", sym->place);
    } else if (isInRegister()) {
      sprintf(buf, "%s", reg->getName());
    } else {   // isImmediate
      sprintf(buf, "$%d", imm->getInt());
    }

    return buf;
  }

public:
  // left value: variable, in register or in memory
  // right value: temporary value, in register or just a constant
  bool rvalue;

  Storage storage;

  Symbol* sym;   // the variable symbol if it's an lvalue

  // for Memory
  std::shared_ptr<Value> base;

  // for Register
  std::shared_ptr<Register> reg;

  // for IMMEDIATE
  Imm* imm;
};

enum ScopeKind
{
  SC_GLOBAL,
  SC_LOCAL,
  SC_FUNCTION,
  SC_SWITCH,
  SC_LOOP,
};

/**
 * Scope class represents a scope in the program.
 * It can be a global scope, a local scope, a function scope, a switch scope, or a loop scope.
 * It contains a list of symbols declared in the scope.
 *
 * foo () {
 *  int x;  <- local scope
 *  for (...) {  <- loop scope
 *
 * }
 */
class Scope
{
  friend class ScopeManager;

public:
  Scope(Scope* parent, ScopeKind kind)
    : kind(kind)
    , parent(parent)
  {}

  Scope(Scope* parent)
    : Scope(parent, SC_LOCAL)
  {}

  void setName(const char* name)
  {
    this->name = name;
  }

  const std::string& getName() const
  {
    return this->name;
  }

  void add(Symbol* sym)
  {
    symbols.push_back(sym);
  }

  Symbol* find(const char* name, SymbolKind kind)
  {
    for (auto sym = symbols.rbegin(); sym != symbols.rend(); ++sym) {
      if ((!name || strcmp((*sym)->getName(), name) == 0) &&
          (kind == SY_NONKIND || (*sym)->kind == kind))
        return *sym;
    }
    return parent ? parent->find(name, kind) : nullptr;
  }

  Scope* getParent() const
  {
    return parent;
  }

  std::vector<Symbol*>& getSymbols()
  {
    return symbols;
  }

private:
  ScopeKind kind;
  Scope* parent;
  std::string name;
  std::vector<Symbol*> symbols;

public:
  int stack_size = 0;
};

class ScopeManager
{
public:
  ScopeManager()
    : root(new Scope(nullptr))
    , current(root)
  {}

  void enter(ScopeKind kind)
  {
    current = new Scope(current, kind);
  }

  void leave()
  {
    current = current->getParent();
  }

  void add(Symbol* sym)
  {
    current->add(sym);
  }

  Symbol* findSymbol(const char* name, SymbolKind kind) const
  {
    return current->find(name, kind);
  }

  Scope* findScope(ScopeKind kind) const
  {
    for (Scope* s = current; s; s = s->getParent()) {
      if (s->kind == kind)
        return s;
    }
    return nullptr;
  }

  Scope* findBreakableScope() const
  {
    for (Scope* s = current; s; s = s->getParent()) {
      if (s->kind == SC_SWITCH || s->kind == SC_LOOP)
        return s;
    }
    return nullptr;
  }

  Scope* getRoot() const
  {
    return root;
  }

  Scope* getCurrent() const
  {
    return current;
  }

private:
  Scope* root;
  Scope* current;
};
