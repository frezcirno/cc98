#pragma once
#include "c2.hpp"
#include "backend/backend.hpp"
#include "type.hpp"
#include <cassert>
#include <iostream>
#include <vector>
#include <optional>

enum NodeKind
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

class Node;
class Expression;
class PrimaryExpression;
class GenericAssociation;
class GenericSelection;
class StorageClassSpecifier;
class FunctionSpecifier;
class TypeQualifier;
class Identifier;
class ConstantI;
class ConstantF;
class EnumerationConstantUse;
class StringLiteral;
class FuncName;
class ParenthesizedExpression;
class UnaryExpression;
class UnaryExpressionExpr;
class UnaryExpressionOnType;
class CommaExpression;
class AssignmentExpression;
class ConditionalExpression;
class PostfixExpression;
class PostfixExpressionIncOp;
class PostfixExpressionDecOp;
class PostfixExpressionIndexOp;
class PostfixExpressionCallOp;
class PostfixExpressionDotOp;
class PostfixExpressionPtrOp;
class PostfixExpressionCastOp;
class ArithmeticExpression;
class CastExpression;
class Designator;
class DesignatorField;
class DesignatorIndex;
class Designation;
class Initializer;
class InitializerExpression;
class InitializerNested;
class StaticAssertDeclaration;
class Enumerator;
class TypeSpecifier;
class TypeSpecifierBuiltin;
class Specifiers;
class TypeSpecifierAtomic;
class TypeSpecifierTypedefName;
class StructDeclarator;
class StructDeclaration;
class StructDeclarationNormal;
class StructDeclarationStaticAssert;
class TypeSpecifierStructOrUnion;
class AlignmentSpecifier;
class DeclarationSpecifiers;
class Pointer;
class TypeSpecifierEnum;
class Statement;
class LabeledStatement;
class LabeledStatementIdentifier;
class LabeledStatementCase;
class LabeledStatementDefault;
class ExpressionStatement;
class SelectionStatement;
class SelectionStatementIf;
class SelectionStatementSwitch;
class IterationStatement;
class IterationStatementWhile;
class IterationStatementDoWhile;
class IterationStatementFor;
class JumpStatement;
class JumpStatementContinue;
class JumpStatementBreak;
class JumpStatementGoto;
class JumpStatementReturn;
class Declarator;
class AbstractDeclarator;
class DirectDeclarator;
class DirectDeclaratorIdentifier;
class DirectDeclaratorParenthesized;
class DirectDeclaratorArray;
class DirectDeclaratorFunction;
class DirectAbstractDeclarator;
class DirectAbstractDeclaratorParenthesized;
class DirectAbstractDeclaratorArray;
class DirectAbstractDeclaratorFunction;
class ParameterDeclaration;
class ParameterDeclarationDecl;
class ParameterDeclarationAbstDecl;
class ParameterDeclarationAnonymous;
class ParameterList;
class InitDeclarator;
class Declaration;
class DeclarationNormal;
class DeclarationStaticAssert;
class TypeName;
class CompoundStatement;
class FunctionDefinition;
class TranslationUnit;

class Visitor
{
public:
  virtual void visit(GenericAssociation* n) {}
  virtual void visit(GenericSelection* n) {}
  virtual void visit(Identifier* n) {}
  virtual void visit(ConstantI* n) {}
  virtual void visit(ConstantF* n) {}
  virtual void visit(EnumerationConstantUse* n) {}
  virtual void visit(StringLiteral* n) {}
  virtual void visit(FuncName* n) {}
  virtual void visit(ParenthesizedExpression* n) {}
  virtual void visit(CommaExpression* n) {}
  virtual void visit(AssignmentExpression* n) {}
  virtual void visit(ConditionalExpression* n) {}
  virtual void visit(PostfixExpressionIncOp* n) {}
  virtual void visit(PostfixExpressionDecOp* n) {}
  virtual void visit(PostfixExpressionCallOp* n) {}
  virtual void visit(PostfixExpressionDotOp* n) {}
  virtual void visit(PostfixExpressionPtrOp* n) {}
  virtual void visit(PostfixExpressionIndexOp* n) {}
  virtual void visit(PostfixExpressionCastOp* n) {}
  virtual void visit(ArithmeticExpression* n) {}
  virtual void visit(CastExpression* n) {}
  virtual void visit(UnaryExpressionExpr* n) {}
  virtual void visit(UnaryExpressionOnType* n) {}

  virtual Destination visitExpr(Identifier* n)
  {
    return {};
  }

  virtual Destination visitExpr(ConstantI* n)
  {
    return {};
  }

  virtual Destination visitExpr(ConstantF* n)
  {
    return {};
  }

  virtual Destination visitExpr(EnumerationConstantUse* n)
  {
    return {};
  }

  virtual Destination visitExpr(StringLiteral* n)
  {
    return {};
  }

  virtual Destination visitExpr(FuncName* n)
  {
    return {};
  }

  virtual Destination visitExpr(GenericSelection* n)
  {
    return {};
  }

  virtual Destination visitExpr(ParenthesizedExpression* n)
  {
    return {};
  }

  virtual Destination visitExpr(CommaExpression* n)
  {
    return {};
  }

  virtual Destination visitExpr(AssignmentExpression* n)
  {
    return {};
  }

  virtual Destination visitExpr(ConditionalExpression* n)
  {
    return {};
  }

  virtual Destination visitExpr(PostfixExpressionIncOp* n)
  {
    return {};
  }

  virtual Destination visitExpr(PostfixExpressionDecOp* n)
  {
    return {};
  }

  virtual Destination visitExpr(PostfixExpressionCallOp* n)
  {
    return {};
  }

  virtual Destination visitExpr(PostfixExpressionDotOp* n)
  {
    return {};
  }

  virtual Destination visitExpr(PostfixExpressionPtrOp* n)
  {
    return {};
  }

  virtual Destination visitExpr(PostfixExpressionIndexOp* n)
  {
    return {};
  }

  virtual Destination visitExpr(PostfixExpressionCastOp* n)
  {
    return {};
  }

  virtual Destination visitExpr(ArithmeticExpression* n)
  {
    return {};
  }

  virtual Destination visitExpr(CastExpression* n)
  {
    return {};
  }

  virtual Destination visitExpr(UnaryExpressionExpr* n)
  {
    return {};
  }

  virtual Destination visitExpr(UnaryExpressionOnType* n)
  {
    return {};
  }


