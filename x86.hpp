#pragma once
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
  const char* getName() const override
  {
    return name;
  }

  int getSize() const override
  {
    return size;
  }

  X86Register(const char* name, int size)
    : name(name)
    , size(size)
  {}

private:
  const char* name;
  int size;
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

  X86Helper()
  {
    registers = {
      std::make_shared<X86Register>("%rdi", 8), std::make_shared<X86Register>("%rsi", 8),
      std::make_shared<X86Register>("%rdx", 8), std::make_shared<X86Register>("%rcx", 8),
      std::make_shared<X86Register>("%r8", 8),  std::make_shared<X86Register>("%r9", 8),
      std::make_shared<X86Register>("%r8d", 4), std::make_shared<X86Register>("%r9d", 4),
      std::make_shared<X86Register>("%edi", 4), std::make_shared<X86Register>("%esi", 4),
      std::make_shared<X86Register>("%edx", 4), std::make_shared<X86Register>("%ecx", 4),
      std::make_shared<X86Register>("%r8w", 2), std::make_shared<X86Register>("%r9w", 2),
      std::make_shared<X86Register>("%di", 2),  std::make_shared<X86Register>("%si", 2),
      std::make_shared<X86Register>("%dx", 2),  std::make_shared<X86Register>("%cx", 2),
      std::make_shared<X86Register>("%dil", 1), std::make_shared<X86Register>("%sil", 1),
      std::make_shared<X86Register>("%dl", 1),  std::make_shared<X86Register>("%cl", 1),
      std::make_shared<X86Register>("%r8b", 1), std::make_shared<X86Register>("%r9b", 1),
    };
  }



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
   * Generate code for op two values, and return the std::make_shared<Value>
   *
   * If both values are immediate, the result is immediate
   * The now value may reuse op1 or op2 if they are rvalues
   */
  std::shared_ptr<Value> writeArithOrdered(const char* op, std::shared_ptr<Value> lhs,
                                           std::shared_ptr<Value> rhs)
  {
    // mov lhs, %rax
    // add rhs, %rax
    // return Value(%rax)

    std::shared_ptr<Value> r = moveToFreeRegister(lhs);

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

  std::shared_ptr<Value> writeAdd(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    return writeArithOrdered("add", lhs, rhs);
  }

  std::shared_ptr<Value> writeSub(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    return writeArithOrdered("sub", lhs, rhs);
  }

  std::shared_ptr<Value> writeMul(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    return writeArithOrdered("imul", lhs, rhs);
  }

  std::shared_ptr<Value> writeDiv(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    return writeArithOrdered("idiv", lhs, rhs);
  }

  std::shared_ptr<Value> writeMod(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    return writeArithOrdered("mod", lhs, rhs);
  }

  std::shared_ptr<Value> writeAnd(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    return writeArithOrdered("and", lhs, rhs);
  }

  std::shared_ptr<Value> writeOr(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    return writeArithOrdered("or", lhs, rhs);
  }

  std::shared_ptr<Value> writeXor(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    return writeArithOrdered("xor", lhs, rhs);
  }

  std::shared_ptr<Value> writeShl(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    return writeArithOrdered("shl", lhs, rhs);
  }

  std::shared_ptr<Value> writeShr(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    return writeArithOrdered("shr", lhs, rhs);
  }

  std::shared_ptr<Value> writeCmpLt(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    std::shared_ptr<Value> r = writeArithOrdered("cmp", lhs, rhs);
    writef(WriteTarget::TEXT, "setl %s\n", r->getRegister()->getName());   // set if less
    return r;
  }

  std::shared_ptr<Value> writeCmpGt(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    std::shared_ptr<Value> r = writeArithOrdered("cmp", lhs, rhs);
    writef(WriteTarget::TEXT, "setg %s\n", r->getRegister()->getName());   // set if greater
    return r;
  }

  std::shared_ptr<Value> writeCmpLe(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    std::shared_ptr<Value> r = writeArithOrdered("cmp", lhs, rhs);
    writef(WriteTarget::TEXT, "setle %s\n", r->getRegister()->getName());   // set if less or equal
    return r;
  }

  std::shared_ptr<Value> writeCmpGe(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    std::shared_ptr<Value> r = writeArithOrdered("cmp", lhs, rhs);
    writef(
      WriteTarget::TEXT, "setge %s\n", r->getRegister()->getName());   // set if greater or equal
    return r;
  }

  std::shared_ptr<Value> writeCmpEq(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    std::shared_ptr<Value> r = writeArithOrdered("cmp", lhs, rhs);
    writef(WriteTarget::TEXT, "sete %s\n", r->getRegister()->getName());   // set if equal
    return r;
  }

  std::shared_ptr<Value> writeCmpNe(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    std::shared_ptr<Value> r = writeArithOrdered("cmp", lhs, rhs);
    writef(WriteTarget::TEXT, "setne %s\n", r->getRegister()->getName());   // set if not equal
    return r;
  }

  /**
   * negate x = -x
   */
  std::shared_ptr<Value> writeNeg(std::shared_ptr<Value> lhs)
  {
    std::shared_ptr<Value> r = moveToFreeRegister(lhs);
    writef(WriteTarget::TEXT, "negq %s\n", r->getRegister()->getName());
    return r;
  }

  /**
   * not x = {
   *  1 if x == 0,
   *  0 otherwise
   * }
   */
  std::shared_ptr<Value> writeNot(std::shared_ptr<Value> lhs)
  {
    // test %reg, %reg
    // sete %al
    // movzbl %al, %reg
    std::shared_ptr<Value> r = moveToFreeRegister(lhs);
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
  std::shared_ptr<Value> writeRev(std::shared_ptr<Value> lhs)
  {
    std::shared_ptr<Value> r = moveToFreeRegister(lhs);
    writef(WriteTarget::TEXT, "notq %s\n", r->getRegister()->getName());
    return r;
  }

  /**
   * lhs = rhs
   * The lhs must be an lvalue
   * Return the std::make_shared<Value> of lhs
   */
  std::shared_ptr<Value> writeAssign(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    assert(lhs->isLValue());

    if (lhs->isInMemory()) {
      if (rhs->isImmediate())   // immediate -> memory
        writef(
          WriteTarget::TEXT, "movq $%d, -%d(%%rbp)\n", rhs->getImm()->getInt(), lhs->sym->place);
      else if (rhs->isInMemory()) {   // memory -> memory
        std::shared_ptr<Register> reg = allocateRegister();
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

  std::shared_ptr<Register> allocateRegister(int size = 8)
  {
    for (auto& reg : registers) {
      if (reg->getSize() == size && reg.use_count() == 1)   // not in use
        return reg;
    }
    return nullptr;
  }

  /**
   * Load a value into a register
   * If the value is an immediate, load it into a register
   * If the value is in memory, load it into a register
   * If the value is in a register, return the register
   */
  std::shared_ptr<Value> moveToFreeRegister(std::shared_ptr<Value> val)
  {
    if (val->isRValue() && val->isInRegister())
      return val;

    std::shared_ptr<Register> reg = allocateRegister();
    if (val->isImmediate())
      writef(WriteTarget::TEXT, "movq $%d, %s\n", val->getImm()->getInt(), reg->getName());
    else if (val->isInMemory())   // In Memory
      writef(WriteTarget::TEXT, "movq -%d(%%rbp), %s\n", val->sym->place, reg->getName());
    else
      writef(WriteTarget::TEXT, "movq %s, %s\n", val->getRegister()->getName(), reg->getName());

    return std::make_shared<Value>(reg);
  }

  /**
   * Dereference a value specified by lhs, i.e. *lhs
   */
  std::shared_ptr<Value> writeLoad(std::shared_ptr<Value> lhs)
  {
    if (lhs->isInMemory()) {
      // load to register: movq -%d(%%rbp), %reg
      // dereference: movq (%reg), %reg
      std::shared_ptr<Register> reg = allocateRegister();
      writef(WriteTarget::TEXT, "movq -%d(%%rbp), %s\n", lhs->sym->place, reg->getName());
      return std::make_shared<Value>(reg);
    } else if (lhs->isInRegister()) {
      // dereference: movq (%reg), %reg
      std::shared_ptr<Register> reg = (lhs->isRValue() ? lhs->getRegister() : allocateRegister());
      writef(WriteTarget::TEXT, "movq (%s), %s\n", lhs->getRegister()->getName(), reg->getName());
      return std::make_shared<Value>(reg);
    } else {   // immediate
      // dereference: movq -%d(%%rbp), %reg
      std::shared_ptr<Register> reg = allocateRegister();
      writef(WriteTarget::TEXT, "movq $%d, %s\n", lhs->getImm()->getInt(), reg->getName());
      return std::make_shared<Value>(reg);
    }
  }

  std::shared_ptr<Value> writeStore(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
  {
    if (rhs->isLValue()) {
      // mov lhs, rhs
      writef(WriteTarget::TEXT,
             "movq %s, %s\n",
             rhs->getRegister()->getName(),
             lhs->getRegister()->getName());
    } else {
    }
  }

  void writeInc(std::shared_ptr<Value> lhs)
  {
    writef(WriteTarget::TEXT, "incl %s\n", lhs->getRegister()->getName());
  }

  void writeDec(std::shared_ptr<Value> lhs)
  {
    writef(WriteTarget::TEXT, "decl %s\n", lhs->getRegister()->getName());
  }

  void writeJump(const char* label)
  {
    writef(WriteTarget::TEXT, "jmp %s\n", label);
  }

  void writeJumpIfZero(std::shared_ptr<Value> lhs, const char* label)
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

  void writeReturn()
  {
    writef(WriteTarget::TEXT, "leave\n");
    writef(WriteTarget::TEXT, "ret\n");
  }

  void writeReturn(std::shared_ptr<Value> lhs)
  {
    // mov lhs, %rax
    if (lhs->isImmediate())
      writef(WriteTarget::TEXT, "movq $%d, %%rax\n", lhs->getImm()->getInt());
    else if (lhs->isInMemory())
      writef(WriteTarget::TEXT, "movq -%d(%%rbp), %%rax\n", lhs->sym->place);
    else
      writef(WriteTarget::TEXT, "movq %s, %%rax\n", lhs->getRegister()->getName());
    writeReturn();
  }

  std::shared_ptr<Value> writeCall(std::shared_ptr<Value> fn,
                                   const std::vector<std::shared_ptr<Value>>& args)
  {
    // push args
    // call fn
    // pop args
    // return Value
    return nullptr;
  }

  virtual std::shared_ptr<Value> allocateStack(int size = 8)
  {
    return nullptr;
  }

  virtual std::shared_ptr<Value> getAddress(std::shared_ptr<Value> val)
  {
    assert(val->isLValue() && val->isInMemory());
    // leaq -%d(%%rbp), %reg
    std::shared_ptr<Register> reg = allocateRegister();
    writef(WriteTarget::TEXT, "leaq -%d(%%rbp), %s\n", val->sym->place, reg->getName());
    return std::make_shared<Value>(reg);
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

  std::vector<std::shared_ptr<Register>> registers;
};
