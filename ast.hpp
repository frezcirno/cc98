#pragma once
#include "c2.hpp"
#include "backend.hpp"
#include "type.hpp"
#include <cassert>
#include <iostream>
#include <vector>
#include <optional>

class AbstractArrayDeclarator;
class AbstractDeclarator;
class AbstractDeclarator;
class AbstractDirectDeclarator;
class AbstractFunctionDeclarator;
class AbstractParameterDeclaration;
class AbstractParenthesizedDeclarator;
class AlignmentSpecifier;
class AnonymousParameterDeclaration;
class ArithmeticExpression;
class ArrayDeclarator;
class AssignmentExpression;
class AtomicTypeSpecifier;
class BreakStatement;
class CaseStatement;
class CastExpression;
class CommaExpression;
class CompoundStatement;
class ConditionalExpression;
class ConstantF;
class ConstantI;
class ContinueStatement;
class Declaration;
class DeclarationSpecifierList;
class Declarator;
class DefaultStatement;
class Designation;
class DirectDeclarator;
class DoWhileStatement;
class EnumerationConstantUse;
class Enumerator;
class EnumTypeSpecifier;
class ExpressionInitializer;
class ExpressionStatement;
class FieldDesignator;
class ForStatement;
class FuncName;
class FunctionDeclarator;
class FunctionDefinition;
class FunctionSpecifier;
class GenericAssociation;
class GenericSelection;
class GotoStatement;
class Identifier;
class IdentifierDeclarator;
class IfStatement;
class IndexDesignator;
class InitDeclarator;
class Initializer;
class LabeledStatement;
class NestedInitializer;
class NormalDeclaration;
class ParameterDeclaration;
class NormalStructDeclaration;
class ParameterList;
class ParenthesizedDeclarator;
class ParenthesizedExpression;
class Pointer;
class PostfixExpressionCallOp;
class PostfixExpressionCastOp;
class PostfixExpressionDecOp;
class PostfixExpressionDotOp;
class PostfixExpressionIncOp;
class PostfixExpressionIndexOp;
class PostfixExpressionPtrOp;
class ReturnStatement;
class SpecifierQualifierList;
class StaticAssert;
class StorageClassSpecifier;
class StringLiteral;
class StructDeclarator;
class StructUnionTypeSpecifier;
class SwitchStatement;
class TranslationUnit;
class TypeName;
class TypeQualifier;
class BuiltinTypeSpecifier;
class TypedefTypeSpecifier;
class UnaryExpressionOnExpr;
class UnaryExpressionOnType;
class WhileStatement;

class Node
{
public:
  virtual void accept(Visitor* visitor) = 0;
};