  virtual void visit(DesignatorField* n) {}
  virtual void visit(DesignatorIndex* n) {}
  virtual void visit(Designation* n) {}
  virtual void visit(InitializerExpression* n) {}
  virtual void visit(InitializerNested* n) {}
  virtual void visit(StaticAssertDeclaration* n) {}
  virtual void visit(Enumerator* n) {}
  virtual void visit(TypeSpecifierBuiltin* n) {}
  virtual void visit(Specifiers* n) {}
  virtual void visit(TypeSpecifierAtomic* n) {}
  virtual void visit(TypeSpecifierTypedefName* n) {}
  virtual void visit(StorageClassSpecifier* n) {}
  virtual void visit(FunctionSpecifier* n) {}
  virtual void visit(TypeQualifier* n) {}
  virtual void visit(StructDeclarator* n) {}
  virtual void visit(StructDeclarationNormal* n) {}
  virtual void visit(StructDeclarationStaticAssert* n) {}
  virtual void visit(TypeSpecifierStructOrUnion* n) {}
  virtual void visit(AlignmentSpecifier* n) {}
  virtual void visit(DeclarationSpecifiers* n) {}
  virtual void visit(Pointer* n) {}
  virtual void visit(TypeSpecifierEnum* n) {}
  virtual void visit(LabeledStatementIdentifier* n) {}
  virtual void visit(LabeledStatementCase* n) {}
  virtual void visit(LabeledStatementDefault* n) {}
  virtual void visit(ExpressionStatement* n) {}
  virtual void visit(SelectionStatementIf* n) {}
  virtual void visit(SelectionStatementSwitch* n) {}
  virtual void visit(IterationStatementWhile* n) {}
  virtual void visit(IterationStatementDoWhile* n) {}
  virtual void visit(IterationStatementFor* n) {}
  virtual void visit(JumpStatementContinue* n) {}
  virtual void visit(JumpStatementBreak* n) {}
  virtual void visit(JumpStatementGoto* n) {}
  virtual void visit(JumpStatementReturn* n) {}
  virtual void visit(Declarator* n) {}
  virtual void visit(AbstractDeclarator* n) {}
  virtual void visit(DirectDeclaratorIdentifier* n) {}
  virtual void visit(DirectDeclaratorParenthesized* n) {}
  virtual void visit(DirectDeclaratorArray* n) {}
  virtual void visit(DirectDeclaratorFunction* n) {}
  virtual void visit(DirectAbstractDeclaratorParenthesized* n) {}
  virtual void visit(DirectAbstractDeclaratorArray* n) {}
  virtual void visit(DirectAbstractDeclaratorFunction* n) {}
  virtual void visit(ParameterDeclarationDecl* n) {}
  virtual void visit(ParameterDeclarationAbstDecl* n) {}
  virtual void visit(ParameterDeclarationAnonymous* n) {}
  virtual void visit(ParameterList* n) {}
  virtual void visit(InitDeclarator* n) {}
  virtual void visit(DeclarationNormal* n) {}
  virtual void visit(DeclarationStaticAssert* n) {}
  virtual void visit(TypeName* n) {}
  virtual void visit(CompoundStatement* n) {}
  virtual void visit(FunctionDefinition* n) {}
  virtual void visit(TranslationUnit* n) {}
};

class Node
{
protected:
  Node(NodeKind kind)
    : Kind(kind)
  {}
  virtual ~Node() {}

public:
  NodeKind getKind() const
  {
    return Kind;
  }

  virtual void accept(Visitor* visitor) = 0;

  virtual void acceptChildren(Visitor* visitor) = 0;

private:
  const NodeKind Kind;
  int lineno;
};

class Expression : public Node
{
protected:
  Expression(NodeKind kind)
    : Node(kind)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() >= E_COMMA_EXPRESSION && node->getKind() <= E_ARITHMETIC_EXPRESSION;
  }

  virtual bool isConst() const
  {
    return false;
  }

  virtual Value eval() const
  {
    return Value();
  }

  virtual Destination acceptExprVisitor(Visitor* visitor) = 0;
};

class PrimaryExpression : public Expression
{
protected:
  PrimaryExpression(NodeKind kind)
    : Expression(kind)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() >= P_IDENTIFIER && node->getKind() <= P_GENERIC_SELECTION;
  }
};

class GenericAssociation : public Node
{
public:
  GenericAssociation(TypeName* typeName, Expression* assignmentExpression)
    : Node(N_GENERIC_ASSOCIATION)
    , typeName(typeName)
    , assignmentExpression(assignmentExpression)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == N_GENERIC_ASSOCIATION;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override;

public:
  TypeName* typeName;
  Expression* assignmentExpression;
};

class GenericSelection : public PrimaryExpression
{
public:
  GenericSelection(Expression* assignmentExpression,
                   std::vector<GenericAssociation*>* genericAssociationList)
    : PrimaryExpression(P_GENERIC_SELECTION)
    , assignmentExpression(assignmentExpression)
    , genericAssociationList(genericAssociationList)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    assignmentExpression->accept(visitor);
    for (auto genericAssociation : *genericAssociationList)
      genericAssociation->accept(visitor);
  }

  Value eval() const override
  {
    return Value();
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* assignmentExpression;
  std::vector<GenericAssociation*>* genericAssociationList;
};

class Identifier : public PrimaryExpression
{
public:
  Identifier(char* sVal)
    : PrimaryExpression(P_IDENTIFIER)
    , name(sVal)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == P_IDENTIFIER;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

  Value eval() const override
  {
    return Value(name);
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  const char* name;
};

class ConstantI : public PrimaryExpression
{
public:
  ConstantI(int iVal)
    : PrimaryExpression(P_CONSTANT_I)
    , val(iVal)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == P_CONSTANT_I;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

  bool isConst() const override
  {
    return true;
  }

  Value eval() const override
  {
    return Value(val);
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  int val;
};

class ConstantF : public PrimaryExpression
{
public:
  ConstantF(float fVal)
    : PrimaryExpression(P_CONSTANT_F)
    , val(fVal)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == P_CONSTANT_F;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

  bool isConst() const override
  {
    return true;
  }

