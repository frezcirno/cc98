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
      printf("|   ");

    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
  }

  void visit(GenericAssociation* n) override
  {
    printIndent("GenericAssociation\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(GenericSelection* n) override
  {
    printIndent("GenericSelection\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(Identifier* n) override
  {
    printIndent("Identifier %s\n", n->name);
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(ConstantI* n) override
  {
    printIndent("ConstantI %d\n", n->val);
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(ConstantF* n) override
  {
    printIndent("ConstantF %f\n", n->val);
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(EnumerationConstantUse* n) override
  {
    printIndent("EnumerationConstantUse %s\n", n->_enum);
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(StringLiteral* n) override
  {
    printIndent("StringLiteral %s\n", n->val);
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(FuncName* n) override
  {
    printIndent("FuncName __func__\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(ParenthesizedExpression* n) override
  {
    printIndent("ParenthesizedExpression\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(UnaryExpressionExpr* n) override
  {
    printIndent("UnaryExpressionExpr\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(UnaryExpressionOnType* n) override
  {
    printIndent("UnaryExpressionOnType\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(CommaExpression* n) override
  {
    printIndent("CommaExpression\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(AssignmentExpression* n) override
  {
    printIndent("AssignmentExpression\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(ConditionalExpression* n) override
  {
    printIndent("ConditionalExpression\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(PostfixExpressionIncOp* n) override
  {
    printIndent("PostfixExpressionIncOp\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(PostfixExpressionDecOp* n) override
  {
    printIndent("PostfixExpressionDecOp\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(PostfixExpressionCallOp* n) override
  {
    printIndent("PostfixExpressionCallOp\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(PostfixExpressionDotOp* n) override
  {
    printIndent("PostfixExpressionDotOp\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(PostfixExpressionPtrOp* n) override
  {
    printIndent("PostfixExpressionPtrOp\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(PostfixExpressionIndexOp* n) override
  {
    printIndent("PostfixExpressionIndexOp\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(PostfixExpressionCastOp* n) override
  {
    printIndent("PostfixExpressionCastOp\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(ArithmeticExpression* n) override
  {
    printIndent("ArithmeticExpression %s\n", ArithmeticExpression::to_string(n->op));
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(CastExpression* n) override
  {
    printIndent("CastExpression\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(DesignatorField* n) override
  {
    printIndent("DesignatorField\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(DesignatorIndex* n) override
  {
    printIndent("DesignatorIndex\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(Designation* n) override
  {
    printIndent("Designation\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(InitializerExpression* n) override
  {
    printIndent("InitializerExpression\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(InitializerNested* n) override
  {
    printIndent("InitializerNested\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(StaticAssertDeclaration* n) override
  {
    printIndent("StaticAssertDeclaration\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(Enumerator* n) override
  {
    printIndent("Enumerator\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }

  void visit(TypeSpecifierBuiltin* n) override
  {
    printIndent("TypeSpecifierBuiltin %s\n", n->typeName());
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(Specifiers* n) override
  {
    printIndent("Specifiers\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(TypeSpecifierAtomic* n) override
  {
    printIndent("TypeSpecifierAtomic\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(TypeSpecifierTypedefName* n) override
  {
    printIndent("TypeSpecifierTypedefName %s\n", n->name);
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(StructDeclarator* n) override
  {
    printIndent("StructDeclarator\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(StructDeclarationNormal* n) override
  {
    printIndent("StructDeclarationNormal\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(StructDeclarationStaticAssert* n) override
  {
    printIndent("StructDeclarationStaticAssert\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(TypeSpecifierStructOrUnion* n) override
  {
    printIndent(
      "TypeSpecifierStructOrUnion %s\n",
      n->type == TypeSpecifierStructOrUnion::StructOrUnion::S_STRUCT ? "struct" : "union");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(AlignmentSpecifier* n) override
  {
    printIndent("AlignmentSpecifier\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(DeclarationSpecifiers* n) override
  {
    printIndent("DeclarationSpecifiers\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(Pointer* n) override
  {
    printIndent("Pointer\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(TypeSpecifierEnum* n) override
  {
    printIndent("TypeSpecifierEnum %s\n", n->name);
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(LabeledStatementIdentifier* n) override
  {
    printIndent("LabeledStatementIdentifier %s\n", n->identifier);
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(LabeledStatementCase* n) override
  {
    printIndent("LabeledStatementCase\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(LabeledStatementDefault* n) override
  {
    printIndent("LabeledStatementDefault default\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(ExpressionStatement* n) override
  {
    printIndent("ExpressionStatement\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(SelectionStatementIf* n) override
  {
    printIndent("SelectionStatementIf if\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(SelectionStatementSwitch* n) override
  {
    printIndent("SelectionStatementSwitch switch\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(IterationStatementWhile* n) override
  {
    printIndent("IterationStatementWhile while\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(IterationStatementDoWhile* n) override
  {
    printIndent("IterationStatementDoWhile do\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(IterationStatementFor* n) override
  {
    printIndent("IterationStatementFor for\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(JumpStatementContinue* n) override
  {
    printIndent("JumpStatementContinue continue\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(JumpStatementBreak* n) override
  {
    printIndent("JumpStatementBreak break\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(JumpStatementGoto* n) override
  {
    printIndent("JumpStatementGoto goto %s\n", n->identifier);
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(JumpStatementReturn* n) override
  {
    printIndent("JumpStatementReturn return\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(Declarator* n) override
  {
    printIndent("Declarator\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(AbstractDeclarator* n) override
  {
    printIndent("AbstractDeclarator\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(DirectDeclaratorIdentifier* n) override
  {
    printIndent("DirectDeclaratorIdentifier %s\n", n->identifier);
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(DirectDeclaratorParenthesized* n) override
  {
    printIndent("DirectDeclaratorParenthesized\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(DirectDeclaratorArray* n) override
  {
    printIndent("DirectDeclaratorArray [\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(DirectDeclaratorFunction* n) override
  {
    printIndent("DirectDeclaratorFunction\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(DirectAbstractDeclaratorParenthesized* n) override
  {
    printIndent("DirectAbstractDeclaratorParenthesized\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(DirectAbstractDeclaratorArray* n) override
  {
    printIndent("DirectAbstractDeclaratorArray\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(DirectAbstractDeclaratorFunction* n) override
  {
    printIndent("DirectAbstractDeclaratorFunction\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(ParameterDeclarationDecl* n) override
  {
    printIndent("ParameterDeclarationDecl\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(ParameterDeclarationAbstDecl* n) override
  {
    printIndent("ParameterDeclarationAbstDecl\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(ParameterDeclarationAnonymous* n) override
  {
    printIndent("ParameterDeclarationAnonymous\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(ParameterList* n) override
  {
    printIndent("ParameterList\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(InitDeclarator* n) override
  {
    printIndent("InitDeclarator\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(DeclarationNormal* n) override
  {
    printIndent("DeclarationNormal\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(DeclarationStaticAssert* n) override
  {
    printIndent("DeclarationStaticAssert\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(TypeName* n) override
  {
    printIndent("TypeName\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }
  void visit(CompoundStatement* n) override
  {
    printIndent("CompoundStatement {\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
    printIndent("CompoundStatement }\n");
  }
  void visit(FunctionDefinition* n) override
  {
    printIndent("FunctionDefinition\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }

  void visit(TranslationUnit* n) override
  {
    printIndent("TranslationUnit\n");
    indentLevel++;
    n->acceptChildren(this);
    indentLevel--;
  }

private:
  int indentLevel;
};