class Visitor
{
public:
  virtual void visit(AbstractArrayDeclarator* n) {}
  virtual void visit(AbstractDeclarator* n) {}
  virtual void visit(AbstractFunctionDeclarator* n) {}
  virtual void visit(AbstractParameterDeclaration* n) {}
  virtual void visit(AbstractParenthesizedDeclarator* n) {}
  virtual void visit(AlignmentSpecifier* n) {}
  virtual void visit(AnonymousParameterDeclaration* n) {}
  virtual void visit(ArithmeticExpression* n) {}
  virtual void visit(ArrayDeclarator* n) {}
  virtual void visit(AssignmentExpression* n) {}
  virtual void visit(AtomicTypeSpecifier* n) {}
  virtual void visit(BreakStatement* n) {}
  virtual void visit(CaseStatement* n) {}
  virtual void visit(CastExpression* n) {}
  virtual void visit(CommaExpression* n) {}
  virtual void visit(CompoundStatement* n) {}
  virtual void visit(ConditionalExpression* n) {}
  virtual void visit(ConstantF* n) {}
  virtual void visit(ConstantI* n) {}
  virtual void visit(ContinueStatement* n) {}
  virtual void visit(DeclarationSpecifierList* n) {}
  virtual void visit(Declarator* n) {}
  virtual void visit(DefaultStatement* n) {}
  virtual void visit(Designation* n) {}
  virtual void visit(DoWhileStatement* n) {}
  virtual void visit(EnumerationConstantUse* n) {}
  virtual void visit(Enumerator* n) {}
  virtual void visit(EnumTypeSpecifier* n) {}
  virtual void visit(ExpressionInitializer* n) {}
  virtual void visit(ExpressionStatement* n) {}
  virtual void visit(FieldDesignator* n) {}
  virtual void visit(ForStatement* n) {}
  virtual void visit(FuncName* n) {}
  virtual void visit(FunctionDeclarator* n) {}
  virtual void visit(FunctionDefinition* n) {}
  virtual void visit(FunctionSpecifier* n) {}
  virtual void visit(GenericAssociation* n) {}
  virtual void visit(GenericSelection* n) {}
  virtual void visit(GotoStatement* n) {}
  virtual void visit(Identifier* n) {}
  virtual void visit(IdentifierDeclarator* n) {}
  virtual void visit(IfStatement* n) {}
  virtual void visit(IndexDesignator* n) {}
  virtual void visit(InitDeclarator* n) {}
  virtual void visit(LabeledStatement* n) {}
  virtual void visit(NestedInitializer* n) {}
  virtual void visit(NormalDeclaration* n) {}
  virtual void visit(ParameterDeclaration* n) {}
  virtual void visit(NormalStructDeclaration* n) {}
  virtual void visit(ParameterList* n) {}
  virtual void visit(ParenthesizedDeclarator* n) {}
  virtual void visit(ParenthesizedExpression* n) {}
  virtual void visit(Pointer* n) {}
  virtual void visit(PostfixExpressionCallOp* n) {}
  virtual void visit(PostfixExpressionCastOp* n) {}
  virtual void visit(PostfixExpressionDecOp* n) {}
  virtual void visit(PostfixExpressionDotOp* n) {}
  virtual void visit(PostfixExpressionIncOp* n) {}
  virtual void visit(PostfixExpressionIndexOp* n) {}
  virtual void visit(PostfixExpressionPtrOp* n) {}
  virtual void visit(ReturnStatement* n) {}
  virtual void visit(SpecifierQualifierList* n) {}
  virtual void visit(StaticAssert* n) {}
  virtual void visit(StorageClassSpecifier* n) {}
  virtual void visit(StringLiteral* n) {}
  virtual void visit(StructDeclarator* n) {}
  virtual void visit(StructUnionTypeSpecifier* n) {}
  virtual void visit(SwitchStatement* n) {}
  virtual void visit(TranslationUnit* n) {}
  virtual void visit(TypeName* n) {}
  virtual void visit(TypeQualifier* n) {}
  virtual void visit(BuiltinTypeSpecifier* n) {}
  virtual void visit(TypedefTypeSpecifier* n) {}
  virtual void visit(UnaryExpressionOnExpr* n) {}
  virtual void visit(UnaryExpressionOnType* n) {}
  virtual void visit(WhileStatement* n) {}


  virtual Value* visitExpr(Identifier* n)
  {
    return {};
  }

  virtual Value* visitExpr(ConstantI* n)
  {
    return {};
  }

  virtual Value* visitExpr(ConstantF* n)
  {
    return {};
  }

  virtual Value* visitExpr(EnumerationConstantUse* n)
  {
    return {};
  }

  virtual Value* visitExpr(StringLiteral* n)
  {
    return {};
  }

  virtual Value* visitExpr(FuncName* n)
  {
    return {};
  }

  virtual Value* visitExpr(GenericSelection* n)
  {
    return {};
  }

  virtual Value* visitExpr(ParenthesizedExpression* n)
  {
    return {};
  }

  virtual Value* visitExpr(CommaExpression* n)
  {
    return {};
  }

  virtual Value* visitExpr(AssignmentExpression* n)
  {
    return {};
  }

  virtual Value* visitExpr(ConditionalExpression* n)
  {
    return {};
  }

  virtual Value* visitExpr(PostfixExpressionIncOp* n)
  {
    return {};
  }

  virtual Value* visitExpr(PostfixExpressionDecOp* n)
  {
    return {};
  }

  virtual Value* visitExpr(PostfixExpressionCallOp* n)
  {
    return {};
  }

  virtual Value* visitExpr(PostfixExpressionDotOp* n)
  {
    return {};
  }

  virtual Value* visitExpr(PostfixExpressionPtrOp* n)
  {
    return {};
  }

  virtual Value* visitExpr(PostfixExpressionIndexOp* n)
  {
    return {};
  }

  virtual Value* visitExpr(PostfixExpressionCastOp* n)
  {
    return {};
  }

  virtual Value* visitExpr(ArithmeticExpression* n)
  {
    return {};
  }

  virtual Value* visitExpr(CastExpression* n)
  {
    return {};
  }

  virtual Value* visitExpr(UnaryExpressionOnExpr* n)
  {
    return {};
  }

  virtual Value* visitExpr(UnaryExpressionOnType* n)
  {
    return {};
  }
};


class FuncUnit : virtual public Node
{};

class FileUnit : virtual public Node
{};

class Expression : public Node
{
public:
  virtual bool isConst() const
  {
    return false;
  }

  virtual Imm* evalConst(const ScopeManager& scopes) const
  {
    throw std::runtime_error("not a constant expression");
  }

  virtual Value* acceptExpr(Visitor* visitor) = 0;
};

class PrimaryExpression : public Expression
{};