  Value eval() const override
  {
    return Value(val);
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  float val;
};

class EnumerationConstantUse : public PrimaryExpression
{
public:
  EnumerationConstantUse(char* sVal)
    : PrimaryExpression(P_ENUMERATION_CONSTANT)
    , _enum(sVal)
  {
    // TODO: resolve
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == P_ENUMERATION_CONSTANT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

  bool isConst() const override
  {
    return true;
  }

  Value eval() const override
  {
    return Value(_enum);
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  const char* _enum;
};

class StringLiteral : public PrimaryExpression
{
public:
  StringLiteral(char* sVal)
    : PrimaryExpression(P_STRING_LITERAL)
    , val(sVal)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == P_STRING_LITERAL;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

  bool isConst() const override
  {
    return true;
  }

  Value eval() const override
  {
    return Value(val);
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  const char* val;
};

class FuncName : public PrimaryExpression
{
public:
  FuncName()
    : PrimaryExpression(P_FUNC_NAME)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == P_FUNC_NAME;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

  bool isConst() const override
  {
    return true;
  }

  Value eval() const override
  {
    return Value("__func__");
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }
};

class ParenthesizedExpression : public PrimaryExpression
{
public:
  ParenthesizedExpression(Expression* expression)
    : PrimaryExpression(P_PARENTHESIZED_EXPRESSION)
    , expr(expression)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == P_PARENTHESIZED_EXPRESSION;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    expr->accept(visitor);
  }

  bool isConst() const override
  {
    return expr->isConst();
  }

  Value eval() const override
  {
    return expr->eval();
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* expr;
};

class UnaryExpression : public Expression
{
protected:
  UnaryExpression(NodeKind kind)
    : Expression(kind)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() >= U_INC_OP && node->getKind() <= U_ALIGNOF_OP;
  }
};

class UnaryExpressionExpr : public UnaryExpression
{
public:
  UnaryExpressionExpr(NodeKind kind, Expression* expression)
    : UnaryExpression(kind)
    , expression(expression)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() >= U_INC_OP && node->getKind() <= U_NOT_OP;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    expression->accept(visitor);
  }

  bool isConst() const override
  {
    return expression->isConst();   // TODO
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* expression;
};

class UnaryExpressionOnType : public UnaryExpression
{
public:
  UnaryExpressionOnType(NodeKind kind, TypeName* typeName)
    : UnaryExpression(kind)
    , typeName(typeName)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() >= U_SIZEOF_OP && node->getKind() <= U_ALIGNOF_OP;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override;

  bool isConst() const override
  {
    return true;
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  TypeName* typeName;
};

class CommaExpression : public Expression
{
public:
  CommaExpression()
    : Expression(E_COMMA_EXPRESSION)
    , expressionList()
  {}

  void push_back(Expression* expression)
  {
    expressionList.push_back(expression);
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == E_COMMA_EXPRESSION;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    for (auto* expression : expressionList)
      expression->accept(visitor);
  }

  bool isConst() const override
  {
    return expressionList.back()->isConst();
  }

public:
  std::vector<Expression*> expressionList;
};

class AssignmentExpression : public Expression
{
public:
  enum AssignmentOperator
  {
    ASSIGN,
    MUL_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN,
    ADD_ASSIGN,
    SUB_ASSIGN,
    LEFT_ASSIGN,
    RIGHT_ASSIGN,
    AND_ASSIGN,
    XOR_ASSIGN,
    OR_ASSIGN,
  };

public:
  AssignmentExpression(Expression* unaryExpression, AssignmentOperator assignmentOperator,
                       Expression* expression)
    : Expression(E_ASSIGNMENT_EXPRESSION)
    , lhs(unaryExpression)
    , op(assignmentOperator)
    , rhs(expression)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == E_ASSIGNMENT_EXPRESSION;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    lhs->accept(visitor);
    rhs->accept(visitor);
  }

  bool isConst() const override
  {
    return op == ASSIGN && rhs->isConst();
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* lhs;
  AssignmentOperator op;
  Expression* rhs;
};

class ConditionalExpression : public Expression
{
public:
  ConditionalExpression(Expression* pred, Expression* expression1, Expression* expression2)
    : Expression(E_CONDITIONAL_EXPRESSION)
    , pred(pred)
    , exprTrue(expression1)
    , exprFalse(expression2)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == E_CONDITIONAL_EXPRESSION;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    pred->accept(visitor);
    exprTrue->accept(visitor);
    if (exprFalse)
      exprFalse->accept(visitor);
  }

  bool isConst() const override
  {
    return pred->isConst() && exprTrue->isConst();   // TODO
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* pred;
  Expression* exprTrue;
  Expression* exprFalse;
};

class PostfixExpression : public Expression
{
protected:
  PostfixExpression(NodeKind kind)
    : Expression(kind)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() >= PFE_INC && node->getKind() <= PFE_INIT;
  }
};

class PostfixExpressionIncOp : public PostfixExpression
{
public:
  PostfixExpressionIncOp(Expression* primaryExpression)
    : PostfixExpression(PFE_INC)
    , primaryExpression(primaryExpression)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == PFE_INC;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    primaryExpression->accept(visitor);
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

private:
  Expression* primaryExpression;
};

class PostfixExpressionDecOp : public PostfixExpression
{
public:
  PostfixExpressionDecOp(Expression* primaryExpression)
    : PostfixExpression(PFE_DEC)
    , primaryExpression(primaryExpression)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == PFE_DEC;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    primaryExpression->accept(visitor);
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

private:
  Expression* primaryExpression;
};

class PostfixExpressionCallOp : public PostfixExpression
{
public:
  PostfixExpressionCallOp(Expression* primaryExpression,
                          std::vector<Expression*>* argumentExpressionList = nullptr)
    : PostfixExpression(PFE_CALL)
    , primaryExpression(primaryExpression)
    , argumentExpressionList(argumentExpressionList)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == PFE_CALL;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    primaryExpression->accept(visitor);
    if (argumentExpressionList) {
      for (auto argumentExpression : *argumentExpressionList)
        argumentExpression->accept(visitor);
    }
  }

  Value eval() const override
  {
    return Value();
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* primaryExpression;
  std::vector<Expression*>* argumentExpressionList;
};

class PostfixExpressionDotOp : public PostfixExpression
{
public:
  PostfixExpressionDotOp(Expression* primaryExpression, char* identifier)
    : PostfixExpression(PFE_FIELD)
    , primaryExpression(primaryExpression)
    , identifier(identifier)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == PFE_FIELD;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    primaryExpression->accept(visitor);
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

private:
  Expression* primaryExpression;
  const char* identifier;
};

class PostfixExpressionPtrOp : public PostfixExpression
{
public:
  PostfixExpressionPtrOp(Expression* primaryExpression, char* identifier)
    : PostfixExpression(PFE_PTR_FIELD)
    , primaryExpression(primaryExpression)
    , identifier(identifier)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == PFE_PTR_FIELD;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    primaryExpression->accept(visitor);
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

private:
  Expression* primaryExpression;
  const char* identifier;
};

class PostfixExpressionIndexOp : public PostfixExpression
{
public:
  PostfixExpressionIndexOp(Expression* primaryExpression, Expression* expression)
    : PostfixExpression(PFE_INDEX)
    , expr(primaryExpression)
    , index(expression)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == PFE_INDEX;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    expr->accept(visitor);
    index->accept(visitor);
  }

  bool isConst() const override
  {
    return expr->isConst() && index->isConst();
  }

  Value eval() const override
  {
    Value off = index->eval();
    return expr->eval().index(off);
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

private:
  Expression* expr;
  Expression* index;
};

class PostfixExpressionCastOp : public PostfixExpression
{
public:
  PostfixExpressionCastOp(TypeName* typeName, std::vector<Initializer*>* initializerList)
    : PostfixExpression(PFE_INIT)
    , typeName(typeName)
    , initializerList(initializerList)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == PFE_INIT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override;

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

private:
  TypeName* typeName;
  std::vector<Initializer*>* initializerList;
};

class ArithmeticExpression : public Expression
{
public:
  enum ArithmeticOperator
  {
    A_MUL_OP,
    A_DIV_OP,
    A_MOD_OP,
    A_ADD_OP,
    A_SUB_OP,
    A_LEFT_OP,
    A_RIGHT_OP,
    R_LT_OP,
    R_GT_OP,
    R_LE_OP,
    R_GE_OP,
    E_EQ_OP,
    E_NE_OP,
    B_AND_OP,
    B_XOR_OP,
    B_OR_OP,
    L_AND_OP,
    L_OR_OP,
  };

  static const char* to_string(ArithmeticOperator op)
  {
    switch (op) {
      case A_MUL_OP:
        return "*";
      case A_DIV_OP:
        return "/";
      case A_MOD_OP:
        return "%";
      case A_ADD_OP:
        return "+";
      case A_SUB_OP:
        return "-";
      case A_LEFT_OP:
        return "<<";
      case A_RIGHT_OP:
        return ">>";
      case R_LT_OP:
        return "<";
      case R_GT_OP:
        return ">";
      case R_LE_OP:
        return "<=";
      case R_GE_OP:
        return ">=";
      case E_EQ_OP:
        return "==";
      case E_NE_OP:
        return "!=";
      case B_AND_OP:
        return "&";
      case B_XOR_OP:
        return "^";
      case B_OR_OP:
        return "|";
      case L_AND_OP:
        return "&&";
      default:
        return "||";
    }
  }

public:
  ArithmeticExpression(Expression* expression1, ArithmeticOperator arithmeticOperator,
                       Expression* expression2)
    : Expression(E_ARITHMETIC_EXPRESSION)
    , lhs(expression1)
    , op(arithmeticOperator)
    , rhs(expression2)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == E_ARITHMETIC_EXPRESSION;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    lhs->accept(visitor);
    rhs->accept(visitor);
  }

  bool isConst() const override
  {
    return lhs->isConst() && rhs->isConst();
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* lhs;
  ArithmeticOperator op;
  Expression* rhs;
};

class CastExpression : public Expression
{
public:
  CastExpression(TypeName* typeName, Expression* expression)
    : Expression(E_CAST_EXPRESSION)
    , typeName(typeName)
    , expr(expression)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == E_CAST_EXPRESSION;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override;

  bool isConst() const override
  {
    return expr->isConst();
  }

  Destination acceptExprVisitor(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  TypeName* typeName;
  Expression* expr;
};

class Designator : public Node
{
protected:
  Designator(NodeKind kind)
    : Node(kind)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() >= D_FIELD_DESIGNATOR && node->getKind() <= D_INDEX_DESIGNATOR;
  }
};

class DesignatorField : public Designator
{
public:
  DesignatorField(char* identifier)
    : Designator(D_FIELD_DESIGNATOR)
    , identifier(identifier)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == D_FIELD_DESIGNATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

public:
  const char* identifier;
};

class DesignatorIndex : public Designator
{
public:
  DesignatorIndex(Expression* constantExpression)
    : Designator(D_INDEX_DESIGNATOR)
    , constantExpression(constantExpression)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == D_INDEX_DESIGNATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    constantExpression->accept(visitor);
  }

public:
  Expression* constantExpression;
};

class Designation : public Node
{
public:
  Designation(std::vector<Designator*>* designator)
    : Node(N_DESIGNATION)
    , designator(designator)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {

    for (auto d : *designator) {
      d->accept(visitor);
    }
  }

public:
  std::vector<Designator*>* designator;
};

class Initializer : public Node
{
protected:
  Initializer(NodeKind kind, Designation* designation = nullptr)
    : Node(kind)
    , designation(designation)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() == I_NESTED_INITIALIZER || node->getKind() == I_EXPRESSION;
  }

public:
  Designation* designation;
};

class InitializerExpression : public Initializer
{
public:
  InitializerExpression(Expression* assignmentExpression)
    : Initializer(I_EXPRESSION)
    , expr(assignmentExpression)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == I_EXPRESSION;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    expr->accept(visitor);
  }

public:
  Expression* expr;
};

class InitializerNested : public Initializer
{
public:
  InitializerNested(std::vector<Initializer*>* initializerList)
    : Initializer(I_NESTED_INITIALIZER)
    , initializers(initializerList)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == I_NESTED_INITIALIZER;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    for (auto initializer : *initializers) {
      initializer->accept(visitor);
    }
  }

public:
  std::vector<Initializer*>* initializers;
};

class StaticAssertDeclaration : public Node
{
public:
  StaticAssertDeclaration(Expression* constantExpression, char* stringLiteral)
    : Node(N_STATIC_ASSERT_DECLARATION)
    , constantExpression(constantExpression)
    , stringLiteral(stringLiteral)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == N_STATIC_ASSERT_DECLARATION;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {

    constantExpression->accept(visitor);
  }

public:
  Expression* constantExpression;
  const char* stringLiteral;
};

class Enumerator : public Node
{
public:
  Enumerator(Identifier* identifier, Expression* constantExpression = nullptr)
    : Node(N_ENUMERATOR)
    , identifier(identifier)
    , constantExpression(constantExpression)
  {
    // TODO: check constantExpression type
    if (constantExpression) {
      assert(constantExpression->isConst());
    }
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == N_ENUMERATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {

    identifier->accept(visitor);
    constantExpression->accept(visitor);
  }

public:
  Identifier* identifier;
  Expression* constantExpression;
};

class TypeSpecifier : public Node
{
protected:
  TypeSpecifier(NodeKind kind)
    : Node(kind)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() >= T_BUILTIN && node->getKind() <= T_TYPEDEF_NAME;
  }

