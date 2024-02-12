#pragma once
// #include "ast.hpp"
#include <cstring>
#include <string>
#include <vector>

class Value;
class Type;
class Symbol;
class Scope;

extern Type ty_void;
extern Type ty_bool;

extern Type ty_char;
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

class Value
{
public:
  Value()
    : u32(0xfefefefe)
    , ty(&ty_uint)
  {}

  Value(int i)
    : i32(i)
    , ty(&ty_int)
  {}

  Value(unsigned int u)
    : u32(u)
    , ty(&ty_uint)
  {}

  Value(float f)
    : f32(f)
    , ty(&ty_float)
  {}

  Value(double d)
    : f64(d)
    , ty(&ty_double)
  {}

  Value(const char* str);

  Type* getType() const
  {
    return ty;
  }

  int getInt() const
  {
    return i32;
  }

  Value index(Value idx) const;

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
  Type()
    : name(nullptr)
    , kind(TY_VOID)
    , size(0)
    , align(0)
    , is_signed(true)
  {}

  Type(TypeKind kind, int size, int align, bool is_signed = false, bool is_atomic = false)
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

  bool is_function() const
  {
    return kind == TY_FUNC;
  }

  bool require_define() const
  {
    return kind == TY_STRUCT || kind == TY_UNION || kind == TY_ENUM;
  }

  bool is_compatible(Type* t1, Type* t2);

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
    ty->array_size = len;
    return ty;
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

  void add_member(Symbol* m)
  {
    _structure.push_back(m);
  }

public:
  // Declaration
  const char* name;
  TypeKind kind;

  int size;
  int align;
  bool is_const;
  bool is_volatile;
  bool is_restrict;
  bool is_signed;
  bool is_atomic;
  Type* origin;


  // Pointer or array
  Type* base;
  int array_size;

  // Variable-length array

  // structure type
  std::vector<Symbol*> _structure;
  bool is_flexible;
  bool is_packed;

  // function type
  Type* return_ty;
  Type* params;
  bool is_variadic;
  Type* next;
};

class StructureType : public Type
{
public:
  StructureType()
    : Type(TY_STRUCT, 0, 1)
  {}

  void add_member(Symbol* m)
  {
    members.push_back(m);
  }

  std::vector<Symbol*> members;
};

class Symbol
{
public:
  enum SymbolType
  {
    VARIABLE,
    FUNCTION,
    TYPEDEF
  };

  Symbol(SymbolType type, const char* name)
    : type(type)
    , name(name)
  {}

  const char* getName() const
  {
    return name;
  }

  Type* ty;
  Value val;

private:
  SymbolType type;
  const char* name;
};

class Scope
{
public:
  Scope(Scope* parent)
    : parent(parent)
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

private:
  Scope* parent;
  std::vector<Symbol*> symbols;
};