class Identifier : public PrimaryExpression
{
public:
  Identifier(char* sVal)
    : PrimaryExpression()
    , name(sVal)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  Value* acceptExpr(Visitor* visitor) override
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
    : PrimaryExpression()
    , val(iVal)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  bool isConst() const override
  {
    return true;
  }

  Imm* evalConst(const ScopeManager& scopes) const override
  {
    return new Imm(val);
  }

  Value* acceptExpr(Visitor* visitor) override
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
    : PrimaryExpression()
    , val(fVal)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  bool isConst() const override
  {
    return true;
  }

  Imm* evalConst(const ScopeManager& scopes) const override
  {
    return new Imm(val);
  }

  Value* acceptExpr(Visitor* visitor) override
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
    : PrimaryExpression()
    , _enum(sVal)
  {
    // TODO: resolve
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }


  bool isConst() const override
  {
    return true;
  }

  Imm* evalConst(const ScopeManager& scopes) const override
  {
    Symbol* sym = scopes.find(_enum, SymbolKind::SK_ENUMERATION_CONSTANT);
    return sym->val;
  }

  Value* acceptExpr(Visitor* visitor) override
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
    : PrimaryExpression()
    , val(sVal)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }


  bool isConst() const override
  {
    return true;
  }

  Imm* evalConst(const ScopeManager& scopes) const override
  {
    // typ: const char*
    int len = strlen(val);
    return new Imm(val, ty_char.array_of(len));
  }

  Value* acceptExpr(Visitor* visitor) override
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
    : PrimaryExpression()
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }


  bool isConst() const override
  {
    return true;
  }

  Imm* evalConst(const ScopeManager& scopes) const override
  {
    const char* name = scopes.find("__func__", SymbolKind::SK_FUNCTION)->getName();
    return new Imm(name, ty_char.array_of(strlen(name)));
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }
};

class ParenthesizedExpression : public PrimaryExpression
{
public:
  ParenthesizedExpression(Expression* expression)
    : PrimaryExpression()
    , expr(expression)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  bool isConst() const override
  {
    return expr->isConst();
  }

  Imm* evalConst(const ScopeManager& scopes) const override
  {
    return expr->evalConst(scopes);
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* expr;
};

class UnaryExpression : public Expression
{
protected:
  UnaryExpression()
    : Expression()
  {}
};

class UnaryExpressionOnExpr : public UnaryExpression
{
public:
  enum UnaryOperator
  {
    U_INC_OP,
    U_DEC_OP,
    U_ADDR_OP,
    U_DEREF_OP,
    U_POS_OP,
    U_NEG_OP,
    U_REV_OP,
    U_NOT_OP,
    U_SIZEOF_OP,
  };

  UnaryExpressionOnExpr(UnaryOperator op, Expression* expression)
    : UnaryExpression()
    , op(op)
    , expr(expression)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  bool isConst() const override
  {
    return expr->isConst();   // TODO
  }

  Imm* evalConst(const ScopeManager& scopes) const override
  {
    return expr->evalConst(scopes);
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  UnaryOperator op;
  Expression* expr;
};

class UnaryExpressionOnType : public UnaryExpression
{
public:
  enum TypeOperator
  {
    U_SIZEOF_OP,
    U_ALIGNOF_OP
  };

  UnaryExpressionOnType(TypeOperator kind, TypeName* typeName)
    : UnaryExpression()
    , op(kind)
    , typeName(typeName)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  void acceptChildren(Visitor* visitor);

  bool isConst() const override
  {
    return true;
  }

  Imm* evalConst(const ScopeManager& scopes) const override
  {
    // TODO: resolve
    if (op == U_SIZEOF_OP)
      return new Imm(0x1234);
    else
      return new Imm(0x5678);
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  TypeOperator op;
  TypeName* typeName;
};

class CommaExpression : public Expression
{
public:
  CommaExpression()
    : Expression()
    , exprs()
  {}

  void push_back(Expression* expression)
  {
    exprs.push_back(expression);
  }

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  bool isConst() const override
  {
    return exprs.back()->isConst();
  }

public:
  std::vector<Expression*> exprs;
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
  AssignmentExpression(Expression* unaryExpression, AssignmentOperator op, Expression* expression)
    : Expression()
    , lhs(unaryExpression)
    , op(op)
    , rhs(expression)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  bool isConst() const override
  {
    return op == ASSIGN && rhs->isConst();
  }

