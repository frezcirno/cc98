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

bool Type::is_compatible(Type* t1, Type* t2)
{
  if (t1->kind == t2->kind) {
    return true;
  }
  return false;
}

Imm* Imm::index(Imm* idx) const
{
  assert(ty->is_indexable());
  auto ty1 = ty->getBaseTy();
  auto ptr1 = (char*)ptr + idx->i32 * ty->getBaseTy()->getSize();
  Imm* v = new Imm(ty1, ptr1);
  return v;
}

Imm* Imm::add(Imm* imm) const
{
  return new Imm(i32 + imm->i32);
}