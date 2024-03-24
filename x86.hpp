#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdarg>
#include "backend.hpp"
#include "type.hpp"

class X86Register : public Register
{
public:
  X86Register(const char* name)
    : name(name)
  {}

  X86Register(const X86Register& rhs)
    : name(rhs.name)
  {}

  const char* getName() const override
  {
    static char buf[100];
    snprintf(buf, sizeof(buf), "%%%s", name);
    return buf;
  }

public:
  static const std::shared_ptr<Register> dil, sil, dl, cl, r8b, r9b;
  static const std::shared_ptr<Register> di, si, dx, cx, r8w, r9w;
  static const std::shared_ptr<Register> edi, esi, edx, ecx, r8d, r9d;
  static const std::shared_ptr<Register> rdi, rsi, rdx, rcx, r8, r9;

private:
  const char* name;
};

class X86Helper : public AsmHelper
{
public:
  enum class WriteTarget
  {
    TEXT,
    DATA,
  };

  enum Directive
  {
    ALIGN = 0,
    ASCIIZ,
    BYTE,
    DATA,
    SPACE,
    TEXT,
    WORD
  };

  const char* toString(Directive d)
  {
    static const char* DirectiveStrings[] = {
      [ALIGN] = ".align",
      [ASCIIZ] = ".asciiz",
      [BYTE] = ".byte",
      [DATA] = ".data",
      [SPACE] = ".space",
      [TEXT] = ".text",
      [WORD] = ".word",
    };
    return DirectiveStrings[d];
  }

public:
  /**
   * after writing to the buffer call this to "release" the program
   */
  void writeProgram(std::ostream& os)
  {
    os << ".section .data\n";
    os << ".align 16\n";
    os << dataSection;

    os << ".section .text\n";
    os << ".align 16\n";
    os << textSection;
  }

  void writef(WriteTarget t, const char* fmt, ...)
  {
    va_list args;
    va_start(args, fmt);
    writevf(t, fmt, args);
    va_end(args);
  }

  void writevf(WriteTarget t, const char* fmt, va_list args)
  {
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    if (t == WriteTarget::TEXT)
      textSection.append(buffer);
    else
      dataSection.append(buffer);
  }

  void writePrologue() {}

  void writeEpilogue() {}

  void writeString(WriteTarget t, const char* s)
  {
    writef(t, ".string \"%s\"\n", s);
  }

  void writeInt(WriteTarget t, int i)
  {
    writef(t, ".int %d\n", i);
  }

  void writeFloat(WriteTarget t, float f)
  {
    writef(t, ".float %f\n", f);
  }

  void writeLabel(WriteTarget t, const char* labelName)
  {
    writef(t, "%s:\n", labelName);
  }

  void writeFunctionPrologue(Symbol* sym)
  {
    const char* name = sym->getName();
    writef(WriteTarget::TEXT, ".globl %s\n", name);
    writef(WriteTarget::TEXT, ".type %s, @function\n", name);
    writef(WriteTarget::TEXT, "%s:\n", name);

    writef(WriteTarget::TEXT, "pushq %%rbp\n");
    writef(WriteTarget::TEXT, "movq %%rsp, %%rbp\n");

    int stackSize = 0x80;
    for (auto param : sym->getType()->params)
      stackSize += param->getType()->getSize();
    // TODO: allocate stack space for local variables
    writef(WriteTarget::TEXT, "subq $%d, %%rsp\n", stackSize);
  }

  void writeFunctionEpilogue()
  {
    writef(WriteTarget::TEXT, "leave\n");
    writef(WriteTarget::TEXT, "ret\n");
  }

  void writeGlobalData(const char* label, const char* val)
  {
    writef(WriteTarget::DATA, "%s:\n", label);
    writef(WriteTarget::DATA, ".string \"%s\"\n", val);
  }

  /**
   * Generate code for op two values, and return the new value
   *
   * If both values are immediate, the result is immediate
   * The now value may reuse op1 or op2 if they are rvalues
   */
  Value* writeArithOrdered(const char* op, Value* lhs, Value* rhs)
  {
    // mov lhs, %rax
    // add rhs, %rax
    // return Value(%rax)

    Value* r = moveToFreeRegister(lhs);

    if (rhs->isImmediate())
      writef(WriteTarget::TEXT,
             "%sq $%d, %s\n",
             op,
             rhs->getImm()->getInt(),
             r->getRegister()->getName());
    else if (rhs->isInRegister())
      writef(WriteTarget::TEXT,
             "%sq %s, %s\n",
             op,
             rhs->getRegister()->getName(),
             r->getRegister()->getName());
    else
      writef(WriteTarget::TEXT,
             "%sq -%d(%%rbp), %s\n",
             op,
             rhs->sym->place,
             r->getRegister()->getName());

    return r;
  }

  Value* writeAdd(Value* lhs, Value* rhs)
  {
    return writeArithOrdered("add", lhs, rhs);
  }