  Value* acceptExpr(Visitor* visitor) override
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
    : Expression()
    , cond(pred)
    , then(expression1)
    , els(expression2)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  bool isConst() const override
  {
    return cond->isConst() && then->isConst();   // TODO
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* cond;
  Expression* then;
  Expression* els;
};

class PostfixExpressionIncOp : public Expression
{
public:
  PostfixExpressionIncOp(Expression* primaryExpression)
    : Expression()
    , expr(primaryExpression)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* expr;
};

class PostfixExpressionDecOp : public Expression
{
public:
  PostfixExpressionDecOp(Expression* primaryExpression)
    : Expression()
    , expr(primaryExpression)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* expr;
};

class PostfixExpressionCallOp : public Expression
{
public:
  PostfixExpressionCallOp(Expression* primaryExpression,
                          std::vector<Expression*>* argumentExpressionList = nullptr)
    : Expression()
    , expr(primaryExpression)
    , args(argumentExpressionList)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* expr;
  std::vector<Expression*>* args;
};

class PostfixExpressionDotOp : public Expression
{
public:
  PostfixExpressionDotOp(Expression* primaryExpression, char* identifier)
    : Expression()
    , expr(primaryExpression)
    , field(identifier)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* expr;
  const char* field;
};

class PostfixExpressionPtrOp : public Expression
{
public:
  PostfixExpressionPtrOp(Expression* primaryExpression, char* identifier)
    : Expression()
    , expr(primaryExpression)
    , field(identifier)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* expr;
  const char* field;
};

class PostfixExpressionIndexOp : public Expression
{
public:
  PostfixExpressionIndexOp(Expression* primaryExpression, Expression* expression)
    : Expression()
    , expr(primaryExpression)
    , index(expression)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  bool isConst() const override
  {
    return expr->isConst() && index->isConst();
  }

  Imm* evalConst(const ScopeManager& scopes) const override
  {
    Imm* off = index->evalConst(scopes);
    return expr->evalConst(scopes)->index(off);
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* expr;
  Expression* index;
};

class PostfixExpressionCastOp : public Expression
{
public:
  PostfixExpressionCastOp(TypeName* typeName, std::vector<Initializer*>* initializerList)
    : Expression()
    , typeName(typeName)
    , initializerList(initializerList)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
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
  ArithmeticExpression(Expression* expression1, ArithmeticOperator op, Expression* expression2)
    : Expression()
    , lhs(expression1)
    , op(op)
    , rhs(expression2)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  bool isConst() const override
  {
    return lhs->isConst() && rhs->isConst();
  }

  Value* acceptExpr(Visitor* visitor) override
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
    : Expression()
    , typeName(typeName)
    , expr(expression)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  bool isConst() const override
  {
    return expr->isConst();
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  TypeName* typeName;
  Expression* expr;
};

class Designator : public Node
{};

class FieldDesignator : public Designator
{
public:
  FieldDesignator(char* identifier)
    : field(identifier)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  const char* field;
};

class IndexDesignator : public Designator
{
public:
  IndexDesignator(Expression* constantExpression)
    : index(constantExpression)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Expression* index;
};

class Designation : public Node
{
public:
  Designation(std::vector<Designator*>* designator)
    : designators(designator)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  std::vector<Designator*>* designators;
};

class Initializer : public Node
{
protected:
  Initializer(Designation* designation = nullptr)
    : designation(designation)
  {}

public:
  Designation* designation;
};

class ExpressionInitializer : public Initializer
{
public:
  ExpressionInitializer(Expression* assignmentExpression)
    : Initializer()
    , expr(assignmentExpression)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Expression* expr;
};

class NestedInitializer : public Initializer
{
public:
  NestedInitializer(std::vector<Initializer*>* initializerList)
    : Initializer()
    , initializers(initializerList)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  std::vector<Initializer*>* initializers;
};

class Enumerator : public Node
{
public:
  Enumerator(Identifier* identifier, Expression* constantExpression = nullptr)
    : identifier(identifier)
    , expr(constantExpression)
  {
    // TODO: check constantExpression type
    if (constantExpression) {
      assert(constantExpression->isConst());
    }
  }

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Identifier* identifier;
  Expression* expr;
};

class DeclarationSpecifier : public Node
{};

class SpecifierQualifier : public DeclarationSpecifier
{};

class TypeSpecifier : public SpecifierQualifier
{
protected:
  TypeSpecifier() {}
};

class BuiltinTypeSpecifier : public TypeSpecifier
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
  BuiltinTypeSpecifier(BuiltinType type)
    : TypeSpecifier()
    , type(type)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }


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

class SpecifierQualifierList : public Node
{
public:
  SpecifierQualifierList() {}

  void push_back(TypeSpecifier* typeSpecifier)
  {
    specs.push_back((SpecifierQualifier*)typeSpecifier);
  }

  void push_back(TypeQualifier* typeQualifier)
  {
    specs.push_back((SpecifierQualifier*)typeQualifier);
  }

  std::vector<SpecifierQualifier*>::iterator begin()
  {
    return specs.begin();
  }

