#pragma once
#include "ast.hpp"
#include <cstdarg>

class NodePrinter : public Visitor
{
public:
  NodePrinter()
    : indentLevel(0)
  {}

  void printIndent(const char* str, ...)
  {
    for (int i = 0; i < indentLevel; i++)
      printf("| ");

    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
  }

  void visit(GenericAssociation* n) override
  {
    printIndent("GenericAssociation\n");
    indentLevel++;

    n->typeName->accept(this);
    n->expr->accept(this);

    indentLevel--;
  }
  void visit(GenericSelection* n) override
  {
    printIndent("GenericSelection\n");
    indentLevel++;
    n->expr->accept(this);
    if (n->associations)
      for (auto assoc : *n->associations)
        assoc->accept(this);
    indentLevel--;
  }
  void visit(Identifier* n) override
  {
    printIndent("Identifier %s\n", n->name);
  }
  void visit(ConstantI* n) override
  {
    printIndent("ConstantI %d\n", n->val);
  }
  void visit(ConstantF* n) override
  {
    printIndent("ConstantF %f\n", n->val);
  }
  void visit(EnumerationConstantUse* n) override
  {
    printIndent("EnumerationConstantUse %s\n", n->_enum);
  }
  void visit(StringLiteral* n) override
  {
    printIndent("StringLiteral %s\n", n->val);
  }
  void visit(FuncName* n) override
  {
    printIndent("FuncName __func__\n");
  }
  void visit(ParenthesizedExpression* n) override
  {
    printIndent("ParenthesizedExpression\n");
    indentLevel++;
    n->expr->accept(this);
    indentLevel--;
  }
  void visit(UnaryExpressionOnExpr* n) override
  {
    printIndent("UnaryExpressionOnExpr\n");
    indentLevel++;
    n->expr->accept(this);
    indentLevel--;
  }
  void visit(UnaryExpressionOnType* n) override
  {
    printIndent("UnaryExpressionOnType\n");
    indentLevel++;
    n->typeName->accept(this);
    indentLevel--;
  }
  void visit(CommaExpression* n) override
  {
    printIndent("CommaExpression\n");
    indentLevel++;
    for (auto expr : n->exprs)
      expr->accept(this);
    indentLevel--;
  }
  void visit(AssignmentExpression* n) override
  {
    printIndent("AssignmentExpression\n");
    indentLevel++;
    n->lhs->accept(this);
    n->rhs->accept(this);
    indentLevel--;
  }
  void visit(ConditionalExpression* n) override
  {
    printIndent("ConditionalExpression\n");
    indentLevel++;
    n->cond->accept(this);
    n->then->accept(this);
    n->els->accept(this);
    indentLevel--;
  }
  void visit(PostfixExpressionIncOp* n) override
  {
    printIndent("PostfixExpressionIncOp\n");
    indentLevel++;
    n->expr->accept(this);
    indentLevel--;
  }
  void visit(PostfixExpressionDecOp* n) override
  {
    printIndent("PostfixExpressionDecOp\n");
    indentLevel++;
    n->expr->accept(this);
    indentLevel--;
  }
  void visit(PostfixExpressionCallOp* n) override
  {
    printIndent("PostfixExpressionCallOp\n");
    indentLevel++;
    n->expr->accept(this);
    if (n->args) {
      for (auto arg : *n->args)
        arg->accept(this);
    }
    indentLevel--;
  }
  void visit(PostfixExpressionDotOp* n) override
  {
    printIndent("PostfixExpressionDotOp %s\n", n->field);
    indentLevel++;
    n->expr->accept(this);
    indentLevel--;
  }
  void visit(PostfixExpressionPtrOp* n) override
  {
    printIndent("PostfixExpressionPtrOp %s\n", n->field);
    indentLevel++;
    n->expr->accept(this);
    indentLevel--;
  }
  void visit(PostfixExpressionIndexOp* n) override
  {
    printIndent("PostfixExpressionIndexOp\n");
    indentLevel++;
    n->expr->accept(this);
    n->index->accept(this);
    indentLevel--;
  }
  void visit(PostfixExpressionCastOp* n) override
  {
    printIndent("PostfixExpressionCastOp\n");
    indentLevel++;
    n->typeName->accept(this);
    if (n->initializerList) {
      for (auto init : *n->initializerList)
        init->accept(this);
    }
    indentLevel--;
  }
  void visit(ArithmeticExpression* n) override
  {
    printIndent("ArithmeticExpression %s\n", ArithmeticExpression::to_string(n->op));
    indentLevel++;
    n->lhs->accept(this);
    n->rhs->accept(this);
    indentLevel--;
  }
  void visit(CastExpression* n) override
  {
    printIndent("CastExpression\n");
    indentLevel++;
    n->typeName->accept(this);
    n->expr->accept(this);
    indentLevel--;
  }
  void visit(FieldDesignator* n) override
  {
    printIndent("FieldDesignator %s\n", n->field);
  }
  void visit(IndexDesignator* n) override
  {
    printIndent("IndexDesignator %d\n", n->index);
  }
  void visit(Designation* n) override
  {
    printIndent("Designation\n");
    indentLevel++;
    if (n->designators) {
      for (auto desig : *n->designators)
        desig->accept(this);
    }
    indentLevel--;
  }
  void visit(ExpressionInitializer* n) override
  {
    printIndent("ExpressionInitializer\n");
    indentLevel++;
    n->expr->accept(this);
    indentLevel--;
  }
  void visit(NestedInitializer* n) override
  {
    printIndent("NestedInitializer\n");
    indentLevel++;
    if (n->initializers) {
      for (auto init : *n->initializers)
        init->accept(this);
    }
    indentLevel--;
  }
  void visit(StaticAssert* n) override
  {
    printIndent("StaticAssert\n");
    indentLevel++;
    n->expr->accept(this);
    indentLevel--;
  }
  void visit(Enumerator* n) override
  {
    printIndent("Enumerator\n");
    indentLevel++;
    n->identifier->accept(this);
    if (n->expr)
      n->expr->accept(this);
    indentLevel--;
  }

