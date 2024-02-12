#pragma once
#include "../type.hpp"

class Register
{
public:
  virtual const char* getName() const = 0;
};

class Destination
{
public:
  enum class Kind
  {
    REGISTER,
    MEMORY,
    IMMEDIATE
  };

  Destination()
    : kind(Kind::IMMEDIATE)
    , imm(0)
  {}

  Destination(Register* reg)
    : kind(Kind::REGISTER)
    , reg(reg)
  {}

  Destination(Symbol* sym)
    : kind(Kind::MEMORY)
    , sym(sym)
  {}

  Destination(Value imm)
    : kind(Kind::IMMEDIATE)
    , imm(imm)
  {}

  Kind kind;
  Register* reg;
  Symbol* sym;
  Value imm = 0;
  int offset;
};