  Value* writeSub(Value* lhs, Value* rhs)
  {
    return writeArithOrdered("sub", lhs, rhs);
  }

  Value* writeMul(Value* lhs, Value* rhs)
  {
    return writeArithOrdered("imul", lhs, rhs);
  }

  Value* writeDiv(Value* lhs, Value* rhs)
  {
    return writeArithOrdered("idiv", lhs, rhs);
  }

  Value* writeMod(Value* lhs, Value* rhs)
  {
    return writeArithOrdered("mod", lhs, rhs);
  }

  Value* writeAnd(Value* lhs, Value* rhs)
  {
    return writeArithOrdered("and", lhs, rhs);
  }

  Value* writeOr(Value* lhs, Value* rhs)
  {
    return writeArithOrdered("or", lhs, rhs);
  }

  Value* writeXor(Value* lhs, Value* rhs)
  {
    return writeArithOrdered("xor", lhs, rhs);
  }

  Value* writeShl(Value* lhs, Value* rhs)
  {
    return writeArithOrdered("shl", lhs, rhs);
  }

  Value* writeShr(Value* lhs, Value* rhs)
  {
    return writeArithOrdered("shr", lhs, rhs);
  }

  Value* writeCmpLt(Value* lhs, Value* rhs)
  {
    Value* r = writeArithOrdered("cmp", lhs, rhs);
    writef(WriteTarget::TEXT, "setl %s\n", r->getRegister()->getName());   // set if less
    return r;
  }

  Value* writeCmpGt(Value* lhs, Value* rhs)
  {
    Value* r = writeArithOrdered("cmp", lhs, rhs);
    writef(WriteTarget::TEXT, "setg %s\n", r->getRegister()->getName());   // set if greater
    return r;
  }

  Value* writeCmpLe(Value* lhs, Value* rhs)
  {
    Value* r = writeArithOrdered("cmp", lhs, rhs);
    writef(WriteTarget::TEXT, "setle %s\n", r->getRegister()->getName());   // set if less or equal
    return r;
  }

  Value* writeCmpGe(Value* lhs, Value* rhs)
  {
    Value* r = writeArithOrdered("cmp", lhs, rhs);
    writef(
      WriteTarget::TEXT, "setge %s\n", r->getRegister()->getName());   // set if greater or equal
    return r;
  }

  Value* writeCmpEq(Value* lhs, Value* rhs)
  {
    Value* r = writeArithOrdered("cmp", lhs, rhs);
    writef(WriteTarget::TEXT, "sete %s\n", r->getRegister()->getName());   // set if equal
    return r;
  }

  Value* writeCmpNe(Value* lhs, Value* rhs)
  {
    Value* r = writeArithOrdered("cmp", lhs, rhs);
    writef(WriteTarget::TEXT, "setne %s\n", r->getRegister()->getName());   // set if not equal
    return r;
  }

  /**
   * negate x = -x
   */
  Value* writeNeg(Value* lhs)
  {
    Value* r = moveToFreeRegister(lhs);
    writef(WriteTarget::TEXT, "negq %s\n", r->getRegister()->getName());
    return r;
  }

  /**
   * not x = {
   *  1 if x == 0,
   *  0 otherwise
   * }
   */
  Value* writeNot(Value* lhs)
  {
    // test %reg, %reg
    // sete %al
    // movzbl %al, %reg
    Value* r = moveToFreeRegister(lhs);
    writef(WriteTarget::TEXT,
           "testq %s, %s\n",
           r->getRegister()->getName(),
           r->getRegister()->getName());
    writef(WriteTarget::TEXT, "sete al\n");
    writef(WriteTarget::TEXT, "movzbl al, %s\n", r->getRegister()->getName());
    return r;
  }

  /**
   * reverse x = ~x
   */
  Value* writeRev(Value* lhs)
  {
    Value* r = moveToFreeRegister(lhs);
    writef(WriteTarget::TEXT, "notq %s\n", r->getRegister()->getName());
    return r;
  }

  /**
   * lhs = rhs
   * The lhs must be an lvalue
   * Return the new value of lhs
   */
  Value* writeAssign(Value* lhs, Value* rhs)
  {
    assert(lhs->isLValue());

    if (lhs->isInMemory()) {
      if (rhs->isImmediate())   // immediate -> memory
        writef(
          WriteTarget::TEXT, "movq $%d, -%d(%%rbp)\n", rhs->getImm()->getInt(), lhs->sym->place);
      else if (rhs->isInMemory()) {   // memory -> memory
        Register* reg = allocateRegister();
        writef(WriteTarget::TEXT, "movq -%d(%%rbp), %s\n", rhs->sym->place, reg->getName());
        writef(WriteTarget::TEXT, "movq %s, -%d(%%rbp)\n", reg->getName(), lhs->sym->place);
      } else   // register -> memory
        writef(WriteTarget::TEXT,
               "movq %s, -%d(%%rbp)\n",
               rhs->getRegister()->getName(),
               lhs->sym->place);
    } else {
      if (rhs->isImmediate())   // immediate -> register
        writef(WriteTarget::TEXT,
               "movq $%d, %s\n",
               rhs->getImm()->getInt(),
               lhs->getRegister()->getName());
      else if (rhs->isInMemory())   // memory -> register
        writef(WriteTarget::TEXT,
               "movq -%d(%%rbp), %s\n",
               rhs->sym->place,
               lhs->getRegister()->getName());
      else   // register -> register
        writef(WriteTarget::TEXT,
               "movq %s, %s\n",
               rhs->getRegister()->getName(),
               lhs->getRegister()->getName());
    }

    return lhs;
  }