  bool isBuiltin() const
  {
    return isa<TypeSpecifierBuiltin>(this);
  }
};

class TypeSpecifierBuiltin : public TypeSpecifier
{
public:
  enum BuiltinType
  {
    VOID,
    CHAR,
    SHORT,
    INT,
    LONG,
    FLOAT,
    DOUBLE,
    SIGNED,
    UNSIGNED,
    BOOL,
    COMPLEX,
    IMAGINARY,
  };

public:
  TypeSpecifierBuiltin(BuiltinType type)
    : TypeSpecifier(T_BUILTIN)
    , type(type)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == T_BUILTIN;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

  const char* typeName() const
  {
    const char* arr[] = {
      "void",
      "char",
      "short",
      "int",
      "long",
      "float",
      "double",
      "signed",
      "unsigned",
      "bool",
      "complex",
      "imaginary",
    };
    return arr[type];
  }

public:
  BuiltinType type;
};

class Specifiers : public Node
{
public:
  Specifiers()
    : Node(N_SPECIFIER_QUALIFIER_LIST)
    , specs()
  {}

  void push_back(TypeSpecifier* typeSpecifier)
  {
    specs.push_back(typeSpecifier);
  }

  void push_back(TypeQualifier* typeQualifier)
  {
    specs.push_back((Node*)typeQualifier);
  }

  std::vector<Node*>::iterator begin()
  {
    return specs.begin();
  }

  std::vector<Node*>::iterator end()
  {
    return specs.end();
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    for (auto typeSpecifier : specs) {
      typeSpecifier->accept(visitor);
    }
  }

public:
  std::vector<Node*> specs;
};

class TypeSpecifierAtomic : public TypeSpecifier
{
public:
  TypeSpecifierAtomic(Node* typeName)
    : TypeSpecifier(T_ATOMIC)
    , typeName(typeName)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == T_ATOMIC;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    typeName->accept(visitor);
  }

public:
  Node* typeName;
};

class TypeSpecifierTypedefName : public TypeSpecifier
{
public:
  TypeSpecifierTypedefName(char* name)
    : TypeSpecifier(T_TYPEDEF_NAME)
    , name(name)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == T_TYPEDEF_NAME;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

public:
  const char* name;
};

