#pragma once
#include <stdexcept>

enum class NodeKind
{
  // EXPRESSION,
  E_COMMA_EXPRESSION,
  E_ASSIGNMENT_EXPRESSION,
  E_CONDITIONAL_EXPRESSION,
  // E_POSTFIX_EXPRESSION,
  PFE_INC,
  PFE_DEC,
  PFE_CALL,
  PFE_FIELD,
  PFE_PTR_FIELD,
  PFE_INDEX,
  PFE_INIT,
  // E_PRIMARY_EXPRESSION,
  P_IDENTIFIER,
  P_CONSTANT_I,
  P_CONSTANT_F,
  P_ENUMERATION_CONSTANT,
  P_STRING_LITERAL,
  P_FUNC_NAME,
  P_PARENTHESIZED_EXPRESSION,
  P_GENERIC_SELECTION,
  // E_UNARY_EXPRESSION,
  U_INC_OP,
  U_DEC_OP,
  U_ADDR_OP,
  U_DEREF_OP,
  U_POS_OP,
  U_NEG_OP,
  U_REV_OP,
  U_NOT_OP,
  U_SIZEOF_OP,
  U_ALIGNOF_OP,

  E_CAST_EXPRESSION,
  E_ARITHMETIC_EXPRESSION,

  N_FUNCTION_DEFINITION,

  N_DECLARATOR,
  N_ABSTRACT_DECLARATOR,

  // D_DIRECT_DECLRATOR
  D_IDENTIFIER,
  D_PARENTHESIZED_DECLARATOR,
  D_ARRAY_DECLARATOR,
  D_FUNCTION_DECLARATOR,

  // D_DIRECT_ABSTRACT_DECLARATOR
  D_PARENTHESIZED_ABSTRACT_DECLARATOR,
  D_ARRAY_ABSTRACT_DECLARATOR,
  D_FUNCTION_ABSTRACT_DECLARATOR,

  N_PARAMETER_LIST,
  N_DECLARATION_NORMAL,
  N_DECLARATION_STATIC,
  PAR_DECLARATOR,
  PAR_ABSTRACT_DECLARATOR,
  PAR_ANONYMOUS,

  N_INIT_DECLARATOR,
  N_STRUCT_DECLARATOR,
  N_STATIC_ASSERT_DECLARATION,
  N_STRUCT_DECLARATION_NORMAL,
  N_STRUCT_DECLARATION_STATIC,
  N_GENERIC_SELECTION,
  // N_DESIGNATOR,
  D_FIELD_DESIGNATOR,
  D_INDEX_DESIGNATOR,
  N_DESIGNATION,
  // N_INITIALIZER,
  I_EXPRESSION,
  I_NESTED_INITIALIZER,
  // N_STATEMENT,
  // // S_LABELED_STATEMENT,
  L_IDENTIFIER_STATEMENT,
  L_CASE_STATEMENT,
  L_DEFAULT_STATEMENT,
  S_COMPOUND_STATEMENT,
  S_EXPRESSION_STATEMENT,
  // // S_SELECTION_STATEMENT,
  S_IF_STATEMENT,
  S_SWITCH_STATEMENT,

  // // S_ITERATION_STATEMENT,
  S_WHILE_STATEMENT,
  S_DO_WHILE_STATEMENT,
  S_FOR_STATEMENT,
  // // S_JUMP_STATEMENT,
  S_GOTO_STATEMENT,
  S_CONTINUE_STATEMENT,
  S_BREAK_STATEMENT,
  S_RETURN_STATEMENT,
  N_POINTER,
  // N_TYPE_SPECIFIER,
  T_BUILTIN,
  T_ATOMIC,
  T_STRUCT,
  T_UNION,
  T_ENUM,
  T_TYPEDEF_NAME,
  N_ALIGNMENT_SPECIFIER,
  N_DECLARATION_SPECIFIERS,
  N_ENUMERATOR,
  N_SPECIFIER_QUALIFIER_LIST,
  N_TYPE_NAME,
  N_TRANSLATION_UNIT,
  N_GENERIC_ASSOCIATION,
  N_STORAGE_CLASS_SPECIFIER,
  N_FUNCTION_SPECIFIER,
  N_TYPE_QUALIFIER,
  N_END,
};

class Visitor;

void print_backtrace();

template<typename To, typename From>
inline bool isa(const From* Val)
{
  // return To::classof(Val);
  return dynamic_cast<const To*>(Val) != nullptr;
}

template<typename To, typename From>
inline To* cast(From* Val)
{
  // if (!isa<To>(Val)) {
  //   print_backtrace();
  //   throw std::runtime_error("Invalid cast!");
  // }
  // return static_cast<To*>(Val);
  auto r = dynamic_cast<To*>(Val);
  if (!r) {
    print_backtrace();
    throw std::runtime_error("Invalid cast!");
  }
  return r;
}

template<typename To, typename From>
inline To* dyn_cast(From* Val)
{
  // return isa<To>(Val) ? static_cast<To*>(Val) : nullptr;
  return dynamic_cast<To*>(Val);
}

template<typename To>
inline bool isa(const To* Val)
{
  return true;
}

template<typename To>
inline To* cast(To& Val)
{
  return Val;
}

template<typename To>
inline To* dyn_cast(To& Val)
{
  return Val;
}
