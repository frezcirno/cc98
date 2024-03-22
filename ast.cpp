#include "ast.hpp"

const char* Declarator::name() const
{
  return decl->name();
}