class StructDeclarator : public Node
{
public:
  StructDeclarator(Declarator* declarator, Expression* constantExpression = nullptr)
    : Node(N_STRUCT_DECLARATOR)
    , declarator(declarator)
    , bitWidth(constantExpression)
  {
    assert(declarator || bitWidth);
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == N_STRUCT_DECLARATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override;

public:
  Declarator* declarator;
  Expression* bitWidth;
};

class StructDeclaration : public Node
{
protected:
  StructDeclaration(NodeKind kind)
    : Node(kind)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() == N_STRUCT_DECLARATION_NORMAL ||
           node->getKind() == N_STRUCT_DECLARATION_STATIC;
  }
};

class StructDeclarationNormal : public StructDeclaration
{
public:
  StructDeclarationNormal(Specifiers* specifierQualifierList,
                          std::vector<StructDeclarator*>* structDeclaratorList = nullptr)
    : StructDeclaration(N_STRUCT_DECLARATION_NORMAL)
    , specifiers(specifierQualifierList)
    , structDecls(structDeclaratorList)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == N_STRUCT_DECLARATION_NORMAL;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    specifiers->accept(visitor);

    if (structDecls) {
      for (auto* structDeclarator : *structDecls) {
        structDeclarator->accept(visitor);
      }
    }
  }

public:
  Specifiers* specifiers;
  std::vector<StructDeclarator*>* structDecls;
};

class StructDeclarationStaticAssert : public StructDeclaration
{
public:
  StructDeclarationStaticAssert(StaticAssertDeclaration* staticAssertDeclaration)
    : StructDeclaration(N_STRUCT_DECLARATION_STATIC)
    , staticAssertDeclaration(staticAssertDeclaration)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == N_STRUCT_DECLARATION_STATIC;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {

    staticAssertDeclaration->accept(visitor);
  }

public:
  StaticAssertDeclaration* staticAssertDeclaration;
};

class TypeSpecifierStructOrUnion : public TypeSpecifier
{
public:
  enum StructOrUnion
  {
    S_STRUCT,
    S_UNION,
  };

public:
  TypeSpecifierStructOrUnion(StructOrUnion type, const char* name,
                             std::vector<StructDeclaration*>* decls = nullptr)
    : TypeSpecifier(T_STRUCT)
    , type(type)
    , name(name)
    , declarations(decls)
  {}

  bool isAnonymous() const
  {
    return name == nullptr;
  }

  bool isEmpty() const
  {
    return !declarations || declarations->empty();
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == T_STRUCT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    if (declarations) {
      for (auto structDeclaration : *declarations)
        structDeclaration->accept(visitor);
    }
  }

public:
  StructOrUnion type;
  const char* name;
  std::vector<StructDeclaration*>* declarations;
};

class AlignmentSpecifier : public Node
{
public:
  AlignmentSpecifier(TypeName* typeName)
    : Node(N_ALIGNMENT_SPECIFIER)
    , typeName(typeName)
    , constantExpression(nullptr)
  {}

  AlignmentSpecifier(Expression* constantExpression)
    : Node(N_ALIGNMENT_SPECIFIER)
    , typeName(nullptr)
    , constantExpression(constantExpression)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == N_ALIGNMENT_SPECIFIER;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override;

public:
  TypeName* typeName;
  Expression* constantExpression;
};

class StorageClassSpecifier : public Node
{
public:
  enum StorageClass
  {
    TYPEDEF,
    EXTERN,
    STATIC,
    THREAD_LOCAL,
    AUTO,
    REGISTER,
  };

public:
  StorageClassSpecifier(StorageClass storageClass)
    : Node(N_STORAGE_CLASS_SPECIFIER)
    , storageClass(storageClass)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == N_STORAGE_CLASS_SPECIFIER;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

public:
  StorageClass storageClass;
};

class FunctionSpecifier : public Node
{
public:
  enum EFunctionSpecifier
  {
    INLINE,
    NORETURN,
  };

public:
  FunctionSpecifier(EFunctionSpecifier funcSpec)
    : Node(N_STORAGE_CLASS_SPECIFIER)
    , funcSpec(funcSpec)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == N_STORAGE_CLASS_SPECIFIER;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

public:
  EFunctionSpecifier funcSpec;
};

class TypeQualifier : public Node
{
public:
  enum ETypeQualifier
  {
    CONST,
    RESTRICT,
    VOLATILE,
    ATOMIC,
  };

public:
  TypeQualifier(ETypeQualifier typeQualifier)
    : Node(N_TYPE_QUALIFIER)
    , typeQualifier(typeQualifier)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == N_TYPE_QUALIFIER;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

public:
  ETypeQualifier typeQualifier;
};

class DeclarationSpecifiers : public Node
{
public:
  DeclarationSpecifiers()
    : Node(N_DECLARATION_SPECIFIERS)
  {}

  void push_back(StorageClassSpecifier* storageClassSpecifier)
  {
    specs.push_back(storageClassSpecifier);
  }

  void push_back(TypeSpecifier* type)
  {
    // TODO: validate
    // auto cur = std::find(typeSpecs.begin(), typeSpecs.end(), [](Node
    // *i){return isa<TypeSpecifier>(i);});

    specs.push_back(type);
  }

  void push_back(TypeQualifier* typeQualifier)
  {
    specs.push_back(typeQualifier);
  }

  void push_back(FunctionSpecifier* functionSpecifier)
  {
    specs.push_back(functionSpecifier);
  }

  void push_back(AlignmentSpecifier* alignmentSpecifier)
  {
    specs.push_back(alignmentSpecifier);
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == N_DECLARATION_SPECIFIERS;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    for (auto* spec : specs)
      spec->accept(visitor);
  }

public:
  std::vector<Node*> specs;
};

class Pointer : public Node
{
public:
  Pointer(std::vector<TypeQualifier*>* typeQualifierList = nullptr, Pointer* pointer = nullptr)
    : Node(N_POINTER)
    , typeQuals(typeQualifierList)
    , pointer(pointer)
  {}

  bool is_empty() const
  {
    return pointer == nullptr;
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == N_POINTER;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    if (pointer)
      pointer->accept(visitor);
  }

public:
  std::vector<TypeQualifier*>* typeQuals;
  Pointer* pointer;
};

class TypeSpecifierEnum : public TypeSpecifier
{
public:
  TypeSpecifierEnum(char* identifier, std::vector<Enumerator*>* enumeratorList)
    : TypeSpecifier(T_ENUM)
    , name(identifier)
    , enumerators(enumeratorList)
  {}

  TypeSpecifierEnum(char* identifier)
    : TypeSpecifierEnum(identifier, nullptr)
  {}

  TypeSpecifierEnum(std::vector<Enumerator*>* enumeratorList)
    : TypeSpecifierEnum(nullptr, enumeratorList)
  {}

  bool is_anonymous() const
  {
    return name == nullptr;
  }