  std::vector<SpecifierQualifier*>::iterator end()
  {
    return specs.end();
  }

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  std::vector<SpecifierQualifier*> specs;
};

class AtomicTypeSpecifier : public TypeSpecifier
{
public:
  AtomicTypeSpecifier(TypeName* typeName)
    : TypeSpecifier()
    , typeName(typeName)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  TypeName* typeName;
};

class TypedefTypeSpecifier : public TypeSpecifier
{
public:
  TypedefTypeSpecifier(char* name)
    : TypeSpecifier()
    , name(name)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  const char* name;
};

class StructDeclarator : public Node
{
public:
  StructDeclarator(Declarator* declarator, Expression* constantExpression = nullptr)
    : decl(declarator)
    , bitWidth(constantExpression)
  {
    assert(declarator || bitWidth);
  }

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Declarator* decl;
  Expression* bitWidth;
};

class StructDeclaration : public Node
{};

class NormalStructDeclaration : public StructDeclaration
{
public:
  NormalStructDeclaration(SpecifierQualifierList* specifierQualifierList,
                          std::vector<StructDeclarator*>* structDeclaratorList = nullptr)
    : declSpecs(specifierQualifierList)
    , declarators(structDeclaratorList)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  SpecifierQualifierList* declSpecs;
  std::vector<StructDeclarator*>* declarators;
};

class StructUnionTypeSpecifier : public TypeSpecifier
{
public:
  enum StructOrUnionKind
  {
    S_STRUCT,
    S_UNION,
  };

public:
  StructUnionTypeSpecifier(StructOrUnionKind kind, const char* name,
                           std::vector<StructDeclaration*>* decls = nullptr)
    : TypeSpecifier()
    , kind(kind)
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

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  StructOrUnionKind kind;
  const char* name;
  std::vector<StructDeclaration*>* declarations;
};

class AlignmentSpecifier : public DeclarationSpecifier
{
public:
  AlignmentSpecifier(TypeName* typeName)
    : typeName(typeName)
    , expr(nullptr)
  {}

  AlignmentSpecifier(Expression* constantExpression)
    : typeName(nullptr)
    , expr(constantExpression)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  TypeName* typeName;
  Expression* expr;
};

class StorageClassSpecifier : public DeclarationSpecifier
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
    : storageClass(storageClass)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  StorageClass storageClass;
};

class FunctionSpecifier : public DeclarationSpecifier
{
public:
  enum EFunctionSpecifier
  {
    INLINE,
    NORETURN,
  };

public:
  FunctionSpecifier(EFunctionSpecifier funcSpec)
    : functionSpecifier(funcSpec)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  EFunctionSpecifier functionSpecifier;
};

class TypeQualifier : public SpecifierQualifier
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
    : typeQualifier(typeQualifier)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  ETypeQualifier typeQualifier;
};

class DeclarationSpecifierList : public Node
{
public:
  DeclarationSpecifierList() {}

  void push_back(StorageClassSpecifier* storageClassSpecifier)
  {
    specs.push_back((DeclarationSpecifier*)storageClassSpecifier);
  }

  void push_back(TypeSpecifier* type)
  {
    // TODO: validate
    // auto cur = std::find(typeSpecs.begin(), typeSpecs.end(), [](FuncUnit
    // *i){return isa<TypeSpecifier>(i);});

    specs.push_back((DeclarationSpecifier*)type);
  }

  void push_back(TypeQualifier* typeQualifier)
  {
    specs.push_back((DeclarationSpecifier*)typeQualifier);
  }

  void push_back(FunctionSpecifier* functionSpecifier)
  {
    specs.push_back((DeclarationSpecifier*)functionSpecifier);
  }

  void push_back(AlignmentSpecifier* alignmentSpecifier)
  {
    specs.push_back((DeclarationSpecifier*)alignmentSpecifier);
  }

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  std::vector<DeclarationSpecifier*> specs;
};

class Pointer : public Node
{
public:
  Pointer(std::vector<TypeQualifier*>* typeQualifierList = nullptr, Pointer* pointer = nullptr)
    : typeQuals(typeQualifierList)
    , pointer(pointer)
  {}

  bool is_empty() const
  {
    return pointer == nullptr;
  }

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  std::vector<TypeQualifier*>* typeQuals;
  Pointer* pointer;
};

class EnumTypeSpecifier : public TypeSpecifier
{
public:
  EnumTypeSpecifier(char* identifier, std::vector<Enumerator*>* enumeratorList)
    : TypeSpecifier()
    , name(identifier)
    , enumerators(enumeratorList)
  {}

  EnumTypeSpecifier(char* identifier)
    : EnumTypeSpecifier(identifier, nullptr)
  {}

  EnumTypeSpecifier(std::vector<Enumerator*>* enumeratorList)
    : EnumTypeSpecifier(nullptr, enumeratorList)
  {}

