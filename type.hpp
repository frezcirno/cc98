#pragma once
#include "c2.hpp"
#include "backend.hpp"
#include <cstdio>
#include <cstring>
#include <iterator>
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
};

enum SymbolKind
{
  SK_TYPEDEF,
  SK_VARIABLE,
  SK_FUNCTION,
  SK_LABEL,
  SK_CASE_LABEL,
  SK_DEFAULT_LABEL,
  SK_ENUMERATION_CONSTANT,
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

  Imm* getValue() const
  {
    return val;
  }

  void setValue(Imm* v)
  {
    val = v;
  }

  bool isVariable() const
  {
    return kind == SK_VARIABLE;
  }

  /*
    int x;  <- declaration
    int x = 1;  <- definition
  */
  bool with_value() const
  {
    return kind == SK_VARIABLE && val != nullptr;
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
  Imm* val;   // value of the symbol
};

/**
 * Value class represents a value in the program.
 * It can be a variable, a constant, or a temporary value.
 */
class Value
{
public:
  enum class Kind
  {
    LVALUE,   // variable, in register or in memory
    RVALUE,   // temporary value, in register or just a constant
  };

  enum class Storage
  {
    REGISTER,
    MEMORY,
    IMMEDIATE,
  };

  Value(Symbol* sym)
    : kind(Kind::LVALUE)
    , storage(Storage::MEMORY)
    , sym(sym)
  {}

  Value(Register* reg)
    : kind(Kind::RVALUE)
    , storage(Storage::REGISTER)
    , reg(reg)
  {}

  Value(Imm* imm)
    : kind(Kind::RVALUE)
    , storage(Storage::IMMEDIATE)
    , imm(imm)
  {}

  bool isLValue() const
  {
    return kind == Kind::LVALUE;
  }

  bool isRValue() const
  {
    return kind == Kind::RVALUE;
  }

  bool isInRegister() const
  {
    return storage == Storage::REGISTER;
  }

  Register* getRegister() const
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

  Imm* getImmediate() const
  {
    return imm;
  }

  const char* getAsmName() const
  {
    static char buf[256];

    if (isInMemory()) {
      sprintf(buf, "-%x(%%rbp)", sym->place);
    } else if (isInRegister()) {
      sprintf(buf, "%%%s", reg->getName());
    } else {   // isImmediate
      sprintf(buf, "$%d", imm->getInt());
    }

    return buf;
  }

public:
  Kind kind;

  Storage storage;

  Symbol* sym;   // the variable symbol if it's an lvalue

  // for Memory
  Value* base;

  // for Register
  Register* reg;

  // for IMMEDIATE
  Imm* imm;
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
  enum ScopeKind
  {
    GLOBAL,
    LOCAL,
    FUNCTION,
    SWITCH,
    LOOP,
  };

  Scope(Scope* parent, ScopeKind kind)
    : kind(kind)
    , parent(parent)
  {}

  Scope(Scope* parent)
    : Scope(parent, LOCAL)
  {}

  void add(Symbol* sym)
  {
    symbols.push_back(sym);
  }

  Symbol* find(const char* name)
  {
    for (auto sym : symbols) {
      if (strcmp(sym->getName(), name) == 0)
        return sym;
    }
    return parent ? parent->find(name) : nullptr;
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
  std::vector<Symbol*> symbols;

public:
  int stack_size = 0;
};

class ScopeManager
{
public:
  ScopeManager()
    : global(new Scope(nullptr))
    , current(global)
  {}

  void enter(Scope::ScopeKind kind)
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

  Symbol* find(const char* name) const
  {
    return current->find(name);
  }

  Symbol* find(const char* name, SymbolKind kind) const
  {
    for (Scope* s = current; s; s = s->getParent()) {
      for (auto sym : s->getSymbols()) {
        if (strcmp(sym->getName(), name) == 0 && sym->kind == kind)
          return sym;
      }
    }
    return nullptr;
  }

  Scope* find(Scope::ScopeKind kind)
  {
    for (Scope* s = current; s; s = s->getParent()) {
      if (s->kind == kind)
        return s;
    }
    return nullptr;
  }

  Scope* getGlobal() const
  {
    return global;
  }

  Scope* getCurrent() const
  {
    return current;
  }

  Scope* getFunctionScope()
  {
    return find(Scope::LOCAL);
  }

private:
  Scope* global;
  Scope* current;
};