  bool with_declaration() const
  {
    return !enumerators->empty();
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == T_ENUM;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {

    if (enumerators) {
      for (auto enumerator : *enumerators)
        enumerator->accept(visitor);
    }
  }

public:
  const char* name;
  std::vector<Enumerator*>* enumerators;
};

class Statement : public Node
{
protected:
  Statement(NodeKind kind)
    : Node(kind)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() >= L_IDENTIFIER_STATEMENT && node->getKind() <= S_RETURN_STATEMENT;
  }
};

class LabeledStatement : public Statement
{
protected:
  LabeledStatement(NodeKind kind, Statement* statement)
    : Statement(kind)
    , statement(statement)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() >= L_IDENTIFIER_STATEMENT && node->getKind() <= L_DEFAULT_STATEMENT;
  }

public:
  Statement* statement;
};

class LabeledStatementIdentifier : public LabeledStatement
{
public:
  LabeledStatementIdentifier(char* identifier, Statement* statement)
    : LabeledStatement(L_IDENTIFIER_STATEMENT, statement)
    , identifier(identifier)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == L_IDENTIFIER_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

public:
  const char* identifier;
};

class LabeledStatementCase : public LabeledStatement
{
public:
  LabeledStatementCase(Node* constantExpression, Statement* statement)
    : LabeledStatement(L_CASE_STATEMENT, statement)
    , constantExpression(constantExpression)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == L_CASE_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {

    constantExpression->accept(visitor);
  }

public:
  Node* constantExpression;
};

class LabeledStatementDefault : public LabeledStatement
{
public:
  LabeledStatementDefault(Statement* statement)
    : LabeledStatement(L_DEFAULT_STATEMENT, statement)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == L_DEFAULT_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}
};

class ExpressionStatement : public Statement
{
public:
  ExpressionStatement(Expression* expr = nullptr)
    : Statement(S_EXPRESSION_STATEMENT)
    , expr(expr)
  {}

  bool isEmpty() const
  {
    return expr == nullptr;
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == S_EXPRESSION_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    if (expr)
      expr->accept(visitor);
  }

public:
  Expression* expr;
};

class SelectionStatement : public Statement
{
protected:
  SelectionStatement(NodeKind kind)
    : Statement(kind)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() == S_IF_STATEMENT || node->getKind() == S_SWITCH_STATEMENT;
  }
};

class SelectionStatementIf : public SelectionStatement
{
public:
  SelectionStatementIf(Expression* expression, Statement* statement,
                       Statement* elseStatement = nullptr)
    : SelectionStatement(S_IF_STATEMENT)
    , expression(expression)
    , statement(statement)
    , elseStatement(elseStatement)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == S_IF_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    expression->accept(visitor);
    statement->accept(visitor);
    if (elseStatement) {
      elseStatement->accept(visitor);
    }
  }

public:
  Expression* expression;
  Statement* statement;
  Statement* elseStatement;
};

class SelectionStatementSwitch : public SelectionStatement
{
public:
  SelectionStatementSwitch(Expression* expression, Statement* statement)
    : SelectionStatement(S_SWITCH_STATEMENT)
    , expression(expression)
    , statement(statement)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == S_SWITCH_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    expression->accept(visitor);
    statement->accept(visitor);
  }

public:
  Expression* expression;
  Statement* statement;
};

class IterationStatement : public Statement
{
protected:
  IterationStatement(NodeKind kind)
    : Statement(kind)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() >= S_WHILE_STATEMENT && node->getKind() <= S_FOR_STATEMENT;
  }
};

class IterationStatementWhile : public IterationStatement
{
public:
  IterationStatementWhile(Expression* expression, Statement* statement)
    : IterationStatement(S_WHILE_STATEMENT)
    , expression(expression)
    , statement(statement)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == S_WHILE_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    expression->accept(visitor);
    statement->accept(visitor);
  }

public:
  Expression* expression;
  Statement* statement;
};

class IterationStatementDoWhile : public IterationStatement
{
public:
  IterationStatementDoWhile(Expression* expression, Statement* statement)
    : IterationStatement(S_DO_WHILE_STATEMENT)
    , expression(expression)
    , statement(statement)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == S_DO_WHILE_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    expression->accept(visitor);
    statement->accept(visitor);
  }

public:
  Expression* expression;
  Statement* statement;
};

class IterationStatementFor : public IterationStatement
{
public:
  IterationStatementFor(Node* declOrExpr, Node* condition, Expression* update, Statement* statement)
    : IterationStatement(S_FOR_STATEMENT)
    , update(update)
    , statement(statement)
  {
    if (isa<Declaration>(declOrExpr)) {
      declaration = cast<Declaration>(declOrExpr);
    } else if (isa<Expression>(declOrExpr)) {
      init = cast<Expression>(declOrExpr);
    } else {
      assert(false);
    }

    if (isa<Statement>(condition)) {
      auto statement = cast<Statement>(condition);
      assert(isa<ExpressionStatement>(statement));
      this->condition = cast<ExpressionStatement>(statement)->expr;
    } else {
      assert(false);
    }
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == S_FOR_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override;

public:
  Declaration* declaration;
  Expression* init;
  Expression* condition;
  Expression* update;
  Statement* statement;
};

class JumpStatement : public Statement
{
protected:
  JumpStatement(NodeKind kind)
    : Statement(kind)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() >= S_GOTO_STATEMENT && node->getKind() <= S_RETURN_STATEMENT;
  }
};

class JumpStatementContinue : public JumpStatement
{
public:
  JumpStatementContinue()
    : JumpStatement(S_CONTINUE_STATEMENT)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == S_CONTINUE_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}
};

class JumpStatementBreak : public JumpStatement
{
public:
  JumpStatementBreak()
    : JumpStatement(S_BREAK_STATEMENT)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == S_BREAK_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}
};

class JumpStatementGoto : public JumpStatement
{
public:
  JumpStatementGoto(char* identifier)
    : JumpStatement(S_GOTO_STATEMENT)
    , identifier(identifier)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == S_GOTO_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

public:
  const char* identifier;
};

class JumpStatementReturn : public JumpStatement
{
public:
  JumpStatementReturn(Expression* expression = nullptr)
    : JumpStatement(S_RETURN_STATEMENT)
    , expression(expression)
  {}

  bool is_empty() const
  {
    return expression == nullptr;
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == S_RETURN_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    if (expression)
      expression->accept(visitor);
  }

public:
  Expression* expression;
};

class Declarator : public Node
{
public:
  Declarator(Pointer* pointer, DirectDeclarator* directDeclarator)
    : Node(N_DECLARATOR)
    , pointer(pointer)
    , directDeclarator(directDeclarator)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == N_DECLARATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override;

  const char* name() const;

public:
  Pointer* pointer;
  DirectDeclarator* directDeclarator;
};

class AbstractDeclarator : public Node
{
public:
  AbstractDeclarator(Pointer* pointer, DirectAbstractDeclarator* directDeclarator = nullptr)
    : Node(N_ABSTRACT_DECLARATOR)
    , pointer(pointer)
    , directAbstractDeclarator(directDeclarator)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == N_ABSTRACT_DECLARATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override;

public:
  Pointer* pointer;
  DirectAbstractDeclarator* directAbstractDeclarator;
};

