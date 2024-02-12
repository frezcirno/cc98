#include "ast.hpp"
#include "backend/backend.hpp"
#include "backend/x86.hpp"
#include "ir.hpp"
#include "parser.tab.hpp"
#include "type.hpp"
#include <cassert>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class Translator : public Visitor
{
public:
  void writeProgram(std::ostream& os)
  {
    writer.writeProgram(os);
  }


public:
  void visit(GenericAssociation* n) {}
  void visit(GenericSelection* n) {}

  Destination visitExpr(Identifier* n)
  {
    auto scope = scopes.top();
    auto sym = scope->find(n->name);
    if (!sym) {
      std::cerr << "error: undefined variable " << n->name << std::endl;
      exit(1);
    }
    return Destination(sym);
  }

  Destination visitExpr(ConstantI* n)
  {
    return Destination(n->eval());
  }

  Destination visitExpr(ConstantF* n)
  {
    return Destination(n->eval());
  }

  Destination visitExpr(EnumerationConstantUse* n)
  {
    return Destination(n->eval());
  }

  Destination visitExpr(StringLiteral* n)
  {
    return Destination(n->eval());
  }

  Destination visitExpr(FuncName* n)
  {
    return Destination(n->eval());
  }

  // primary-expression ::= ( expression )
  Destination visitExpr(ParenthesizedExpression* n)
  {
    return n->expr->acceptExprVisitor(this);
  }

  // comma-expression ::= assignment-expression , comma-expression
  Destination visitExpr(CommaExpression* n)
  {
    Destination dest;
    for (auto expr : n->expressionList) {
      dest = expr->acceptExprVisitor(this);
    }
    return dest;
  }

  // assignment-expression ::= unary-expression assignment-operator assignment-expression
  Destination visitExpr(AssignmentExpression* n)
  {
    auto lhs = n->lhs->acceptExprVisitor(this);
    auto rhs = n->rhs->acceptExprVisitor(this);
  }

  // conditional-expression ::= logical-or-expression ? expression : conditional-expression
  Destination visitExpr(ConditionalExpression* n) {}


  Destination visitExpr(PostfixExpressionIncOp* n) {}
  Destination visitExpr(PostfixExpressionDecOp* n) {}
  Destination visitExpr(PostfixExpressionCallOp* n) {}
  Destination visitExpr(PostfixExpressionDotOp* n) {}
  Destination visitExpr(PostfixExpressionPtrOp* n) {}
  Destination visitExpr(PostfixExpressionIndexOp* n) {}
  Destination visitExpr(PostfixExpressionCastOp* n) {}

  // arithmetic-expression ::= additive-expression arithmetic-operator arithmetic-expression
  Destination visitExpr(ArithmeticExpression* n) {}

  // cast-expression ::= ( type-name ) cast-expression
  Destination visitExpr(CastExpression* n) {}


  void visit(DesignatorField* n) {}
  void visit(DesignatorIndex* n) {}
  void visit(Designation* n) {}
  void visit(InitializerExpression* n) {}
  void visit(InitializerNested* n) {}
  void visit(StaticAssertDeclaration* n) {}
  void visit(Enumerator* n) {}
  void visit(TypeSpecifierBuiltin* n) {}
  void visit(Specifiers* n) {}
  void visit(TypeSpecifierAtomic* n) {}
  void visit(TypeSpecifierTypedefName* n) {}

  void visit(StructDeclarator* n)
  {
    throw std::runtime_error("not implemented");
  }

  void visit(StructDeclarationNormal* n) {}
  void visit(StructDeclarationStaticAssert* n) {}
  void visit(TypeSpecifierStructOrUnion* n) {}
  void visit(AlignmentSpecifier* n) {}
  void visit(DeclarationSpecifiers* n) {}

  void visit(Pointer* n)
  {
    throw std::runtime_error("not implemented");
  }

  // pointer ::= * type-qualifier-list? pointer?
  Type* visitPointer(Pointer* n, Type* typ_prefix)
  {
    Type* typ = typ_prefix->pointer_to();
    if (n->typeQuals) {
      for (auto tq : *n->typeQuals) {
        switch (tq->typeQualifier) {
          case TypeQualifier::CONST:
            if (typ->is_const)
              throw std::runtime_error("duplicated const qualifier");
            typ->is_const = true;
            break;
          case TypeQualifier::RESTRICT:
            if (typ->is_restrict)
              throw std::runtime_error("duplicated restrict qualifier");
            typ->is_restrict = true;
            break;
          case TypeQualifier::VOLATILE:
            if (typ->is_volatile)
              throw std::runtime_error("duplicated volatile qualifier");
            typ->is_volatile = true;
            break;
          case TypeQualifier::ATOMIC:
            if (typ->is_atomic)
              throw std::runtime_error("duplicated atomic qualifier");
            typ->is_atomic = true;
            break;
        }
      }
    }

    if (n->pointer) {
      return visitPointer(n->pointer, typ);
    }

    return typ;
  }

  void visit(UnaryExpressionExpr* n) {}
  void visit(UnaryExpressionOnType* n) {}
  void visit(TypeSpecifierEnum* n) {}

  void visit(LabeledStatementIdentifier* n) {}
  void visit(LabeledStatementCase* n) {}
  void visit(LabeledStatementDefault* n) {}

  Destination visitExpr(UnaryExpressionExpr* n) {}
  Destination visitExpr(UnaryExpressionOnType* n) {}

  // expression-statement ::= expression? ;
  void visit(ExpressionStatement* n)
  {
    if (n->expr) {
      n->expr->acceptExprVisitor(this);
    }
  }

  void visit(SelectionStatementIf* n) {}
  void visit(SelectionStatementSwitch* n) {}
  void visit(IterationStatementWhile* n) {}
  void visit(IterationStatementDoWhile* n) {}
  void visit(IterationStatementFor* n) {}
  void visit(JumpStatementContinue* n) {}
  void visit(JumpStatementBreak* n) {}
  void visit(JumpStatementGoto* n) {}
  void visit(JumpStatementReturn* n) {}

  void visit(Declarator* n)
  {
    throw std::runtime_error("not implemented");
  }

  // initializer ::= assignment-expression
  //              | { initializer-list ,? }
  // initializer-list ::= designation? initializer (, designation? initializer)*
  void visitInitializer(Initializer* n, Symbol* sym)
  {
    if (isa<InitializerExpression>(n)) {
      auto ia = cast<InitializerExpression>(n);
      auto val = ia->expr->eval();
      sym->val = val;
    } else if (isa<InitializerNested>(n)) {
      // auto in = cast<InitializerNested>(n);
      // for (auto init : *in->initializers) {
      //   visitInitializer(init, sym);
      // }
      throw std::runtime_error("not implemented");
    } else {
      assert(0);
    }
  }

  // declarator ::= pointer? direct-declarator
  // Declarator with optional pointer
  Symbol* visitDecl(Declarator* n, Type* typ_prefix)
  {
    Type* typ = typ_prefix;
    if (n->pointer) {
      typ = visitPointer(n->pointer, typ);
    }

    return visitDirectDecl(n->directDeclarator, typ);
  }

  void visit(AbstractDeclarator* n) {}

  // direct-declarator ::= identifier
  //                    | ( declarator )
  //                    | direct-declarator [ constant-expression? ]
  //                    | direct-declarator ( )
  //                    | direct-declarator ( parameter-type-list )
  //                    | direct-declarator ( identifier-list )
  Symbol* visitDirectDecl(DirectDeclarator* n, Type* typ)
  {
    if (isa<DirectDeclaratorIdentifier>(n)) {
      auto ddi = cast<DirectDeclaratorIdentifier>(n);
      auto sym = new Symbol(Symbol::VARIABLE, ddi->name());
      sym->ty = typ;
      return sym;
    } else if (isa<DirectDeclaratorParenthesized>(n)) {
      auto ddp = cast<DirectDeclaratorParenthesized>(n);
      return visitDecl(ddp->declarator, typ);
    } else if (isa<DirectDeclaratorArray>(n)) {
      auto dda = cast<DirectDeclaratorArray>(n);
      int arr_siz = -1;
      if (dda->expression) {
        assert(dda->expression->isConst());
        Value v = dda->expression->eval();
        assert(v.getType()->is_integer());
        arr_siz = v.getInt();
      }
      auto typ2 = typ->array_of(arr_siz);
      return visitDirectDecl(dda->directDecl, typ2);
    } else if (isa<DirectDeclaratorFunction>(n)) {
      auto ddf = cast<DirectDeclaratorFunction>(n);
      auto typ2 = typ->func_type();
      if (ddf->parameterList) {
        for (auto paramDecl : *ddf->parameterList) {
          // TODO
        }
      }
      if (ddf->identifierList) {
        for (auto id : *ddf->identifierList) {
          // TODO
        }
      }
      return visitDirectDecl(ddf->directDecl, typ2);
    } else {
      assert(0);
    }
  }

  void visit(DirectDeclaratorIdentifier* n) {}
  void visit(DirectDeclaratorParenthesized* n) {}
  void visit(DirectDeclaratorArray* n) {}
  void visit(DirectDeclaratorFunction* n) {}
  void visit(DirectAbstractDeclarator* n) {}
  void visit(DirectAbstractDeclaratorParenthesized* n) {}
  void visit(DirectAbstractDeclaratorArray* n) {}
  void visit(DirectAbstractDeclaratorFunction* n) {}
  void visit(ParameterDeclarationDecl* n) {}
  void visit(ParameterDeclarationAbstDecl* n) {}
  void visit(ParameterDeclarationAnonymous* n) {}
  void visit(ParameterList* n) {}

  void visit(InitDeclarator* n)
  {
    throw std::runtime_error("not implemented");
  }

  Type* parseEnum(TypeSpecifierEnum* n)
  {
    Type* typ = Type::enum_type();

    if (n->name) {
      typ->name = n->name;
    }

    if (n->enumerators) {
      for (auto enumerator : *n->enumerators) {}
    }

    return typ;
  }

  // struct-declaration ::= specifier-qualifier-list struct-declarator-list? ;
  //                     | static_assert-declaration
  // struct-declarator ::= declarator? : constant-expression
  //                     | declarator
  void parseStructMember(StructDeclaration* n, Type* typ)
  {
    if (isa<StructDeclarationStaticAssert>(n)) {
      auto sds = cast<StructDeclarationStaticAssert>(n);
      assert(0);
      return;
    }

    auto sdn = cast<StructDeclarationNormal>(n);
    auto mtyp = parseSpecs(sdn->specifiers);

    if (sdn->structDecls) {
      for (auto sd : *sdn->structDecls) {
        if (sd->bitWidth) {
          // TODO
        }
        auto sym = visitDecl(sd->declarator, mtyp);
        typ->add_member(sym);
      }
    }
  }

  // struct-or-union-specifier ::= struct-or-union identifier? { struct-declaration-list }
  //                             | struct-or-union identifier
  Type* parseStructUnion(TypeSpecifierStructOrUnion* n)
  {
    Type* typ = new Type();

    if (n->type == TypeSpecifierStructOrUnion::S_STRUCT)
      typ->kind = Type::TY_STRUCT;
    else
      typ->kind = Type::TY_UNION;

    if (n->name)
      typ->name = n->name;

    for (auto& m : *n->declarations)
      parseStructMember(m, typ);

    return typ;
  }

  // specifier-qualifier-list ::= type-specifier specifier-qualifier-list?
  //                            | type-qualifier specifier-qualifier-list?
  Type* parseSpecs(Specifiers* n)
  {
    return __parseSpecs(n->specs);
  }

  // declaration-specifiers ::= storage-class-specifier declaration-specifiers?
  //                          | type-specifier declaration-specifiers?
  //                          | type-qualifier declaration-specifiers?
  //                          | function-specifier declaration-specifiers?
  //                          | alignment-specifier declaration-specifiers?
  // type-specifier ::= void | char | short | int | long | float | double | signed | unsigned
  //                  | _Bool | _Complex | atomic-type-specifier | struct-or-union-specifier
  //                  | enum-specifier | typedef-name
  Type* parseDeclSpecs(DeclarationSpecifiers* n)
  {
    return __parseSpecs(n->specs);
  }

  Type* __parseSpecs(const std::vector<Node*>& specs)
  {
    int signedness = -1;

    // filter out typeSpecs
    std::vector<TypeSpecifier*> typeSpecs;
    std::vector<Node*> typeQuals;
    for (auto i : specs) {
      if (isa<TypeSpecifier>(i)) {
        if (isa<TypeSpecifierBuiltin>(i)) {
          auto tsb = cast<TypeSpecifierBuiltin>(i);
          if (tsb->type == TypeSpecifierBuiltin::SIGNED ||
              tsb->type == TypeSpecifierBuiltin::UNSIGNED) {
            if (signedness != -1) {
              std::cerr << "error: multiple signedness specifiers\n";
              exit(1);
            }

            if (tsb->type == TypeSpecifierBuiltin::SIGNED)
              signedness = 1;
            else   // TypeSpecifierBuiltin::UNSIGNED
              signedness = 0;
          }
          continue;
        }

        typeSpecs.push_back(cast<TypeSpecifier>(i));
      } else {
        typeQuals.push_back(i);
      }
    }

    Type* typ = &ty_int;
    for (auto ts : typeSpecs) {
      if (isa<TypeSpecifierBuiltin>(ts)) {
        auto tsb = cast<TypeSpecifierBuiltin>(ts);
        switch (tsb->type) {
          case TypeSpecifierBuiltin::VOID:
            typ = &ty_void;
            break;
          case TypeSpecifierBuiltin::BOOL:
            typ = &ty_bool;
            break;
          case TypeSpecifierBuiltin::CHAR:
            typ = &ty_char;
            break;
          case TypeSpecifierBuiltin::SHORT:
            typ = &ty_short;
            break;
          case TypeSpecifierBuiltin::INT:
            typ = &ty_int;
            break;
          case TypeSpecifierBuiltin::LONG:
            typ = &ty_long;
            break;
          case TypeSpecifierBuiltin::FLOAT:
            typ = &ty_float;
            break;
          case TypeSpecifierBuiltin::DOUBLE:
            typ = &ty_double;
            break;
          case TypeSpecifierBuiltin::COMPLEX:
          case TypeSpecifierBuiltin::IMAGINARY:
            // TODO
            assert(0);
            break;
          case TypeSpecifierBuiltin::SIGNED:
          case TypeSpecifierBuiltin::UNSIGNED:
            __builtin_unreachable();
        }
      } else if (isa<TypeSpecifierStructOrUnion>(ts)) {
        auto tss = cast<TypeSpecifierStructOrUnion>(ts);
        bool isStruct = tss->type == TypeSpecifierStructOrUnion::S_STRUCT;
        typ = parseStructUnion(tss);
      } else if (isa<TypeSpecifierEnum>(ts)) {
        auto tse = cast<TypeSpecifierEnum>(ts);
        typ = parseEnum(tse);
      } else if (isa<TypeSpecifierTypedefName>(ts)) {
        auto tst = cast<TypeSpecifierTypedefName>(ts);
        // TODO
        assert(false);
      } else {   // TypeSpecifierAtomic
        auto tsa = cast<TypeSpecifierAtomic>(ts);
        // TODO
        assert(false);
      }
    }

    for (auto i : typeQuals) {
      if (isa<TypeQualifier>(i)) {
        auto tq = cast<TypeQualifier>(i);
        switch (tq->typeQualifier) {
          case TypeQualifier::CONST:
            typ->is_const = true;
            break;
          case TypeQualifier::RESTRICT:
            typ->is_restrict = true;
            break;
          case TypeQualifier::VOLATILE:
            typ->is_volatile = true;
            break;
          case TypeQualifier::ATOMIC:
            typ->is_atomic = true;
            break;
        }
      } else {   // FunctionSpecifier
        auto fs = cast<FunctionSpecifier>(i);
        assert(false);
      }
    }

    if (signedness != -1) {
      if (typ->is_flonum()) {
        std::cerr << "error: signedness specifier for floating point type\n";
        exit(1);
      }

      if (signedness == 0) {
        typ->is_signed = true;
      } else if (signedness == 1) {
        typ->is_signed = false;
      }
    }

    return typ;
  }

  // declaration ::= declaration-specifiers init-declarator-list? ;
  // init-declarator-list ::= init-declarator (, init-declarator)*
  // init-declarator ::= declarator (= initializer)?
  void visit(DeclarationNormal* n)
  {
    // DeclarationSpecifiers: int
    // DeclarationSpecifiers: enum X { A, B, C }
    // DeclarationSpecifiers: struct X { int A, float B, char C }
    auto typ = parseDeclSpecs(n->declSpecs);

    if (n->initDecls) {
      // declare a variable/function
      for (auto initDecl : *n->initDecls) {
        // init-declarator ::= declarator (= initializer)?
        // Declarator: a
        // Declarator: add(int,int)
        // Declarator: a[10]
        Symbol* sym = visitDecl(initDecl->declarator, typ);

        // Initializer: 0
        // Initializer: { 1, 2, 3 }
        // Initializer: { [0]={.1=1,[2]=2}, .1=1, .2=2 }
        if (initDecl->initializer) {
          // only variable can have initializer
          if (sym->ty->is_function())
            throw std::runtime_error("function cannot have initializer");

          visitInitializer(initDecl->initializer, sym);
        }

        scopes.top()->add(sym);
      }
    } else if (typ->require_define()) {
      // declare a struct/union/enum type
      Symbol* sym = new Symbol(Symbol::TYPEDEF, typ->name);
      sym->ty = typ;
      scopes.top()->add(sym);
    } else {
      // error: no initializer
      throw std::runtime_error("no initializer");
    }
  }

  void visit(DeclarationStaticAssert* n) {}

  void visit(TypeName* n) {}

  void visit(CompoundStatement* n)
  {
    auto scope = new Scope(scopes.top());
    scopes.push(scope);
    n->acceptChildren(this);
    scopes.pop();
  }

  void visit(FunctionDefinition* n)
  {
    Type* retType = parseDeclSpecs(n->declSpecs);
    auto sym = new Symbol(Symbol::FUNCTION, n->declarator->name());

    if (n->declarator) {
      visitDecl(n->declarator, retType);
    } else {
      sym->ty = retType;
    }

    scopes.top()->add(sym);

    n->compoundStatement->accept(this);
  }

  void visit(TranslationUnit* n)
  {
    scopes.push(new Scope(nullptr));
    n->acceptChildren(this);
  }

private:
  int tempCounter = 0;
  int labelCounter = 0;
  std::stack<Scope*> scopes;
  X86Writer writer;
};

int main(int argc, char** argv)
{
  int res;
  Node* root;

  if (argc <= 1)
    return 1;

  res = parse(argv[1], &root);
  if (res == 0) {
    Translator trans;
    root->accept(&trans);
    trans.writeProgram(std::cout);
  }

  return res;
}