  std::shared_ptr<Register> allocateRegister()
  {
    static int regCount = 0;
    static const char* registers[] = {
      "rdi",
      "rsi",
      "rdx",
      "rcx",
      "r8",
      "r9",
    };
    return new X86Register(registers[regCount++ % 6]);
  }

  /**
   * Load a value into a register
   * If the value is an immediate, load it into a register
   * If the value is in memory, load it into a register
   * If the value is in a register, return the register
   */
  Value* moveToFreeRegister(Value* val)
  {
    if (val->isRValue() && val->isInRegister())
      return val;

    Register* reg = allocateRegister();
    if (val->isImmediate())
      writef(WriteTarget::TEXT, "movq $%d, %s\n", val->getImm()->getInt(), reg->getName());
    else if (val->isInMemory())   // In Memory
      writef(WriteTarget::TEXT, "movq -%d(%%rbp), %s\n", val->sym->place, reg->getName());
    else
      writef(WriteTarget::TEXT, "movq %s, %s\n", val->getRegister()->getName(), reg->getName());

    return new Value(reg);
  }

  /**
   * Dereference a value specified by lhs, i.e. *lhs
   */
  Value* writeLoad(Value* lhs)
  {
    if (lhs->isInMemory()) {
      // load to register: movq -%d(%%rbp), %reg
      // dereference: movq (%reg), %reg
      Register* reg = allocateRegister();
      writef(WriteTarget::TEXT, "movq -%d(%%rbp), %s\n", lhs->sym->place, reg->getName());
      return new Value(reg);
    } else if (lhs->isInRegister()) {
      // dereference: movq (%reg), %reg
      Register* reg = (lhs->isRValue() ? lhs->getRegister() : allocateRegister());
      writef(WriteTarget::TEXT, "movq (%s), %s\n", lhs->getRegister()->getName(), reg->getName());
      return new Value(reg);
    } else {   // immediate
      // dereference: movq -%d(%%rbp), %reg
      Register* reg = allocateRegister();
      writef(WriteTarget::TEXT, "movq $%d, %s\n", lhs->getImm()->getInt(), reg->getName());
      return new Value(reg);
    }
  }

  Value* writeStore(Value* lhs, Value* rhs) {}

  void writeBreak() {}
  void writeContinue() {}

  void writeInc(Value* lhs)
  {
    writef(WriteTarget::TEXT, "incl %s\n", lhs->getRegister()->getName());
  }

  void writeJump(const char* label)
  {
    writef(WriteTarget::TEXT, "jmp %s\n", label);
  }

  void writeJumpIfZero(Value* lhs, const char* label)
  {
    writef(WriteTarget::TEXT,
           "testl %s, %s\n",
           lhs->getRegister()->getName(),
           lhs->getRegister()->getName());
    writef(WriteTarget::TEXT, "je %s\n", label);
  }

  void writeLabel(const char* label)
  {
    writef(WriteTarget::TEXT, "%s:\n", label);
  }

  void writeReturn() {}

  void writeReturn(Value* lhs) {}

  void writeDec(Value* lhs) {}

  Value* writeCall(Value* fn, const std::vector<Value*>& args)
  {
    // push args
    // call fn
    // pop args
    // return Value
    return nullptr;
  }

  virtual Value* allocateStack(int size = 8)
  {
    return nullptr;
  }

  virtual Value* getAddress(Value* val)
  {
    assert(val->isLValue() && val->isInMemory());
    // leaq -%d(%%rbp), %reg
    Register* reg = allocateRegister();
    writef(WriteTarget::TEXT, "leaq -%d(%%rbp), %s\n", val->sym->place, reg->getName());
    return new Value(reg);
  }

  void writeComment(const char* fmt, ...)
  {
    va_list args;
    va_start(args, fmt);
    writef(WriteTarget::TEXT, "# ");
    writevf(WriteTarget::TEXT, fmt, args);
    writef(WriteTarget::TEXT, "\n");
    va_end(args);
  }

private:
  int dataIndent = 0;
  int textIndent = 0;

  std::string dataSection;
  std::string textSection;
};