class DirectDeclarator : public Node
{
protected:
  DirectDeclarator(NodeKind kind)
    : Node(kind)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() >= D_IDENTIFIER && node->getKind() <= D_FUNCTION_DECLARATOR;
  }

public:
  virtual const char* name() const = 0;
};

class DirectDeclaratorIdentifier : public DirectDeclarator
{
public:
  DirectDeclaratorIdentifier(char* identifier)
    : DirectDeclarator(D_IDENTIFIER)
    , identifier(identifier)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == D_IDENTIFIER;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override {}

  const char* name() const override
  {
    return identifier;
  }

public:
  const char* identifier;
};

class DirectDeclaratorParenthesized : public DirectDeclarator
{
public:
  DirectDeclaratorParenthesized(Declarator* declarator)
    : DirectDeclarator(D_PARENTHESIZED_DECLARATOR)
    , declarator(declarator)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == D_PARENTHESIZED_DECLARATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    declarator->accept(visitor);
  }

  const char* name() const override
  {
    return declarator->name();
  }

public:
  Declarator* declarator;
};

class DirectDeclaratorArray : public DirectDeclarator
{
public:
  DirectDeclaratorArray(DirectDeclarator* declarator,
                        std::vector<TypeQualifier*>* typeQualifierList = nullptr,
                        Expression* expression = nullptr, bool static_ = false, bool any_ = false)
    : DirectDeclarator(D_ARRAY_DECLARATOR)
    , directDecl(declarator)
    , typeQualifierList(typeQualifierList)
    , expression(expression)
    , static_(static_)
    , any_(any_)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == D_ARRAY_DECLARATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    directDecl->accept(visitor);
    if (expression)
      expression->accept(visitor);
  }

  const char* name() const override
  {
    return directDecl->name();
  }


public:
  DirectDeclarator* directDecl;
  std::vector<TypeQualifier*>* typeQualifierList;
  Expression* expression;
  bool static_;
  bool any_;
};

class DirectDeclaratorFunction : public DirectDeclarator
{
public:
  enum DirectDeclaratorFunctionType
  {
    D_PARAMETER_TYPE_LIST,
    D_IDENTIFIER_LIST,
    D_NO_PARAMETER,
  };

public:
  DirectDeclaratorFunction(DirectDeclarator* declarator, ParameterList* parameterTypeList)
    : DirectDeclarator(D_FUNCTION_DECLARATOR)
    , directDeclaratorFunctionType(D_PARAMETER_TYPE_LIST)
    , directDecl(declarator)
    , parameterList(parameterTypeList)
  {}

  DirectDeclaratorFunction(DirectDeclarator* declarator, std::vector<const char*>* identifierList)
    : DirectDeclarator(D_FUNCTION_DECLARATOR)
    , directDeclaratorFunctionType(D_IDENTIFIER_LIST)
    , directDecl(declarator)
    , identifierList(identifierList)
  {}

  DirectDeclaratorFunction(DirectDeclarator* declarator)
    : DirectDeclarator(D_FUNCTION_DECLARATOR)
    , directDeclaratorFunctionType(D_NO_PARAMETER)
    , directDecl(declarator)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == D_FUNCTION_DECLARATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override;

  const char* name() const override
  {
    return directDecl->name();
  }

public:
  DirectDeclaratorFunctionType directDeclaratorFunctionType;
  DirectDeclarator* directDecl;
  ParameterList* parameterList;
  std::vector<const char*>* identifierList;
};

class DirectAbstractDeclarator : public Node
{
protected:
  DirectAbstractDeclarator(NodeKind kind)
    : Node(kind)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() >= D_PARENTHESIZED_ABSTRACT_DECLARATOR &&
           node->getKind() <= D_FUNCTION_ABSTRACT_DECLARATOR;
  }
};

class DirectAbstractDeclaratorParenthesized : public DirectAbstractDeclarator
{
public:
  DirectAbstractDeclaratorParenthesized(AbstractDeclarator* abstractDeclarator)
    : DirectAbstractDeclarator(D_PARENTHESIZED_ABSTRACT_DECLARATOR)
    , abstractDeclarator(abstractDeclarator)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == D_PARENTHESIZED_ABSTRACT_DECLARATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    abstractDeclarator->accept(visitor);
  }

public:
  AbstractDeclarator* abstractDeclarator;
};

class DirectAbstractDeclaratorArray : public DirectAbstractDeclarator
{
public:
  DirectAbstractDeclaratorArray(DirectAbstractDeclarator* abstractDeclarator,
                                std::vector<TypeQualifier*>* typeQualifierList,
                                Expression* assignmentExpression, bool static_ = false)
    : DirectAbstractDeclarator(D_ARRAY_ABSTRACT_DECLARATOR)
    , abstractDeclarator(abstractDeclarator)
    , typeQualifierList(typeQualifierList)
    , assignmentExpression(assignmentExpression)
    , static_(static_)
    , any_(false)
  {}

  DirectAbstractDeclaratorArray(DirectAbstractDeclarator* abstractDeclarator, bool any_ = false)
    : DirectAbstractDeclarator(D_ARRAY_ABSTRACT_DECLARATOR)
    , abstractDeclarator(abstractDeclarator)
    , assignmentExpression(nullptr)
    , static_(false)
    , any_(any_)
  {}

  DirectAbstractDeclaratorArray(bool any_ = false)
    : DirectAbstractDeclarator(D_ARRAY_ABSTRACT_DECLARATOR)
    , abstractDeclarator(nullptr)
    , assignmentExpression(nullptr)
    , static_(false)
    , any_(any_)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == D_ARRAY_ABSTRACT_DECLARATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    abstractDeclarator->accept(visitor);
    if (assignmentExpression) {
      assignmentExpression->accept(visitor);
    }
  }

public:
  DirectAbstractDeclarator* abstractDeclarator;
  std::vector<TypeQualifier*>* typeQualifierList;
  Expression* assignmentExpression;
  bool static_;
  bool any_;
};

class DirectAbstractDeclaratorFunction : public DirectAbstractDeclarator
{
public:
  DirectAbstractDeclaratorFunction(DirectAbstractDeclarator* abstractDeclarator,
                                   ParameterList* parameterTypeList = nullptr)
    : DirectAbstractDeclarator(D_FUNCTION_ABSTRACT_DECLARATOR)
    , abstractDeclarator(abstractDeclarator)
    , parameterTypeList(parameterTypeList)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == D_FUNCTION_ABSTRACT_DECLARATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override;

public:
  DirectAbstractDeclarator* abstractDeclarator;
  ParameterList* parameterTypeList;
};

class ParameterDeclaration : public Node
{
protected:
  ParameterDeclaration(NodeKind kind, DeclarationSpecifiers* declarationSpecifiers)
    : Node(kind)
    , declarationSpecifiers(declarationSpecifiers)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() >= PAR_DECLARATOR && node->getKind() <= PAR_ANONYMOUS;
  }

public:
  DeclarationSpecifiers* declarationSpecifiers;
};