  bool is_anonymous() const
  {
    return name == nullptr;
  }

  bool with_declaration() const
  {
    return !enumerators->empty();
  }

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  const char* name;
  std::vector<Enumerator*>* enumerators;
};

class Statement : public FuncUnit
{};

class LabeledStatement : public Statement
{
public:
  LabeledStatement(char* identifier, Statement* statement)
    : Statement()
    , label(identifier)
    , stmt(statement)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  const char* label;
  Statement* stmt;
};

class CaseStatement : public Statement
{
public:
  CaseStatement(Expression* constantExpression, Statement* statement)
    : Statement()
    , expr(constantExpression)
    , stmt(statement)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Expression* expr;
  Statement* stmt;
};

class DefaultStatement : public Statement
{
public:
  DefaultStatement(Statement* statement)
    : Statement()
    , stmt(statement)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Statement* stmt;
};

class ExpressionStatement : public Statement
{
public:
  ExpressionStatement(Expression* expr = nullptr)
    : Statement()
    , expr(expr)
  {}

  bool isEmpty() const
  {
    return expr == nullptr;
  }

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }


public:
  Expression* expr;
};

class IfStatement : public Statement
{
public:
  IfStatement(Expression* expression, Statement* statement, Statement* elseStatement = nullptr)
    : Statement()
    , cond(expression)
    , then(statement)
    , els(elseStatement)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Expression* cond;
  Statement* then;
  Statement* els;
};

class SwitchStatement : public Statement
{
public:
  SwitchStatement(Expression* expression, Statement* statement)
    : Statement()
    , cond(expression)
    , body(statement)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Expression* cond;
  Statement* body;
};

class WhileStatement : public Statement
{
public:
  WhileStatement(Expression* expression, Statement* statement)
    : Statement()
    , cond(expression)
    , body(statement)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Expression* cond;
  Statement* body;
};

class DoWhileStatement : public Statement
{
public:
  DoWhileStatement(Expression* expression, Statement* statement)
    : Statement()
    , cond(expression)
    , body(statement)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Expression* cond;
  Statement* body;
};

class ForStatement : public Statement
{
public:
  ForStatement(ExpressionStatement* init, ExpressionStatement* condition, Expression* update,
               Statement* statement)
    : Statement()
    , decl(nullptr)
    , init(init->expr)
    , cond(condition->expr)
    , inc(update)
    , body(statement)
  {}

  ForStatement(Declaration* decl, ExpressionStatement* condition, Expression* update,
               Statement* statement)
    : Statement()
    , decl(decl)
    , init(nullptr)
    , cond(condition->expr)
    , inc(update)
    , body(statement)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Declaration* decl;
  Expression* init;
  Expression* cond;
  Expression* inc;
  Statement* body;
};

class ContinueStatement : public Statement
{
public:
  ContinueStatement()
    : Statement()
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }
};

class BreakStatement : public Statement
{
public:
  BreakStatement()
    : Statement()
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }
};

class GotoStatement : public Statement
{
public:
  GotoStatement(char* identifier)
    : Statement()
    , label(identifier)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  const char* label;
};

class ReturnStatement : public Statement
{
public:
  ReturnStatement(Expression* expression = nullptr)
    : Statement()
    , expr(expression)
  {}

  bool is_empty() const
  {
    return expr == nullptr;
  }

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Expression* expr;
};

class Declarator : public Node
{
public:
  Declarator(Pointer* pointer, DirectDeclarator* directDeclarator)
    : pointer(pointer)
    , decl(directDeclarator)
  {}


  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

  const char* name() const;

public:
  Pointer* pointer;
  DirectDeclarator* decl;
};

class DirectDeclarator : public Node
{
protected:
  DirectDeclarator() {}

public:
  virtual const char* name() const = 0;
};

class ParameterDeclarationBase : public Node
{
protected:
  ParameterDeclarationBase(DeclarationSpecifierList* declarationSpecifierList)
    : declSpecs(declarationSpecifierList)
  {}

public:
  DeclarationSpecifierList* declSpecs;
};

class ParameterList : public Node
{
public:
  ParameterList()
    : params()
    , hasEllipsis(false)
  {}

  // implement begin and end
  auto begin() const
  {
    return params.begin();
  }

  auto end() const
  {
    return params.end();
  }

  size_t size() const
  {
    return params.size() + hasEllipsis;
  }

  void push_back(ParameterDeclarationBase* parameterDeclaration)
  {
    params.push_back(parameterDeclaration);
  }

  void setHasEllipsis(bool hasEllipsis)
  {
    this->hasEllipsis = hasEllipsis;
  }

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  std::vector<ParameterDeclarationBase*> params;
  bool hasEllipsis;
};

