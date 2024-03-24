#pragma once
#include <iostream>
#include <vector>
#include "type.hpp"

class Value;
class Symbol;

class AsmHelper
{
public:
  virtual void writePrologue() = 0;
  virtual void writeEpilogue() = 0;
  virtual void writeGlobalData(const char* label, const char* val) = 0;
  virtual void writeFunctionPrologue(Symbol* sym) = 0;
  virtual void writeFunctionEpilogue() = 0;
  virtual Register* allocateRegister() = 0;
  virtual Value* allocateStack(int size = 8) = 0;
  virtual Value* writeAdd(Value* dest, Value* src) = 0;
  virtual Value* writeSub(Value* dest, Value* src) = 0;
  virtual Value* writeMul(Value* dest, Value* src) = 0;
  virtual Value* writeDiv(Value* dest, Value* src) = 0;
  virtual Value* writeMod(Value* dest, Value* src) = 0;
  virtual Value* writeAnd(Value* dest, Value* src) = 0;
  virtual Value* writeOr(Value* dest, Value* src) = 0;
  virtual Value* writeXor(Value* dest, Value* src) = 0;
  virtual Value* writeShl(Value* dest, Value* src) = 0;
  virtual Value* writeShr(Value* dest, Value* src) = 0;
  virtual Value* writeCmpLt(Value* dest, Value* src) = 0;
  virtual Value* writeCmpGt(Value* dest, Value* src) = 0;
  virtual Value* writeCmpLe(Value* dest, Value* src) = 0;
  virtual Value* writeCmpGe(Value* dest, Value* src) = 0;
  virtual Value* writeCmpEq(Value* dest, Value* src) = 0;
  virtual Value* writeCmpNe(Value* dest, Value* src) = 0;
  virtual Value* writeNeg(Value* dest) = 0;
  virtual Value* writeNot(Value* dest) = 0;
  virtual Value* writeRev(Value* dest) = 0;
  virtual void writeComment(const char* fmt, ...) = 0;
  virtual Value* writeAssign(Value* dest, Value* src) = 0;
  virtual Value* writeLoad(Value* dest) = 0;
  virtual Value* writeStore(Value* dest, Value* src) = 0;
  virtual Value* getAddress(Value* val) = 0;
  virtual void writeBreak() = 0;
  virtual void writeContinue() = 0;
  virtual void writeInc(Value* dest) = 0;
  virtual void writeJump(const char* label) = 0;
  virtual void writeJumpIfZero(Value* dest, const char* label) = 0;
  virtual void writeLabel(const char* label) = 0;
  virtual void writeReturn() = 0;
  virtual void writeReturn(Value* dest) = 0;
  virtual void writeDec(Value* dest) = 0;
  virtual Value* writeCall(Value* fn, const std::vector<Value*>& args) = 0;
};
