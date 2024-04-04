#pragma once
#include <memory>
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
  virtual std::shared_ptr<Register> allocateRegister(int size = 8) = 0;
  virtual std::shared_ptr<Value> allocateStack(int size = 8) = 0;
  virtual std::shared_ptr<Value> writeAdd(std::shared_ptr<Value> dest,
                                          std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeSub(std::shared_ptr<Value> dest,
                                          std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeMul(std::shared_ptr<Value> dest,
                                          std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeDiv(std::shared_ptr<Value> dest,
                                          std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeMod(std::shared_ptr<Value> dest,
                                          std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeAnd(std::shared_ptr<Value> dest,
                                          std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeOr(std::shared_ptr<Value> dest,
                                         std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeXor(std::shared_ptr<Value> dest,
                                          std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeShl(std::shared_ptr<Value> dest,
                                          std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeShr(std::shared_ptr<Value> dest,
                                          std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeCmpLt(std::shared_ptr<Value> dest,
                                            std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeCmpGt(std::shared_ptr<Value> dest,
                                            std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeCmpLe(std::shared_ptr<Value> dest,
                                            std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeCmpGe(std::shared_ptr<Value> dest,
                                            std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeCmpEq(std::shared_ptr<Value> dest,
                                            std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeCmpNe(std::shared_ptr<Value> dest,
                                            std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeNeg(std::shared_ptr<Value> dest) = 0;
  virtual std::shared_ptr<Value> writeNot(std::shared_ptr<Value> dest) = 0;
  virtual std::shared_ptr<Value> writeRev(std::shared_ptr<Value> dest) = 0;
  virtual void writeComment(const char* fmt, ...) = 0;
  virtual std::shared_ptr<Value> writeAssign(std::shared_ptr<Value> dest,
                                             std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> writeLoad(std::shared_ptr<Value> dest) = 0;
  virtual std::shared_ptr<Value> writeStore(std::shared_ptr<Value> dest,
                                            std::shared_ptr<Value> src) = 0;
  virtual std::shared_ptr<Value> getAddress(std::shared_ptr<Value> val) = 0;
  virtual void writeInc(std::shared_ptr<Value> dest) = 0;
  virtual void writeJump(const char* label) = 0;
  virtual void writeJumpIfZero(std::shared_ptr<Value> dest, const char* label) = 0;
  virtual void writeLabel(const char* label) = 0;
  virtual void writeReturn() = 0;
  virtual void writeReturn(std::shared_ptr<Value> dest) = 0;
  virtual void writeDec(std::shared_ptr<Value> dest) = 0;
  virtual std::shared_ptr<Value> writeCall(std::shared_ptr<Value> fn,
                                           const std::vector<std::shared_ptr<Value>>& args) = 0;
};