class IdentifierDeclarator : public DirectDeclarator
{
public:
  IdentifierDeclarator(char* identifier)
    : DirectDeclarator()
    , identifier(identifier)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  const char* name() const override
  {
    return identifier;
  }

public:
  const char* identifier;
};

class ParenthesizedDeclarator : public DirectDeclarator
{
public:
  ParenthesizedDeclarator(Declarator* declarator)
    : DirectDeclarator()
    , decl(declarator)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  const char* name() const override
  {
    return decl->name();
  }

public:
  Declarator* decl;
};

class ArrayDeclarator : public DirectDeclarator
{
public:
  ArrayDeclarator(DirectDeclarator* declarator,
                  std::vector<TypeQualifier*>* typeQualifierList = nullptr,
                  Expression* expression = nullptr, bool static_ = false, bool any_ = false)
    : DirectDeclarator()
    , decl(declarator)
    , typeQualifierList(typeQualifierList)
    , expr(expression)
    , static_(static_)
    , any_(any_)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  const char* name() const override
  {
    return decl->name();
  }

public:
  DirectDeclarator* decl;
  std::vector<TypeQualifier*>* typeQualifierList;
  Expression* expr;
  bool static_;
  bool any_;
};

class FunctionDeclarator : public DirectDeclarator
{
public:
  enum FunctionDeclaratorKind
  {
    D_NORMAL,
    D_OLD_STYLE,
  };

public:
  FunctionDeclarator(DirectDeclarator* declarator, ParameterList* parameterTypeList = nullptr)
    : DirectDeclarator()
    , kind(D_NORMAL)
    , decl(declarator)
    , params(parameterTypeList ? parameterTypeList : new ParameterList())
  {}

  FunctionDeclarator(DirectDeclarator* declarator, std::vector<const char*>* identifierList)
    : DirectDeclarator()
    , kind(D_OLD_STYLE)
    , decl(declarator)
    , oldStyleParams(identifierList ? identifierList : new std::vector<const char*>())
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  const char* name() const override
  {
    return decl->name();
  }

public:
  FunctionDeclaratorKind kind;
  DirectDeclarator* decl;
  union
  {
    ParameterList* params;                      // NOT NULL
    std::vector<const char*>* oldStyleParams;   // NOT NULL
  };
};

class AbstractDeclarator : public Node
{
public:
  AbstractDeclarator(Pointer* pointer, AbstractDirectDeclarator* directDeclarator = nullptr)
    : pointer(pointer)
    , decl(directDeclarator)
  {}


  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Pointer* pointer;
  AbstractDirectDeclarator* decl;
};

class AbstractDirectDeclarator : public Node
{};

class AbstractParenthesizedDeclarator : public AbstractDirectDeclarator
{
public:
  AbstractParenthesizedDeclarator(AbstractDeclarator* abstractDeclarator)
    : AbstractDirectDeclarator()
    , decl(abstractDeclarator)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  AbstractDeclarator* decl;
};

class AbstractArrayDeclarator : public AbstractDirectDeclarator
{
public:
  AbstractArrayDeclarator(AbstractDirectDeclarator* abstractDeclarator,
                          std::vector<TypeQualifier*>* typeQualifierList,
                          Expression* assignmentExpression, bool static_ = false)
    : AbstractDirectDeclarator()
    , decl(abstractDeclarator)
    , typeQualifierList(typeQualifierList)
    , expr(assignmentExpression)
    , static_(static_)
    , any_(false)
  {}

  AbstractArrayDeclarator(AbstractDirectDeclarator* abstractDeclarator, bool any_ = false)
    : AbstractDirectDeclarator()
    , decl(abstractDeclarator)
    , expr(nullptr)
    , static_(false)
    , any_(any_)
  {}

  AbstractArrayDeclarator(bool any_ = false)
    : AbstractDirectDeclarator()
    , decl(nullptr)
    , expr(nullptr)
    , static_(false)
    , any_(any_)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }


public:
  AbstractDirectDeclarator* decl;
  std::vector<TypeQualifier*>* typeQualifierList;
  Expression* expr;
  bool static_;
  bool any_;
};

class AbstractFunctionDeclarator : public AbstractDirectDeclarator
{
public:
  AbstractFunctionDeclarator(AbstractDirectDeclarator* abstractDeclarator,
                             ParameterList* parameterTypeList = nullptr)
    : AbstractDirectDeclarator()
    , decl(abstractDeclarator)
    , params(parameterTypeList ? parameterTypeList : new ParameterList())
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  AbstractDirectDeclarator* decl = nullptr;
  ParameterList* params;   // NOT NULL
};

class ParameterDeclaration : public ParameterDeclarationBase
{
public:
  ParameterDeclaration(DeclarationSpecifierList* declarationSpecifierList, Declarator* declarator)
    : ParameterDeclarationBase(declarationSpecifierList)
    , decl(declarator)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Declarator* decl;
};

