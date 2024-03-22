#pragma once
#include <iostream>
#include <vector>

class Value;
class Symbol;

class Register
{
public:
  // "%rax", etc.
  virtual const char* getName() const = 0;
};

class AsmHelper
{
public:
  virtual void writePrologue();
  virtual void writeEpilogue();
  virtual void writeGlobalData(const char* label, const char* val);
  virtual void writeFunctionPrologue(Symbol* sym);
  virtual void writeFunctionEpilogue();
  virtual Register* allocateRegister();
  virtual Value* allocateStack(int size = 8);
  virtual Value* writeAdd(Value* dest, Value* src);
  virtual Value* writeSub(Value* dest, Value* src);
  virtual Value* writeMul(Value* dest, Value* src);
  virtual Value* writeDiv(Value* dest, Value* src);
  virtual Value* writeMod(Value* dest, Value* src);
  virtual Value* writeAnd(Value* dest, Value* src);
  virtual Value* writeOr(Value* dest, Value* src);
  virtual Value* writeXor(Value* dest, Value* src);
  virtual Value* writeShl(Value* dest, Value* src);
  virtual Value* writeShr(Value* dest, Value* src);
  virtual Value* writeCmpLt(Value* dest, Value* src);
  virtual Value* writeCmpGt(Value* dest, Value* src);
  virtual Value* writeCmpLe(Value* dest, Value* src);
  virtual Value* writeCmpGe(Value* dest, Value* src);
  virtual Value* writeCmpEq(Value* dest, Value* src);
  virtual Value* writeCmpNe(Value* dest, Value* src);
  virtual Value* writeNeg(Value* dest);
  virtual Value* writeNot(Value* dest);
  virtual Value* writeRev(Value* dest);
  virtual Value* writeAssign(Value* dest, Value* src);
  virtual Value* writeLoad(Value* dest);
  virtual Value* writeStore(Value* dest, Value* src);
  virtual Value* getAddress(Value* val);
  virtual void writeBreak();
  virtual void writeContinue();
  virtual void writeInc(Value* dest);
  virtual void writeJump(const char* label);
  virtual void writeJumpIfZero(Value* dest, const char* label);
  virtual void writeLabel(const char* label);
  virtual void writeReturn();
  virtual void writeReturn(Value* dest);
  virtual void writeDec(Value* dest);
  virtual Value* writeCall(Value* fn, const std::vector<Value*>& args);
};