class ParameterDeclarationDecl : public ParameterDeclaration
{
public:
  ParameterDeclarationDecl(DeclarationSpecifiers* declarationSpecifiers, Declarator* declarator)
    : ParameterDeclaration(PAR_DECLARATOR, declarationSpecifiers)
    , declarator(declarator)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == PAR_DECLARATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    declarationSpecifiers->accept(visitor);
    declarator->accept(visitor);
  }

private:
  Declarator* declarator;
};

class ParameterDeclarationAbstDecl : public ParameterDeclaration
{
public:
  ParameterDeclarationAbstDecl(DeclarationSpecifiers* declarationSpecifiers,
                               AbstractDeclarator* abstractDeclarator)
    : ParameterDeclaration(PAR_ABSTRACT_DECLARATOR, declarationSpecifiers)
    , abstractDeclarator(abstractDeclarator)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == PAR_ABSTRACT_DECLARATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    declarationSpecifiers->accept(visitor);
    abstractDeclarator->accept(visitor);
  }

private:
  AbstractDeclarator* abstractDeclarator;
};

class ParameterDeclarationAnonymous : public ParameterDeclaration
{
public:
  ParameterDeclarationAnonymous(DeclarationSpecifiers* declarationSpecifiers)
    : ParameterDeclaration(PAR_ANONYMOUS, declarationSpecifiers)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == PAR_ANONYMOUS;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    declarationSpecifiers->accept(visitor);
  }
};

class ParameterList : public Node
{
public:
  ParameterList()
    : Node(N_PARAMETER_LIST)
    , parameterList(new std::vector<ParameterDeclaration*>)
    , hasEllipsis(false)
  {}

  // implement begin and end
  auto begin() const
  {
    return parameterList->begin();
  }

  auto end() const
  {
    return parameterList->end();
  }

  void push_back(ParameterDeclaration* parameterDeclaration)
  {
    parameterList->push_back(parameterDeclaration);
  }

  void setHasEllipsis(bool hasEllipsis)
  {
    this->hasEllipsis = hasEllipsis;
  }

  static bool classof(Node* node)
  {
    return node->getKind() == N_PARAMETER_LIST;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    for (auto parameterDeclaration : *parameterList) {
      parameterDeclaration->accept(visitor);
    }
  }

public:
  std::vector<ParameterDeclaration*>* parameterList;
  bool hasEllipsis;
};

class InitDeclarator : public Node
{
public:
  InitDeclarator(Declarator* declarator, Initializer* initializer = nullptr)
    : Node(N_INIT_DECLARATOR)
    , declarator(declarator)
    , initializer(initializer)
  {
    // if (!declarator->is_variable()) {
    //   assert(initializer == nullptr && "function  cannot be initialized");
    // }
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == N_INIT_DECLARATOR;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    declarator->accept(visitor);
    if (initializer)
      initializer->accept(visitor);
  }

  const char* name() const
  {
    return declarator->name();
  }

public:
  Declarator* declarator;
  Initializer* initializer;
};

class Declaration : public Node
{
protected:
  Declaration(NodeKind kind)
    : Node(kind)
  {}

public:
  static bool classof(const Node* node)
  {
    return node->getKind() == N_DECLARATION_NORMAL || node->getKind() == N_DECLARATION_STATIC;
  }
};

class DeclarationNormal : public Declaration
{
public:
  DeclarationNormal(DeclarationSpecifiers* declarationSpecifiers,
                    std::vector<InitDeclarator*>* initDeclaratorList = nullptr)
    : Declaration(N_DECLARATION_NORMAL)
    , declSpecs(declarationSpecifiers)
    , initDecls(initDeclaratorList)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == N_DECLARATION_NORMAL;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    declSpecs->accept(visitor);
    if (initDecls) {
      for (auto initDeclarator : *initDecls)
        initDeclarator->accept(visitor);
    }
  }

public:
  DeclarationSpecifiers* declSpecs;
  std::vector<InitDeclarator*>* initDecls;
};

class DeclarationStaticAssert : public Declaration
{
public:
  DeclarationStaticAssert(StaticAssertDeclaration* staticAssertDeclaration)
    : Declaration(N_DECLARATION_STATIC)
    , staticAssertDeclaration(staticAssertDeclaration)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == N_DECLARATION_STATIC;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    staticAssertDeclaration->accept(visitor);
  }

public:
  StaticAssertDeclaration* staticAssertDeclaration;
};

class TypeName : public Node
{
public:
  TypeName(Specifiers* specifierQualifierList, AbstractDeclarator* abstractDeclarator = nullptr)
    : Node(N_TYPE_NAME)
    , specifierQualifierList(specifierQualifierList)
    , abstractDeclarator(abstractDeclarator)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == N_TYPE_NAME;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    specifierQualifierList->accept(visitor);
    if (abstractDeclarator)
      abstractDeclarator->accept(visitor);
  }

public:
  Specifiers* specifierQualifierList;
  AbstractDeclarator* abstractDeclarator;
};

class CompoundStatement : public Statement
{
public:
  CompoundStatement(std::vector<Node*>* blockItemList = nullptr)
    : Statement(S_COMPOUND_STATEMENT)
    , blockItems(blockItemList)
  {}

  static bool classof(const Node* node)
  {
    return node->getKind() == S_COMPOUND_STATEMENT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    if (blockItems) {
      for (auto item : *blockItems) {
        assert(isa<Declaration>(item) || isa<Statement>(item));
        item->accept(visitor);
      }
    }
  }

public:
  std::vector<Node*>* blockItems;
};

class FunctionDefinition : public Node
{
public:
  FunctionDefinition(DeclarationSpecifiers* declarationSpecifiers, Declarator* declarator,
                     std::vector<Declaration*>* declarationList,
                     CompoundStatement* compoundStatement)
    : Node(N_FUNCTION_DEFINITION)
    , declSpecs(declarationSpecifiers)
    , declarator(declarator)
    , params(declarationList)
    , compoundStatement(compoundStatement)
  {
    // assert(declarator->is_variable());
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == N_FUNCTION_DEFINITION;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    declSpecs->accept(visitor);
    declarator->accept(visitor);
    if (params != nullptr) {
      for (auto declaration : *params)
        declaration->accept(visitor);
    }
    compoundStatement->accept(visitor);
  }

  const char* name() const
  {
    return declarator->name();
  }

public:
  DeclarationSpecifiers* declSpecs;
  Declarator* declarator;
  std::vector<Declaration*>* params;
  CompoundStatement* compoundStatement;
};

class TranslationUnit : public Node
{
public:
  TranslationUnit()
    : Node(N_TRANSLATION_UNIT)
  {}

  void push_back(Node* node)
  {
    assert(isa<FunctionDefinition>(node) || isa<Declaration>(node));

    if (isa<FunctionDefinition>(node))
      funcDefOrDecl.push_back(cast<FunctionDefinition>(node));
    else
      funcDefOrDecl.push_back(cast<Declaration>(node));
  }

  static bool classof(const Node* node)
  {
    return node->getKind() == N_TRANSLATION_UNIT;
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor) override
  {
    for (auto* i : funcDefOrDecl)
      i->accept(visitor);
  }

public:
  std::vector<Node*> funcDefOrDecl;
};