class AbstractParameterDeclaration : public ParameterDeclarationBase
{
public:
  AbstractParameterDeclaration(DeclarationSpecifierList* declarationSpecifierList,
                               AbstractDeclarator* abstractDeclarator)
    : ParameterDeclarationBase(declarationSpecifierList)
    , decl(abstractDeclarator)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  AbstractDeclarator* decl;
};

class AnonymousParameterDeclaration : public ParameterDeclarationBase
{
public:
  AnonymousParameterDeclaration(DeclarationSpecifierList* declarationSpecifierList)
    : ParameterDeclarationBase(declarationSpecifierList)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }
};

class InitDeclarator : public Node
{
public:
  InitDeclarator(Declarator* declarator, Initializer* initializer = nullptr)
    : decl(declarator)
    , initializer(initializer)
  {
    // if (!declarator->is_variable()) {
    //   assert(initializer == nullptr && "function  cannot be initialized");
    // }
  }

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

  const char* name() const
  {
    return decl->name();
  }

public:
  Declarator* decl;
  Initializer* initializer;
};

class Declaration : public FuncUnit, public FileUnit
{};

class NormalDeclaration : public Declaration
{
public:
  NormalDeclaration(DeclarationSpecifierList* declarationSpecifierList,
                    std::vector<InitDeclarator*>* initDeclaratorList = nullptr)
    : Declaration()
    , declSpecs(declarationSpecifierList)
    , initDecls(initDeclaratorList)
  {}


  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }


public:
  DeclarationSpecifierList* declSpecs;
  std::vector<InitDeclarator*>* initDecls;
};

class StaticAssert : public Declaration, public StructDeclaration
{
public:
  StaticAssert(Expression* constantExpression, char* stringLiteral)
    : expr(constantExpression)
    , stringLiteral(stringLiteral)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  Expression* expr;
  const char* stringLiteral;
};

class TypeName : public Node
{
public:
  TypeName(SpecifierQualifierList* specifierQualifierList,
           AbstractDeclarator* abstractDeclarator = nullptr)
    : declSpecs(specifierQualifierList)
    , abstractDeclarator(abstractDeclarator)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  SpecifierQualifierList* declSpecs;
  AbstractDeclarator* abstractDeclarator;
};

class GenericAssociation : public Node
{
public:
  GenericAssociation(TypeName* typeName, Expression* assignmentExpression)
    : typeName(typeName)
    , expr(assignmentExpression)
  {}

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  TypeName* typeName;
  Expression* expr;
};

class GenericSelection : public PrimaryExpression
{
public:
  GenericSelection(Expression* assignmentExpression,
                   std::vector<GenericAssociation*>* genericAssociationList)
    : PrimaryExpression()
    , expr(assignmentExpression)
    , associations(genericAssociationList)
  {}

  void accept(Visitor* visitor) override
  {
    visitor->visit(this);
  }

  Imm* evalConst(const ScopeManager& scopes) const override
  {
    // TODO
    return nullptr;
  }

  Value* acceptExpr(Visitor* visitor) override
  {
    return visitor->visitExpr(this);
  }

public:
  Expression* expr;
  std::vector<GenericAssociation*>* associations;
};

class CompoundStatement : public Statement
{
public:
  CompoundStatement(std::vector<FuncUnit*>* blockItemList = nullptr)
    : Statement()
  {
    if (blockItemList) {
      blockItems = std::move(*blockItemList);
    }
  }

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  std::vector<FuncUnit*> blockItems;
};

class FunctionDefinition : public FileUnit
{
public:
  FunctionDefinition(DeclarationSpecifierList* declarationSpecifierList, Declarator* declarator,
                     std::vector<Declaration*>* declarationList,
                     CompoundStatement* compoundStatement)
    : declSpecs(declarationSpecifierList)
    , decl(declarator)
    // , params(declarationList)
    , body(compoundStatement)
  {
    // assert(declarator->is_variable());
    if (declarationList) {
      for (auto decl : *declarationList)
        assert(isa<NormalDeclaration>(decl));
      oldStyleParams = (std::vector<NormalDeclaration*>*)declarationList;
    }
  }

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

  const char* name() const
  {
    return decl->name();
  }

public:
  DeclarationSpecifierList* declSpecs;
  Declarator* decl;
  std::vector<NormalDeclaration*>* oldStyleParams = nullptr;
  CompoundStatement* body;
};

class TranslationUnit : public Node
{
public:
  TranslationUnit() {}

  void push_back(FileUnit* node)
  {
    decls.push_back(node);
  }

  void accept(Visitor* visitor)
  {
    visitor->visit(this);
  }

public:
  std::vector<FileUnit*> decls;
};
