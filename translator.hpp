#pragma once
#include "ast.hpp"
#include "c2.hpp"
#include "type.hpp"
#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
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
  std::shared_ptr<Value> visitExpr(ArithmeticExpression* n)
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
  std::shared_ptr<Value> visitExpr(AssignmentExpression* n)
  {
    std::shared_ptr<Value> lhs = n->lhs->acceptExpr(this);
    std::shared_ptr<Value> rhs = n->rhs->acceptExpr(this);
    std::shared_ptr<Value> res;
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

  std::shared_ptr<Value> visitExpr(ConstantI* n)
  {
    return std::make_shared<Value>(n->evalConst(scopes));
  }

  std::shared_ptr<Value> visitExpr(ConstantF* n)
  {
    return std::make_shared<Value>(n->evalConst(scopes));
  }

  // cast-expression ::= ( type-name ) cast-expression
  std::shared_ptr<Value> visitExpr(CastExpression* n)
  {
    // TODO: implement cast
    return n->expr->acceptExpr(this);
  }

  // comma-expression ::= assignment-expression , comma-expression
  std::shared_ptr<Value> visitExpr(CommaExpression* n)
  {
    std::shared_ptr<Value> dest = nullptr;
    for (auto expr : n->exprs)
      dest = expr->acceptExpr(this);
    return dest;
  }

  // conditional-expression ::= logical-or-expression ? expression : conditional-expression
  std::shared_ptr<Value> visitExpr(ConditionalExpression* n)
  {
    std::string elseLabel = ".else" + std::to_string(labelCounter);
    std::string endLabel = ".end" + std::to_string(labelCounter++);
    std::shared_ptr<Value> dest = std::make_shared<Value>(out->allocateRegister());

    {
      std::shared_ptr<Value> cond = n->cond->acceptExpr(this);
      out->writeJumpIfZero(cond, elseLabel.c_str());
    }

    {
      std::shared_ptr<Value> then = n->then->acceptExpr(this);
      out->writeAssign(dest, then);
      out->writeJump(endLabel.c_str());
    }

    {
      out->writeLabel(elseLabel.c_str());
      std::shared_ptr<Value> els = n->els->acceptExpr(this);
      out->writeAssign(dest, els);
    }

    out->writeLabel(endLabel.c_str());
    return dest;
  }

  std::shared_ptr<Value> visitExpr(EnumerationConstantUse* n)
  {
    return std::make_shared<Value>(n->evalConst(scopes));
  }

  std::shared_ptr<Value> visitExpr(FuncName* n)
  {
    return std::make_shared<Value>(n->evalConst(scopes));
  }

  // primary-expression ::= ( expression )
  std::shared_ptr<Value> visitExpr(ParenthesizedExpression* n)
  {
    return n->expr->acceptExpr(this);
  }

  // expression INC_OP
  // Return the value of the expression before increment
  // as an rvalue
  std::shared_ptr<Value> visitExpr(PostfixExpressionIncOp* n)
  {
    std::shared_ptr<Value> dest = n->expr->acceptExpr(this);
    assert(dest->isLValue() && "increment on rvalue");

    std::shared_ptr<Value> ret = std::make_shared<Value>(out->allocateRegister());
    out->writeAssign(ret, dest);

    out->writeInc(dest);
    return ret;
  }

  // expression DEC_OP
  std::shared_ptr<Value> visitExpr(PostfixExpressionDecOp* n)
  {
    std::shared_ptr<Value> dest = n->expr->acceptExpr(this);
    assert(dest->isLValue() && "decrement on rvalue");

    std::shared_ptr<Value> ret = std::make_shared<Value>(out->allocateRegister());
    out->writeAssign(ret, dest);

    out->writeDec(dest);
    return ret;
  }

  std::shared_ptr<Value> visitExpr(PostfixExpressionCallOp* n)
  {
    auto fn = n->expr->acceptExpr(this);
    std::vector<std::shared_ptr<Value>> args;
    for (auto arg : *n->args)
      args.push_back(arg->acceptExpr(this));
    return out->writeCall(fn, args);
  }

  std::shared_ptr<Value> visitExpr(PostfixExpressionDotOp* n)
  {
    auto dest = n->expr->acceptExpr(this);
    // TODO: handle struct member access
    // out->writeOffset(dest, n->field);
    return dest;
  }

  std::shared_ptr<Value> visitExpr(PostfixExpressionPtrOp* n)
  {
    auto dest = n->expr->acceptExpr(this);
    // TODO: handle struct member access
    return dest;
  }

  std::shared_ptr<Value> visitExpr(PostfixExpressionIndexOp* n)
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
  std::shared_ptr<Value> visitExpr(PostfixExpressionCastOp* n)
  {
    // TODO: implement cast
    throw std::runtime_error("not implemented");
  }

  // unary-expression-on-expr ::= postfix-expression
  //                            | ++ expression
  //                            | -- expression
  //                            | &*+-!~ expression
  //                            | sizeof expression
  std::shared_ptr<Value> visitExpr(UnaryExpressionOnExpr* n)
  {
    std::shared_ptr<Value> val = nullptr;
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
        val = std::make_shared<Value>(new Imm(val->sym->typ->getSize()));
        break;
    }
    return val;
  }

  // unary-expression-on-type ::= sizeof ( type-name )
  //                           | alignof ( type-name )
  std::shared_ptr<Value> visitExpr(UnaryExpressionOnType* n)
  {
    Type* typ = parseTypeName(n->typeName);
    return std::make_shared<Value>(
      new Imm((n->op == UnaryExpressionOnType::U_SIZEOF_OP ? typ->getSize() : 8)));
  }

  std::shared_ptr<Value> visitExpr(Identifier* n)
  {
    auto sym = scopes.findSymbol(n->name, SY_VARIABLE);
    if (!sym) {
      std::cerr << "error: undefined variable " << n->name << std::endl;
      exit(1);
    }
    return std::make_shared<Value>(sym);
  }

  std::shared_ptr<Value> visitExpr(StringLiteral* n)
  {
    std::string label = "str" + std::to_string(labelCounter++);
    out->writeGlobalData(label.c_str(), n->val);
    auto imm = n->evalConst(scopes);
    return std::make_shared<Value>(imm);
  }

  // pointer ::= * type-qualifier* pointer?
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

  // initializer-list ::= designation? initializer (, designation? initializer)*
  // initializer ::= assignment-expression
  //              | { initializer-list ','? }
  // designation ::= (designator)+ '='
  // designator ::= . IDENTIFIER |  [ expression ]
  void visitInitializer(Initializer* n, Symbol* sym)
  {
    if (auto ei = dyn_cast<ExpressionInitializer>(n)) {
      Imm* val = ei->expr->evalConst(scopes);
      sym->setValue(std::make_shared<Value>(val));
      out->writeComment("%s = %d", sym->getName(), val->getInt());
      out->writeAssign(std::make_shared<Value>(sym), std::make_shared<Value>(val));
    } else if (isa<NestedInitializer>(n)) {
      // auto in = cast<NestedInitializer>(n);
      // for (auto init : *in->initializers) {
      //   visitInitializer(init, sym);
      // }
      throw std::runtime_error("not implemented");
    } else {
      assert(0);
    }

    // TODO: handle n.designation
    // emit code for initialization
  }

  // declarator ::= pointer? direct-declarator
  Symbol* visitDecl(Declarator* n, Type* typ_prefix)
  {
    Type* typ = typ_prefix;
    if (n->pointer)
      typ = visitPointer(n->pointer, typ);

    return visitDirectDecl(n->decl, typ);
  }

  // abstract-declarator ::= pointer? direct-abstract-declarator?
  // ..Abstract declarator is anonymous declarator.
  // ..Note that the direct-abstract-declarator may not exist.
  Type* visitAbstDecl(AbstractDeclarator* n, Type* typ_prefix)
  {
    Type* typ = typ_prefix;
    if (n->pointer)
      typ = visitPointer(n->pointer, typ);

    if (!n->decl)
      return typ;

    return visitDirectAbstDecl(n->decl, typ);
  }

  // direct-declarator ::= identifier
  //                    | ( declarator )
  //                    | direct-declarator [ static? type-qualifier* expression? ]
  //                    | direct-declarator [ type-qualifier* '*'? ]
  //                    | direct-declarator ( parameter-type-list? )
  //                    | direct-declarator ( identifier-list )    // Old K&R style
  Symbol* visitDirectDecl(DirectDeclarator* n, Type* typ_prefix)
  {
    if (auto ident = dyn_cast<IdentifierDeclarator>(n)) {
      auto sym = new Symbol(SY_VARIABLE, ident->name());
      sym->setType(typ_prefix);
      return sym;
    }

    if (auto pdecl = dyn_cast<ParenthesizedDeclarator>(n)) {
      return visitDecl(pdecl->decl, typ_prefix);
    }

    if (auto adecl = dyn_cast<ArrayDeclarator>(n)) {
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

    if (auto fdecl = dyn_cast<FunctionDeclarator>(n)) {
      auto typ = typ_prefix->func_type();

      if (fdecl->params) {
        parseParams(fdecl->params, &typ->params);
        typ->hasEllipsis = fdecl->params->hasEllipsis;
      } else {   // old style params
        // TODO: check matching with function prototype
      }

      return visitDirectDecl(fdecl->decl, typ);
    }

    assert(false);
  }

  // direct-abstract-declarator ::= ( abstract-declarator )
  //                             | direct-abstract-declarator? [ '*'? ]
  //                             | direct-abstract-declarator? [ static? type-qualifier*
  //                                                             expression? ]
  //                             | direct-abstract-declarator? ( parameter-type-list? )
  // ..Abstract declarator: No identifier allowed, Base declarator is optional.
  Type* visitDirectAbstDecl(AbstractDirectDeclarator* n, Type* typ_prefix)
  {
    if (auto pdecl = dyn_cast<AbstractParenthesizedDeclarator>(n)) {
      return visitAbstDecl(pdecl->decl, typ_prefix);
    }

    if (auto adecl = dyn_cast<AbstractArrayDeclarator>(n)) {
      int arr_siz = -1;

      if (adecl->expr) {
        assert(adecl->expr->isConst());
        Imm* v = adecl->expr->evalConst(scopes);
        assert(v->getType()->is_integer());
        arr_siz = v->getInt();
      }

      auto typ = typ_prefix->array_of(arr_siz);

      for (auto tq : *adecl->typeQualifierList)
        setTypeQualifiers(typ, tq);

      if (!adecl->decl)
        return typ;

      return visitDirectAbstDecl(adecl->decl, typ);
    }

    if (auto fdecl = dyn_cast<AbstractFunctionDeclarator>(n)) {
      auto typ = typ_prefix->func_type();

      if (fdecl->params->size()) {
        parseParams(fdecl->params, &typ->params);
        typ->hasEllipsis = fdecl->params->hasEllipsis;
      }

      if (!fdecl->decl)
        return typ;

      return visitDirectAbstDecl(fdecl->decl, typ);
    }

    assert(false);
  }

  void visit(BreakStatement* n)
  {
    Symbol* scope = scopes.findSymbol(nullptr, SY_BREAK);
    if (!scope) {
      std::cerr << "error: break statement not within a loop or switch\n";
      exit(1);
    }

    out->writeJump(scope->getName());
  }

  void visit(CaseStatement* n)
  {
    std::string label = ".case" + std::to_string(labelCounter++);

    assert(n->expr->isConst());
    auto val = n->expr->evalConst(scopes);

    auto labsym = new Symbol(SY_CASE_LABEL, label.c_str());
    labsym->setValue(std::make_shared<Value>(val));
    scopes.add(labsym);

    out->writeLabel(label.c_str());

    n->stmt->accept(this);
  }

  void visit(ContinueStatement* n)
  {
    Symbol* cont = scopes.findSymbol(nullptr, SY_CONTINUE);
    if (!cont) {
      std::cerr << "error: continue statement not within a loop\n";
      exit(1);
    }

    out->writeJump(cont->getName());
  }

  // compound-statement ::= { block-item-list? }
  // block-item-list ::= block-item block-item-list?
  // block-item ::= declaration | statement
  void visit(CompoundStatement* n)
  {
    scopes.enter(SC_LOCAL);
    for (auto stmt : n->blockItems)
      stmt->accept(this);
    scopes.leave();
  }

  void visit(DefaultStatement* n)
  {
    std::string label = ".default" + std::to_string(labelCounter++);

    scopes.add(new Symbol(SY_DEFAULT_LABEL, label.c_str()));

    out->writeLabel(label.c_str());

    n->stmt->accept(this);
  }

  // DO statement WHILE '(' expression ')' ';'
  void visit(DoWhileStatement* n)
  {
    std::string loopLabel = ".loop" + std::to_string(labelCounter++);
    std::string endLabel = ".end" + std::to_string(labelCounter++);

    scopes.add(new Symbol(SY_CONTINUE, loopLabel.c_str()));
    scopes.add(new Symbol(SY_BREAK, endLabel.c_str()));

    out->writeLabel(loopLabel.c_str());

    n->body->accept(this);

    auto cond = n->cond->acceptExpr(this);

    out->writeJumpIfZero(cond, endLabel.c_str());
    out->writeJump(loopLabel.c_str());
    out->writeLabel(endLabel.c_str());
  }

  // expression-statement ::= expression? ;
  void visit(ExpressionStatement* n)
  {
    if (n->expr)
      n->expr->acceptExpr(this);
  }

  // function-definition ::= declaration-specifiers declarator declaration-list? compound-statement
  void visit(FunctionDefinition* n)
  {
    // parse function symbol
    Type* retTypePrefix = parseFullSpecs(n->declSpecs);
    // ANSI style params get parsed here
    auto sym = visitDecl(n->decl, retTypePrefix);
    scopes.add(sym);

    // Function body
    scopes.enter(SC_FUNCTION);
    scopes.add(new Symbol(SY_FUNCNAME, sym->getName()));

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
    // for (decl; cond; inc) body
    if (n->decl)
      n->decl->accept(this);
    else if (n->init)
      n->init->accept(this);

    std::string loopLabel = ".loop" + std::to_string(labelCounter++);
    std::string endLabel = ".end" + std::to_string(labelCounter++);

    scopes.add(new Symbol(SY_CONTINUE, loopLabel.c_str()));
    scopes.add(new Symbol(SY_BREAK, endLabel.c_str()));

    out->writeLabel(loopLabel.c_str());

    if (n->cond) {
      auto cond = n->cond->acceptExpr(this);
      out->writeJumpIfZero(cond, endLabel.c_str());
    }

    if (n->inc)
      n->inc->accept(this);

    n->body->accept(this);

    out->writeJump(loopLabel.c_str());
  }

  void visit(GenericAssociation* n) {}

  void visit(GenericSelection* n) {}

  void visit(GotoStatement* n)
  {
    auto sym = scopes.findSymbol(n->label, SY_LABEL);
    if (!sym) {
      std::cerr << "error: undefined label " << n->label << std::endl;
      exit(1);
    }

    out->writeJump(n->label);
  }

  void visit(IfStatement* n)
  {
    std::string elseLabel = ".else" + std::to_string(labelCounter++);

    {
      auto cond = n->cond->acceptExpr(this);
      out->writeJumpIfZero(cond, elseLabel.c_str());
    }

    n->then->accept(this);

    out->writeLabel(elseLabel.c_str());

    if (n->els)
      n->els->accept(this);
  }

  void visit(IndexDesignator* n) {}

  void visit(LabeledStatement* n)
  {
    scopes.add(new Symbol(SY_LABEL, n->label));
    out->writeLabel(n->label);

    n->stmt->accept(this);
  }

  // parameter-list ::= parameter-declaration (, parameter-declaration)*
  // parameter-declaration ::= declaration_specifiers (declarator | abstract_declarator)?
  void parseParams(ParameterList* n, std::vector<Symbol*>* out)
  {
    for (auto param : n->params) {
      Symbol* sym = nullptr;

      // Kindly note: C disallow define new types in func params.
      Type* typ = parseFullSpecs(param->declSpecs);

      assert(!(param->decl && param->adecl));   // cannot both

      if (param->decl)
        sym = visitDecl(param->decl, typ);
      else {
        if (param->adecl)
          typ = visitAbstDecl(param->adecl, typ);

        sym = new Symbol(SY_ABSTRACT_VARIABLE, nullptr);
        sym->setType(typ);
      }

      out->push_back(sym);
    }
  }

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
    std::shared_ptr<Value> cond = n->cond->acceptExpr(this);

    Symbol* switchVar = new Symbol(SY_VARIABLE, nullptr);
    switchVar->setType(cond->sym->typ);
    switchVar->setValue(cond);

    scopes.enter(SC_SWITCH);
    scopes.add(switchVar);
    // TODO: handle switch statement

    n->body->accept(this);

    scopes.leave();
  }

  void visit(WhileStatement* n)
  {
    std::string loopLabel = ".loop" + std::to_string(labelCounter++);
    std::string endLabel = ".end" + std::to_string(labelCounter++);

    scopes.add(new Symbol(SY_CONTINUE, loopLabel.c_str()));
    scopes.add(new Symbol(SY_BREAK, endLabel.c_str()));

    out->writeLabel(loopLabel.c_str());

    {
      std::shared_ptr<Value> cond = n->cond->acceptExpr(this);
      out->writeJumpIfZero(cond, endLabel.c_str());
    }

    n->body->accept(this);

    out->writeJump(loopLabel.c_str());
    out->writeLabel(endLabel.c_str());
  }

  // enum-specifier ::= 'enum' IDENTIFIER? ('{' enumerator (, enumerator)?'}')?
  // enumerator ::= IDENTIFIER '=' constant-expression
  Type* parseEnum(EnumTypeSpecifier* n)
  {
    Type* typ = Type::enum_type();

    if (n->name) {
      typ->setName(n->name);
    }

    if (n->enumerators) {
      for (auto enumerator : *n->enumerators) {
        Symbol* sym = new Symbol(SY_ENUMERATION_CONSTANT, enumerator->identifier->name);
        scopes.add(sym);
      }
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
    return parseFullSpecs(std::vector<DeclarationSpecifier*>{n->specs.begin(), n->specs.end()});
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
    return parseFullSpecs(n->specs);
  }

  static void setTypeQualifiers(Type* typ, TypeQualifier* q)
  {
    switch (q->typeQualifier) {
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
  }

  Type* parseFullSpecs(const std::vector<DeclarationSpecifier*>& specs)
  {
    Type* typ = nullptr;
    int signedness = -1;

    for (auto s : specs) {
      if (auto ts = dyn_cast<TypeSpecifier>(s)) {
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
      } else if (auto tq = dyn_cast<TypeQualifier>(s)) {
        setTypeQualifiers(typ, tq);
      } else if (auto scs = dyn_cast<StorageClassSpecifier>(s)) {
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
      } else if (auto as = dyn_cast<AlignmentSpecifier>(s)) {
        // TODO
      } else if (auto fs = dyn_cast<FunctionSpecifier>(s)) {
        switch (fs->functionSpecifier) {
          case FunctionSpecifier::INLINE:
            typ->is_inline = true;
            break;
          case FunctionSpecifier::NORETURN:
            typ->is_noreturn = true;
            break;
        }
      } else {
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

  // type-name ::= specifier-qualifier-list abstract-declarator?
  Type* parseTypeName(TypeName* n)
  {
    Type* typ = parseSpecs(n->declSpecs);
    if (n->adecl)
      typ = visitAbstDecl(n->adecl, typ);
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
      Symbol* typsym = new Symbol(SY_TYPEDEF, typ->getName());
      typsym->setType(typ);
      scopes.add(typsym);
    }

    if (n->initDecls) {
      // declare a variable/function
      for (auto initDecl : *n->initDecls) {
        // init-declarator ::= declarator (= initializer)?
        Symbol* sym = visitDecl(initDecl->decl, typ);

        sym->place = (scopes.getCurrent()->stack_size += sym->getType()->getSize());

        // initial value
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
    scopes.enter(SC_GLOBAL);
    for (auto decl : n->decls)
      decl->accept(this);
  }

private:
  int tempCounter = 0;
  int labelCounter = 0;
  ScopeManager scopes;
  AsmHelper* out;
};
