#include "type.hpp"
#include <cassert>

Type ty_void = Type(Type::TY_VOID, 1, 1);
Type ty_bool = Type(Type::TY_BOOL, 1, 1);
Type ty_char = Type(Type::TY_CHAR, 1, 1);
Type ty_short = Type(Type::TY_SHORT, 2, 2);
Type ty_int = Type(Type::TY_INT, 8, 8);
Type ty_long = Type(Type::TY_LONG, 8, 8);
Type ty_uchar = Type(Type::TY_CHAR, 1, 1, true);
Type ty_ushort = Type(Type::TY_SHORT, 2, 2, true);
Type ty_uint = Type(Type::TY_INT, 8, 8, true);
Type ty_ulong = Type(Type::TY_LONG, 8, 8, true);
Type ty_float = Type(Type::TY_FLOAT, 4, 4);
Type ty_double = Type(Type::TY_DOUBLE, 8, 8);
Type ty_ldouble = Type(Type::TY_LDOUBLE, 16, 16);

Value::Value(const char* str)
{
  ptr = strdup(str);
  ty = ty_char.array_of(strlen(str) + 1);
}

Value Value::index(Value idx) const
{
  assert(ty->kind == Type::TY_PTR || ty->kind == Type::TY_ARRAY);
  Value v;
  v.ty = ty->base;
  v.ptr = (char*)ptr + idx.i32 * ty->base->size;
  return v;
}