  void visit(BuiltinTypeSpecifier* n) override
  {
    printIndent("BuiltinTypeSpecifier %s\n", n->typeName());
  }
  void visit(SpecifierQualifierList* n) override
  {
    printIndent("SpecifierQualifierList\n");
    indentLevel++;
    for (auto spec : n->specs)
      spec->accept(this);
    indentLevel--;
  }
  void visit(AtomicTypeSpecifier* n) override
  {
    printIndent("AtomicTypeSpecifier\n");
    indentLevel++;
    n->typeName->accept(this);
    indentLevel--;
  }
  void visit(TypedefTypeSpecifier* n) override
  {
    printIndent("TypedefTypeSpecifier %s\n", n->name);
  }
  void visit(StructDeclarator* n) override
  {
    printIndent("StructDeclarator\n");
    indentLevel++;
    if (n->decl)
      n->decl->accept(this);
    indentLevel--;
  }
  void visit(NormalStructDeclaration* n) override
  {
    printIndent("NormalStructDeclaration\n");
    indentLevel++;
    n->declSpecs->accept(this);
    if (n->declarators) {
      for (auto decl : *n->declarators)
        decl->accept(this);
    }
    indentLevel--;
  }
  void visit(StructUnionTypeSpecifier* n) override
  {
    printIndent(
      "StructUnionTypeSpecifier %s %s\n",
      n->kind == StructUnionTypeSpecifier::StructOrUnionKind::S_STRUCT ? "struct" : "union",
      n->name);
    indentLevel++;
    if (n->declarations) {
      for (auto decl : *n->declarations)
        decl->accept(this);
    }
    indentLevel--;
  }
  void visit(AlignmentSpecifier* n) override
  {
    printIndent("AlignmentSpecifier\n");
    indentLevel++;
    n->typeName->accept(this);
    indentLevel--;
  }
  void visit(DeclarationSpecifierList* n) override
  {
    printIndent("DeclarationSpecifierList\n");
    indentLevel++;
    for (auto spec : n->specs)
      spec->accept(this);
    indentLevel--;
  }
  void visit(Pointer* n) override
  {
    printIndent("Pointer\n");
    indentLevel++;
    if (n->typeQuals) {
      for (auto qual : *n->typeQuals)
        qual->accept(this);
    }
    if (n->pointer)
      n->pointer->accept(this);
    indentLevel--;
  }
  void visit(EnumTypeSpecifier* n) override
  {
    printIndent("EnumTypeSpecifier %s\n", n->name);
    indentLevel++;
    if (n->enumerators) {
      for (auto en : *n->enumerators)
        en->accept(this);
    }
    indentLevel--;
  }
  void visit(LabeledStatement* n) override
  {
    printIndent("LabeledStatement %s\n", n->label);
    indentLevel++;
    n->stmt->accept(this);
    indentLevel--;
  }
  void visit(CaseStatement* n) override
  {
    printIndent("CaseStatement\n");
    indentLevel++;
    n->expr->accept(this);
    n->stmt->accept(this);
    indentLevel--;
  }
  void visit(DefaultStatement* n) override
  {
    printIndent("DefaultStatement default\n");
    indentLevel++;
    n->stmt->accept(this);
    indentLevel--;
  }
  void visit(ExpressionStatement* n) override
  {
    printIndent("ExpressionStatement\n");
    indentLevel++;
    n->expr->accept(this);
    indentLevel--;
  }
  void visit(IfStatement* n) override
  {
    printIndent("IfStatement\n");
    indentLevel++;
    n->cond->accept(this);
    n->then->accept(this);
    if (n->els)
      n->els->accept(this);
    indentLevel--;
  }
  void visit(SwitchStatement* n) override
  {
    printIndent("SwitchStatement\n");
    indentLevel++;
    n->cond->accept(this);
    n->body->accept(this);
    indentLevel--;
  }
  void visit(WhileStatement* n) override
  {
    printIndent("WhileStatement\n");
    indentLevel++;
    n->cond->accept(this);
    n->body->accept(this);
    indentLevel--;
  }
  void visit(DoWhileStatement* n) override
  {
    printIndent("DoWhileStatement\n");
    indentLevel++;
    n->cond->accept(this);
    n->body->accept(this);
    indentLevel--;
  }
  void visit(ForStatement* n) override
  {
    printIndent("ForStatement\n");
    indentLevel++;
    n->decl->accept(this);
    n->init->accept(this);
    n->cond->accept(this);
    n->inc->accept(this);
    n->body->accept(this);
    indentLevel--;
  }
  void visit(ContinueStatement* n) override
  {
    printIndent("ContinueStatement\n");
  }
  void visit(BreakStatement* n) override
  {
    printIndent("BreakStatement\n");
  }
  void visit(GotoStatement* n) override
  {
    printIndent("GotoStatement %s\n", n->label);
  }
  void visit(ReturnStatement* n) override
  {
    printIndent("ReturnStatement\n");
  }
  void visit(Declarator* n) override
  {
    printIndent("Declarator\n");
    indentLevel++;
    if (n->pointer)
      n->pointer->accept(this);
    n->decl->accept(this);
    indentLevel--;
  }
  void visit(AbstractDeclarator* n) override
  {
    printIndent("AbstractDeclarator\n");
    indentLevel++;
    if (n->pointer)
      n->pointer->accept(this);
    if (n->decl)
      n->decl->accept(this);
    indentLevel--;
  }
  void visit(IdentifierDeclarator* n) override
  {
    printIndent("IdentifierDeclarator %s\n", n->identifier);
  }
  void visit(ParenthesizedDeclarator* n) override
  {
    printIndent("ParenthesizedDeclarator\n");
    indentLevel++;
    n->decl->accept(this);
    indentLevel--;
  }
  void visit(ArrayDeclarator* n) override
  {
    printIndent("ArrayDeclarator [\n");
    indentLevel++;
    n->decl->accept(this);
    if (n->expr)
      n->expr->accept(this);
    indentLevel--;
    printIndent("]\n");
  }
  void visit(FunctionDeclarator* n) override
  {
    printIndent("FunctionDeclarator\n");
    indentLevel++;
    n->decl->accept(this);
    if (n->params)
      n->params->accept(this);
    indentLevel--;
  }
  void visit(AbstractParenthesizedDeclarator* n) override
  {
    printIndent("AbstractParenthesizedDeclarator\n");
    indentLevel++;
    n->decl->accept(this);
    indentLevel--;
  }
  void visit(AbstractArrayDeclarator* n) override
  {
    printIndent("AbstractArrayDeclarator\n");
    indentLevel++;
    n->decl->accept(this);
    if (n->expr)
      n->expr->accept(this);
    indentLevel--;
  }
  void visit(AbstractFunctionDeclarator* n) override
  {
    printIndent("AbstractFunctionDeclarator\n");
    indentLevel++;
    n->decl->accept(this);
    if (n->params)
      n->params->accept(this);
    indentLevel--;
  }
  void visit(ParameterDeclaration* n) override
  {
    printIndent("ParameterDeclaration\n");
    indentLevel++;
    n->declSpecs->accept(this);
    n->decl->accept(this);
    indentLevel--;
  }
  void visit(AbstractParameterDeclaration* n) override
  {
    printIndent("AbstractParameterDeclaration\n");
    indentLevel++;
    n->declSpecs->accept(this);
    n->decl->accept(this);
    indentLevel--;
  }
  void visit(AnonymousParameterDeclaration* n) override
  {
    printIndent("AnonymousParameterDeclaration\n");
    indentLevel++;
    n->declSpecs->accept(this);
    indentLevel--;
  }
  void visit(ParameterList* n) override
  {
    printIndent("ParameterList\n");
    indentLevel++;
    for (auto param : n->params)
      param->accept(this);
    indentLevel--;
  }
  void visit(InitDeclarator* n) override
  {
    printIndent("InitDeclarator\n");
    indentLevel++;
    n->decl->accept(this);
    if (n->initializer)
      n->initializer->accept(this);
    indentLevel--;
  }
  void visit(NormalDeclaration* n) override
  {
    printIndent("NormalDeclaration\n");
    indentLevel++;
    n->declSpecs->accept(this);
    if (n->initDecls) {
      for (auto initDecl : *n->initDecls)
        initDecl->accept(this);
    }
    indentLevel--;
  }
  void visit(TypeName* n) override
  {
    printIndent("TypeName\n");
    indentLevel++;
    n->declSpecs->accept(this);
    if (n->abstractDeclarator)
      n->abstractDeclarator->accept(this);
    indentLevel--;
  }
  void visit(CompoundStatement* n) override
  {
    printIndent("CompoundStatement {\n");
    indentLevel++;
    for (auto item : n->blockItems)
      item->accept(this);
    indentLevel--;
    printIndent("}\n");
  }
  void visit(FunctionDefinition* n) override
  {
    printIndent("FunctionDefinition\n");
    indentLevel++;
    if (n->declSpecs)
      n->declSpecs->accept(this);
    n->decl->accept(this);
    if (n->oldStyleParams) {
      for (auto param : *n->oldStyleParams)
        param->accept(this);
    }
    n->body->accept(this);
    indentLevel--;
  }

  void visit(TranslationUnit* n) override
  {
    printIndent("TranslationUnit\n");
    indentLevel++;
    for (auto decl : n->decls)
      decl->accept(this);
    indentLevel--;
  }

private:
  int indentLevel;
};
