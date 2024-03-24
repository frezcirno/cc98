#pragma once
#include "ast.hpp"
#include "x86.hpp"
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
  Translator(AsmHelper* asmHelper)
    : out(asmHelper)
  {}

public:
  void writeFunction(std::ostream& os, Symbol* f)
  {
    auto scope = scopes.getCurrent();
    auto func = f->getType();
    auto params = &func->params;
    // auto body = func->getFunctionBody();

    // prologue
    os << "  pushl %ebp\n";
    os << "  movl %esp, %ebp\n";
    os << "  subl $" << func->getSize() << ", %esp\n";

    // function body
    // body->accept(this);

    // epilogue
    os << "  leave\n";
    os << "  ret\n";
  }

public:
  // arithmetic-expression ::= additive-expression arithmetic-operator arithmetic-expression
  Value* visitExpr(ArithmeticExpression* n)
  {
    auto lhs = n->lhs->acceptExpr(this);
    auto rhs = n->rhs->acceptExpr(this);
    switch (n->op) {
      case ArithmeticExpression::A_ADD_OP:
        return out->writeAdd(lhs, rhs);
      case ArithmeticExpression::A_SUB_OP:
        return out->writeSub(lhs, rhs);
      case ArithmeticExpression::A_MUL_OP:
        return out->writeMul(lhs, rhs);
      case ArithmeticExpression::A_DIV_OP:
        return out->writeDiv(lhs, rhs);
      case ArithmeticExpression::A_MOD_OP:
        return out->writeMod(lhs, rhs);
      case ArithmeticExpression::A_LEFT_OP:
        return out->writeShl(lhs, rhs);
      case ArithmeticExpression::A_RIGHT_OP:
        return out->writeShr(lhs, rhs);
      case ArithmeticExpression::R_LT_OP:
        return out->writeCmpLt(lhs, rhs);
      case ArithmeticExpression::R_GT_OP:
        return out->writeCmpGt(lhs, rhs);
      case ArithmeticExpression::R_LE_OP:
        return out->writeCmpLe(lhs, rhs);
      case ArithmeticExpression::R_GE_OP:
        return out->writeCmpGe(lhs, rhs);
      case ArithmeticExpression::E_EQ_OP:
        return out->writeCmpEq(lhs, rhs);
      case ArithmeticExpression::E_NE_OP:
        return out->writeCmpNe(lhs, rhs);
      case ArithmeticExpression::B_AND_OP:
        return out->writeAnd(lhs, rhs);
      case ArithmeticExpression::B_XOR_OP:
        return out->writeXor(lhs, rhs);
      case ArithmeticExpression::B_OR_OP:
        return out->writeOr(lhs, rhs);
      case ArithmeticExpression::L_AND_OP:
        return out->writeAnd(lhs, rhs);
      case ArithmeticExpression::L_OR_OP:
        return out->writeOr(lhs, rhs);
    }
  }

  // assignment-expression ::= unary-expression assignment-operator assignment-expression
  Value* visitExpr(AssignmentExpression* n)
  {
    Value* lhs = n->lhs->acceptExpr(this);
    Value* rhs = n->rhs->acceptExpr(this);
    Value* res;
    switch (n->op) {
      case AssignmentExpression::ASSIGN:
        return out->writeAssign(lhs, rhs);
      case AssignmentExpression::MUL_ASSIGN:
        res = out->writeMul(lhs, rhs);
        return out->writeAssign(lhs, res);
      case AssignmentExpression::DIV_ASSIGN:
        res = out->writeDiv(lhs, rhs);
        return out->writeAssign(lhs, res);
      case AssignmentExpression::MOD_ASSIGN:
        res = out->writeMod(lhs, rhs);
        return out->writeAssign(lhs, res);
      case AssignmentExpression::ADD_ASSIGN:
        res = out->writeAdd(lhs, rhs);
        return out->writeAssign(lhs, res);
      case AssignmentExpression::SUB_ASSIGN:
        res = out->writeSub(lhs, rhs);
        return out->writeAssign(lhs, res);
      case AssignmentExpression::LEFT_ASSIGN:
        res = out->writeShl(lhs, rhs);
        return out->writeAssign(lhs, res);
      case AssignmentExpression::RIGHT_ASSIGN:
        res = out->writeShr(lhs, rhs);
        return out->writeAssign(lhs, res);
      case AssignmentExpression::AND_ASSIGN:
        res = out->writeAnd(lhs, rhs);
        return out->writeAssign(lhs, res);
      case AssignmentExpression::XOR_ASSIGN:
        res = out->writeXor(lhs, rhs);
        return out->writeAssign(lhs, res);
      case AssignmentExpression::OR_ASSIGN:
        res = out->writeOr(lhs, rhs);
        return out->writeAssign(lhs, res);
    }
  }

  Value* visitExpr(ConstantI* n)
  {
    return new Value(n->evalConst(scopes));
  }

  Value* visitExpr(ConstantF* n)
  {
    return new Value(n->evalConst(scopes));
  }

  // cast-expression ::= ( type-name ) cast-expression
  Value* visitExpr(CastExpression* n)
  {
    // TODO: implement cast
    return n->expr->acceptExpr(this);
  }

  // comma-expression ::= assignment-expression , comma-expression
  Value* visitExpr(CommaExpression* n)
  {
    Value* dest;
    for (auto expr : n->exprs)
      dest = expr->acceptExpr(this);
    return dest;
  }

  // conditional-expression ::= logical-or-expression ? expression : conditional-expression
  Value* visitExpr(ConditionalExpression* n)
  {
    std::string elseLabel = "else" + std::to_string(labelCounter);
    std::string endLabel = "end" + std::to_string(labelCounter++);
    Value* dest = new Value(out->allocateRegister());

    Value* cond = n->cond->acceptExpr(this);
    out->writeJumpIfZero(cond, elseLabel.c_str());

    Value* then = n->then->acceptExpr(this);
    out->writeAssign(dest, then);
    out->writeJump(endLabel.c_str());

    out->writeLabel(elseLabel.c_str());
    Value* els = n->els->acceptExpr(this);
    out->writeAssign(dest, els);

    out->writeLabel(endLabel.c_str());
    return dest;
  }

  Value* visitExpr(EnumerationConstantUse* n)
  {
    return new Value(n->evalConst(scopes));
  }

  Value* visitExpr(FuncName* n)
  {
    return new Value(n->evalConst(scopes));
  }

  // primary-expression ::= ( expression )
  Value* visitExpr(ParenthesizedExpression* n)
  {
    return n->expr->acceptExpr(this);
  }

  Value* visitExpr(PostfixExpressionIncOp* n)
  {
    auto dest = n->expr->acceptExpr(this);
    if (!dest->isLValue())
      throw std::runtime_error("invalid lvalue in increment");
    // TODO: defer increment
    return dest;
  }

  Value* visitExpr(PostfixExpressionDecOp* n)
  {
    auto dest = n->expr->acceptExpr(this);
    out->writeDec(dest);
    return dest;
  }

  Value* visitExpr(PostfixExpressionCallOp* n)
  {
    auto fn = n->expr->acceptExpr(this);
    std::vector<Value*> args;
    for (auto arg : *n->args)
      args.push_back(arg->acceptExpr(this));
    return out->writeCall(fn, args);
  }

  Value* visitExpr(PostfixExpressionDotOp* n)
  {
    auto dest = n->expr->acceptExpr(this);
    // TODO: handle struct member access
    // out->writeOffset(dest, n->field);
    return dest;
  }

  Value* visitExpr(PostfixExpressionPtrOp* n)
  {
    auto dest = n->expr->acceptExpr(this);
    // TODO: handle struct member access
    return dest;
  }

  Value* visitExpr(PostfixExpressionIndexOp* n)
  {
    auto dest = n->expr->acceptExpr(this);
    auto index = n->index->acceptExpr(this);
    return out->writeLoad(out->writeAdd(dest, index));
  }

  // postfix-expression ::= primary-expression
  //                      | postfix-expression [ expression ]
  //                      | postfix-expression ( argument-expression-list? )
  //                      | postfix-expression . identifier
  //                      | postfix-expression -> identifier
  //                      | postfix-expression ++|--
  //                      | ( type-name ) { initializer-list }
  Value* visitExpr(PostfixExpressionCastOp* n)
  {
    // TODO: implement cast
    throw std::runtime_error("not implemented");
  }

  // unary-expression-on-expr ::= postfix-expression
  //                    | ++| -- unary-expression
  //                    | unary-operator cast-expression
  //                    | sizeof unary-expression
  Value* visitExpr(UnaryExpressionOnExpr* n)
  {
    Value* val = nullptr;
    switch (n->op) {
      case UnaryExpressionOnExpr::U_INC_OP:
        val = n->expr->acceptExpr(this);
        out->writeInc(val);
        break;
      case UnaryExpressionOnExpr::U_DEC_OP:
        val = n->expr->acceptExpr(this);
        out->writeDec(val);
        break;
      case UnaryExpressionOnExpr::U_ADDR_OP:
        val = n->expr->acceptExpr(this);
        val = out->getAddress(val);
        break;
      case UnaryExpressionOnExpr::U_DEREF_OP:
        val = n->expr->acceptExpr(this);
        val = out->writeLoad(val);
        break;
      case UnaryExpressionOnExpr::U_POS_OP:
        val = n->expr->acceptExpr(this);
        break;
      case UnaryExpressionOnExpr::U_NEG_OP:
        val = n->expr->acceptExpr(this);
        val = out->writeNeg(val);
        break;
      case UnaryExpressionOnExpr::U_REV_OP:
        val = n->expr->acceptExpr(this);
        val = out->writeRev(val);
        break;
      case UnaryExpressionOnExpr::U_NOT_OP:
        val = n->expr->acceptExpr(this);
        val = out->writeNot(val);
        break;
      case UnaryExpressionOnExpr::U_SIZEOF_OP:
        val = n->expr->acceptExpr(this);
        break;
    }
    return val;
  }

  // unary-expression-on-type ::= sizeof ( type-name )
  //                           | sizeof unary-expression
  Value* visitExpr(UnaryExpressionOnType* n) {}

  Value* visitExpr(Identifier* n)
  {
    auto sym = scopes.find(n->name, SK_VARIABLE);
    if (!sym) {
      std::cerr << "error: undefined variable " << n->name << std::endl;
      exit(1);
    }
    return new Value(sym);
  }

  Value* visitExpr(StringLiteral* n)
  {
    std::string label = "str" + std::to_string(labelCounter++);
    out->writeGlobalData(label.c_str(), n->val);
    auto imm = n->evalConst(scopes);
    return new Value(imm);
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

  // initializer ::= assignment-expression
  //              | { initializer-list ,? }
  // initializer-list ::= designation? initializer (, designation? initializer)*
  void visitInitializer(Initializer* n, Symbol* sym)
  {
    if (isa<ExpressionInitializer>(n)) {
      auto ei = cast<ExpressionInitializer>(n);
      Imm* val = ei->expr->evalConst(scopes);
      sym->setValue(val);
      out->writeComment("initialize %s", sym->getName());
      out->writeAssign(new Value(sym), new Value(val));

    } else if (isa<NestedInitializer>(n)) {
      // auto in = cast<NestedInitializer>(n);
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
    if (n->pointer)
      typ = visitPointer(n->pointer, typ);

    return visitDirectDecl(n->decl, typ);
  }

  // abstract-declarator ::= pointer? direct-abstract-declarator?
  Symbol* visitDecl(AbstractDeclarator* n, Type* typ_prefix)
  {
    Type* typ = typ_prefix;
    if (n->pointer)
      typ = visitPointer(n->pointer, typ);
    if (n->decl)
      return visitDirectDecl(n->decl, typ);
    // anonymous variable
    Symbol* sym = new Symbol(SK_VARIABLE, nullptr);
    sym->setType(typ);
    return sym;
  }
  // direct-declarator ::= identifier
  //                    | ( declarator )
  //                    | direct-declarator [ constant-expression? ]
  //                    | direct-declarator ( )
  //                    | direct-declarator ( parameter-type-list )
  //                    | direct-declarator ( identifier-list )
  Symbol* visitDirectDecl(DirectDeclarator* n, Type* typ_prefix)
  {
    if (isa<IdentifierDeclarator>(n)) {
      auto ident = cast<IdentifierDeclarator>(n);
      auto sym = new Symbol(SK_VARIABLE, ident->name());
      sym->setType(typ_prefix);
      return sym;
    }

    if (isa<ParenthesizedDeclarator>(n)) {
      auto pdecl = cast<ParenthesizedDeclarator>(n);
      return visitDecl(pdecl->decl, typ_prefix);
    }

    if (isa<ArrayDeclarator>(n)) {
      auto adecl = cast<ArrayDeclarator>(n);
      int arr_siz = -1;
      if (adecl->expr) {
        assert(adecl->expr->isConst());
        Imm* v = adecl->expr->evalConst(scopes);
        assert(v->getType()->is_integer());
        arr_siz = v->getInt();
      }
      auto typ = typ_prefix->array_of(arr_siz);
      return visitDirectDecl(adecl->decl, typ);
    }

    // else FunctionDeclarator
    auto fdecl = cast<FunctionDeclarator>(n);
    auto typ = typ_prefix->func_type();

    if (fdecl->kind == FunctionDeclarator::D_NORMAL) {
      // capture function parameters in a new scope
      scopes.enter(Scope::LOCAL);
      for (auto param : *fdecl->params)
        param->accept(this);
      // TODO: handle fdecl->params->varargs
      Scope* top = scopes.getCurrent();
      scopes.leave();
      typ->params = std::move(top->getSymbols());
      delete top;
    } else {   // OLD STYLE
      if (fdecl->oldStyleParams) {
        // TODO: check matching with function prototype
      }
    }

    return visitDirectDecl(fdecl->decl, typ);
  }

  // direct-abstract-declarator ::= ( abstract-declarator )
  //                              | [ constant-expression? ]
  //                              | direct-abstract-declarator [ constant-expression? ]
  //                              | direct-abstract-declarator ( )
  //                              | direct-abstract-declarator ( parameter-type-list )
  Symbol* visitDirectDecl(AbstractDirectDeclarator* n, Type* typ_prefix)
  {
    if (isa<AbstractParenthesizedDeclarator>(n)) {
      auto pdecl = cast<AbstractParenthesizedDeclarator>(n);

      return visitDecl(pdecl->decl, typ_prefix);
    }

    if (isa<AbstractArrayDeclarator>(n)) {
      auto adecl = cast<AbstractArrayDeclarator>(n);
      int arr_siz = -1;
      if (adecl->expr) {
        assert(adecl->expr->isConst());
        Imm* v = adecl->expr->evalConst(scopes);
        assert(v->getType()->is_integer());
        arr_siz = v->getInt();
      }
      auto typ = typ_prefix->array_of(arr_siz);

      if (adecl->decl)
        return visitDirectDecl(adecl->decl, typ);

      // anonymous variable
      Symbol* sym = new Symbol(SK_VARIABLE, nullptr);
      sym->setType(typ);
      return sym;
    }

    // AbstractFunctionDeclarator
    auto fdecl = cast<AbstractFunctionDeclarator>(n);
    auto typ = typ_prefix->func_type();

    if (fdecl->params->size()) {
      scopes.enter(Scope::LOCAL);
      for (auto param : *fdecl->params)
        param->accept(this);

      Scope* top = scopes.getCurrent();
      scopes.leave();

      typ->params = std::move(top->getSymbols());
      delete top;
    }

    if (fdecl->decl)
      return visitDirectDecl(fdecl->decl, typ);

    // anonymous variable
    Symbol* sym = new Symbol(SK_VARIABLE, nullptr);
    sym->setType(typ);
    return sym;
  }

  void visit(AbstractDirectDeclarator* n) {}
  void visit(AbstractParenthesizedDeclarator* n) {}
  void visit(AbstractArrayDeclarator* n) {}
  void visit(AbstractDeclarator* n) {}
  void visit(AbstractFunctionDeclarator* n) {}
  void visit(AbstractParameterDeclaration* n)
  {
    Type* typ = parseFullSpecs(n->declSpecs);
    Symbol* sym = visitDecl(n->decl, typ);
    scopes.add(sym);
  }
  void visit(AnonymousParameterDeclaration* n)
  {
    Type* typ = parseFullSpecs(n->declSpecs);
    Symbol* sym = new Symbol(SK_VARIABLE, nullptr);
    sym->setType(typ);
    scopes.add(sym);
  }
  void visit(ArrayDeclarator* n) {}

  void visit(BreakStatement* n)
  {
    out->writeBreak();
  }

  void visit(CaseStatement* n)
  {
    std::string label = "case" + std::to_string(labelCounter++);
    assert(n->expr->isConst());
    auto val = n->expr->evalConst(scopes);

    auto labsym = new Symbol(SK_CASE_LABEL, label.c_str());
    labsym->setValue(val);
    scopes.add(labsym);

    out->writeLabel(label.c_str());

    n->stmt->accept(this);
  }

  void visit(ContinueStatement* n)
  {
    Scope* loopScope = scopes.find(Scope::LOOP);
    if (!loopScope) {
      std::cerr << "error: continue statement not within a loop\n";
      exit(1);
    }

    out->writeContinue();
  }

  // compound-statement ::= { block-item-list? }
  // block-item-list ::= block-item block-item-list?
  // block-item ::= declaration | statement
  void visit(CompoundStatement* n)
  {
    scopes.enter(Scope::LOCAL);
    for (auto stmt : n->blockItems)
      stmt->accept(this);
    scopes.leave();
  }

  void visit(DefaultStatement* n)
  {
    std::string label = "default" + std::to_string(labelCounter++);

    auto labsym = new Symbol(SK_DEFAULT_LABEL, label.c_str());
    scopes.add(labsym);

    out->writeLabel(label.c_str());

    n->stmt->accept(this);
  }

  void visit(DoWhileStatement* n)
  {
    std::string loopLabel = "loop" + std::to_string(labelCounter++);
    std::string endLabel = "end" + std::to_string(labelCounter++);
    out->writeLabel(loopLabel.c_str());
    n->body->accept(this);
    auto cond = n->cond->acceptExpr(this);
    out->writeJumpIfZero(cond, endLabel.c_str());
    out->writeJump(loopLabel.c_str());
    out->writeLabel(endLabel.c_str());
  }

  void visit(Designation* n) {}

  void visit(Enumerator* n) {}

  void visit(ExpressionInitializer* n) {}

  // expression-statement ::= expression? ;
  void visit(ExpressionStatement* n)
  {
    if (n->expr)
      n->expr->acceptExpr(this);
  }

  void visit(FieldDesignator* n) {}

  // function-definition ::= declaration-specifiers declarator declaration-list? compound-statement
  void visit(FunctionDefinition* n)
  {
    // parse function symbol
    Type* retTypePrefix = parseFullSpecs(n->declSpecs);
    // ANSI style params get parsed here
    auto sym = visitDecl(n->decl, retTypePrefix);
    scopes.add(sym);

    scopes.enter(Scope::FUNCTION);

    if (n->oldStyleParams) {
      // parse function parameters in K&R style
      for (auto param : *n->oldStyleParams)
        param->accept(this);
      // fill back in type->params
      sym->getType()->params = scopes.getCurrent()->getSymbols();
    } else {
      // parse function parameters in ANSI style
      for (auto param : sym->getType()->params)
        scopes.add(param);
    }

    out->writeFunctionPrologue(sym);

    // parse function body
    n->body->accept(this);

    out->writeFunctionEpilogue();

    scopes.leave();
  }

  void visit(ForStatement* n)
  {
    scopes.enter(Scope::LOCAL);

    // for (decl; cond; inc) body
    if (n->decl)
      n->decl->accept(this);
    else if (n->init)
      n->init->accept(this);

    std::string loopLabel = "loop" + std::to_string(labelCounter++);
    std::string endLabel = "end" + std::to_string(labelCounter++);

    out->writeLabel(loopLabel.c_str());
    if (n->cond) {
      auto cond = n->cond->acceptExpr(this);
      out->writeJumpIfZero(cond, endLabel.c_str());
    }

    if (n->inc)
      n->inc->accept(this);

    n->body->accept(this);

    out->writeJump(loopLabel.c_str());

    scopes.leave();
  }

  void visit(GenericAssociation* n) {}

  void visit(GenericSelection* n) {}

  void visit(GotoStatement* n)
  {
    auto sym = scopes.find(n->label);
    if (!sym) {
      std::cerr << "error: undefined label " << n->label << std::endl;
      exit(1);
    }
    out->writeJump(n->label);
  }

  void visit(IfStatement* n)
  {
    std::string elseLabel = "else" + std::to_string(labelCounter++);

    auto cond = n->cond->acceptExpr(this);
    out->writeJumpIfZero(cond, elseLabel.c_str());

    n->then->accept(this);

    out->writeLabel(elseLabel.c_str());

    if (n->els)
      n->els->accept(this);
  }

  void visit(IndexDesignator* n) {}

  void visit(LabeledStatement* n)
  {
    scopes.add(new Symbol(SK_LABEL, n->label));
    out->writeLabel(n->label);

    n->stmt->accept(this);
  }

  void visit(ParameterDeclaration* n)
  {
    Type* typ = parseFullSpecs(n->declSpecs);
    Symbol* sym = visitDecl(n->decl, typ);
    scopes.add(sym);
  }

  // parameter-type-list ::= parameter-list
  //                       | parameter-list , ...
  // parameter-list ::= parameter-declaration (, parameter-declaration)*
  void visit(ParameterList* n)
  {
    for (auto param : n->params)
      param->accept(this);
    // TODO: handle varargs
  }

  void visit(ParenthesizedDeclarator* n) {}

  void visit(ReturnStatement* n)
  {
    if (n->expr) {
      auto dest = n->expr->acceptExpr(this);
      out->writeReturn(dest);
    } else {
      out->writeReturn();
    }
  }

  void visit(SwitchStatement* n)
  {
    auto cond = n->cond->acceptExpr(this);
    std::string endLabel = "end" + std::to_string(labelCounter++);
    std::string caseLabel = "case" + std::to_string(labelCounter++);

    out->writeJump(caseLabel.c_str());

    scopes.enter(Scope::SWITCH);
    n->body->accept(this);
    scopes.leave();

    out->writeLabel(endLabel.c_str());
  }

  void visit(WhileStatement* n)
  {
    std::string loopLabel = "loop" + std::to_string(labelCounter++);
    std::string endLabel = "end" + std::to_string(labelCounter++);
    out->writeLabel(loopLabel.c_str());
    Value* cond = n->cond->acceptExpr(this);
    out->writeJumpIfZero(cond, endLabel.c_str());
    n->body->accept(this);
    out->writeJump(loopLabel.c_str());
    out->writeLabel(endLabel.c_str());
  }

  Type* parseEnum(EnumTypeSpecifier* n)
  {
    Type* typ = Type::enum_type();

    if (n->name) {
      typ->setName(n->name);
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
    if (isa<StaticAssert>(n)) {
      auto sa = cast<StaticAssert>(n);
      assert(0);
      return;
    }

    auto nsd = cast<NormalStructDeclaration>(n);
    auto mtyp = parseSpecs(nsd->declSpecs);

    if (nsd->declarators) {
      for (auto sd : *nsd->declarators) {
        if (sd->bitWidth) {
          // TODO
        }
        auto sym = visitDecl(sd->decl, mtyp);
        typ->structure_add_member(sym);
      }
    }
  }

  // struct-or-union-specifier ::= struct-or-union identifier? { struct-declaration-list }
  //                             | struct-or-union identifier
  Type* parseStructUnion(StructUnionTypeSpecifier* n)
  {
    Type* typ =
      new Type(n->kind == StructUnionTypeSpecifier::S_STRUCT ? Type::TY_STRUCT : Type::TY_UNION);

    if (n->name)
      typ->setName(n->name);

    for (auto& m : *n->declarations)
      parseStructMember(m, typ);

    return typ;
  }

  // specifier-qualifier-list ::= type-specifier specifier-qualifier-list?
  //                            | type-qualifier specifier-qualifier-list?
  Type* parseSpecs(SpecifierQualifierList* n)
  {
    std::vector<DeclarationSpecifier*> specs;
    for (auto i : n->specs)
      specs.push_back(i);
    return __parseFullSpecs(specs);
  }

  // declaration-specifiers ::= storage-class-specifier declaration-specifiers?
  //                          | type-specifier declaration-specifiers?
  //                          | type-qualifier declaration-specifiers?
  //                          | function-specifier declaration-specifiers?
  //                          | alignment-specifier declaration-specifiers?
  // type-specifier ::= void | char | short | int | long | float | double | signed | unsigned
  //                  | _Bool | _Complex | atomic-type-specifier | struct-or-union-specifier
  //                  | enum-specifier | typedef-name
  Type* parseFullSpecs(DeclarationSpecifierList* n)
  {
    return __parseFullSpecs(n->specs);
  }

  Type* __parseFullSpecs(const std::vector<DeclarationSpecifier*>& specs)
  {
    Type* typ = nullptr;
    int signedness = -1;

    for (auto s : specs) {
      if (isa<TypeSpecifier>(s)) {
        auto ts = cast<TypeSpecifier>(s);
        if (isa<BuiltinTypeSpecifier>(ts)) {
          auto bts = cast<BuiltinTypeSpecifier>(ts);
          switch (bts->type) {
            case BuiltinTypeSpecifier::VOID:
              typ = &ty_void;
              break;
            case BuiltinTypeSpecifier::BOOL:
              typ = &ty_bool;
              break;
            case BuiltinTypeSpecifier::CHAR:
              typ = &ty_char;
              break;
            case BuiltinTypeSpecifier::SHORT:
              typ = &ty_short;
              break;
            case BuiltinTypeSpecifier::INT:
              typ = &ty_int;
              break;
            case BuiltinTypeSpecifier::LONG:
              typ = &ty_long;
              break;
            case BuiltinTypeSpecifier::FLOAT:
              typ = &ty_float;
              break;
            case BuiltinTypeSpecifier::DOUBLE:
              typ = &ty_double;
              break;
            case BuiltinTypeSpecifier::COMPLEX:
            case BuiltinTypeSpecifier::IMAGINARY:
              // TODO
              assert(0);
              break;
            case BuiltinTypeSpecifier::SIGNED:
            case BuiltinTypeSpecifier::UNSIGNED:
              if (signedness != -1) {
                std::cerr << "error: multiple signedness specifiers\n";
                exit(1);
              }
              signedness = bts->type == BuiltinTypeSpecifier::SIGNED;
              break;
          }
        } else if (isa<StructUnionTypeSpecifier>(ts)) {
          auto suts = cast<StructUnionTypeSpecifier>(ts);
          bool isStruct = suts->kind == StructUnionTypeSpecifier::S_STRUCT;
          typ = parseStructUnion(suts);
        } else if (isa<EnumTypeSpecifier>(ts)) {
          auto ets = cast<EnumTypeSpecifier>(ts);
          typ = parseEnum(ets);
        } else if (isa<TypedefTypeSpecifier>(ts)) {
          auto tst = cast<TypedefTypeSpecifier>(ts);
          // TODO
          assert(false);
        } else {   // AtomicTypeSpecifier
          auto tsa = cast<AtomicTypeSpecifier>(ts);
          // TODO
          assert(false);
        }
      } else if (isa<TypeQualifier>(s)) {
        auto tq = cast<TypeQualifier>(s);
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
      } else if (isa<StorageClassSpecifier>(s)) {
        auto scs = cast<StorageClassSpecifier>(s);
        switch (scs->storageClass) {
          case StorageClassSpecifier::TYPEDEF:
            typ->is_typedef = true;
            break;
          case StorageClassSpecifier::EXTERN:
            typ->is_extern = true;
            break;
          case StorageClassSpecifier::STATIC:
            typ->is_static = true;
            break;
          case StorageClassSpecifier::THREAD_LOCAL:
            typ->is_thread_local = true;
            break;
          case StorageClassSpecifier::AUTO:
            typ->is_auto = true;
            break;
          case StorageClassSpecifier::REGISTER:
            typ->is_register = true;
            break;
        }
      } else if (isa<AlignmentSpecifier>(s)) {
        auto as = cast<AlignmentSpecifier>(s);
        // TODO
      } else {   // FunctionSpecifier
        auto fs = cast<FunctionSpecifier>(s);
        switch (fs->functionSpecifier) {
          case FunctionSpecifier::INLINE:
            typ->is_inline = true;
            break;
          case FunctionSpecifier::NORETURN:
            typ->is_noreturn = true;
            break;
        }
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
  void visit(NormalDeclaration* n)
  {
    Type* typ = parseFullSpecs(n->declSpecs);

    if (typ->is_aggregate()) {
      // declare a struct/union/enum type
      Symbol* typsym = new Symbol(SK_TYPEDEF, typ->getName());
      typsym->setType(typ);
      scopes.add(typsym);
    }

    if (n->initDecls) {
      // declare a variable/function
      for (auto initDecl : *n->initDecls) {
        // init-declarator ::= declarator (= initializer)?
        Symbol* sym = visitDecl(initDecl->decl, typ);

        sym->place = (scopes.getCurrent()->stack_size += sym->getType()->getSize());

        // Initializer: { 1, 2, 3 }
        if (initDecl->initializer) {
          // only variable can have initializer
          if (sym->getType()->is_function())
            throw std::runtime_error("function cannot have initializer");

          visitInitializer(initDecl->initializer, sym);
        }
        scopes.add(sym);
      }
    }
  }

  // translation-unit ::= external-declaration (external-declaration)*
  void visit(TranslationUnit* n)
  {
    scopes.enter(Scope::GLOBAL);
    for (auto decl : n->decls)
      decl->accept(this);
  }

private:
  int tempCounter = 0;
  int labelCounter = 0;
  ScopeManager scopes;
  AsmHelper* out;
};
