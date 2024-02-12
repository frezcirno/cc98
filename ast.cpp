#include "ast.hpp"

void IterationStatementFor::acceptChildren(Visitor* visitor)
{
  if (declaration)
    declaration->accept(visitor);
  if (init)
    init->accept(visitor);
  if (condition)
    condition->accept(visitor);
  if (update)
    update->accept(visitor);
  statement->accept(visitor);
}

void DirectAbstractDeclaratorFunction::acceptChildren(Visitor* visitor)
{
  abstractDeclarator->accept(visitor);

  if (parameterTypeList) {
    for (auto& parameterType : *parameterTypeList) {
      parameterType->accept(visitor);
    }
  }
}

void AbstractDeclarator::acceptChildren(Visitor* visitor)
{
  pointer->accept(visitor);
  directAbstractDeclarator->accept(visitor);
}

void StructDeclarator::acceptChildren(Visitor* visitor)
{
  declarator->accept(visitor);
  if (bitWidth)
    bitWidth->accept(visitor);
}

void GenericAssociation::acceptChildren(Visitor* visitor)
{
  if (typeName)
    typeName->accept(visitor);
  assignmentExpression->accept(visitor);
}

void AlignmentSpecifier::acceptChildren(Visitor* visitor)
{
  if (typeName) {
    typeName->accept(visitor);
  }
  if (constantExpression) {
    constantExpression->accept(visitor);
  }
}

void Declarator::acceptChildren(Visitor* visitor)
{

  if (pointer)
    pointer->accept(visitor);
  directDeclarator->accept(visitor);
}

void PostfixExpressionCastOp::acceptChildren(Visitor* visitor)
{
  typeName->accept(visitor);
  for (auto initializer : *initializerList)
    initializer->accept(visitor);
}

void CastExpression::acceptChildren(Visitor* visitor)
{
  typeName->accept(visitor);
  expr->accept(visitor);
}

void UnaryExpressionOnType::acceptChildren(Visitor* visitor)
{
  typeName->accept(visitor);
}

const char* Declarator::name() const
{
  return directDeclarator->name();
}

void DirectDeclaratorFunction::acceptChildren(Visitor* visitor)
{
  directDecl->accept(visitor);
  if (parameterList)
    parameterList->accept(visitor);
}