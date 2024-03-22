#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

class Register {
public:
  Register(int num, std::string name) : num(num), name(name) {}

  std::string to_string() const { return "$" + name; }

private:
  int num;          // register number
  std::string name; // register name
};

class MipsRegister {
  /*
   * definition of registers
   */
  static const Register zero;

  static const Register v0;
  static const Register v1;

  static const Register paramRegs[];

  static const Register tmpRegs[];

  /** use for temporary values which cannot use a temporary register and which
   * you know noone will mess with ! */
  static const Register tempFP;
  static const Register tempSP;
  static const Register tempVal1;
  static const Register tempVal2;

  static const Register gp;
  static const Register sp;
  static const Register fp;
  static const Register ra;
};

/**
 * Used to write a mips file. Allows writing to the data and text section in
 * parallel, uses a buffer to store write calls whicih is unfolded at const
 * write.
 */
class MipsWriter {
public:
  enum class WriteTarget {
    TEXT,
    DATA,
  };

  class Directive {
  public:
    enum _Value { ALIGN = 0, ASCIIZ, BYTE, DATA, SPACE, TEXT, WORD } value;

    Directive(_Value v) : value(v) {}

    bool operator==(const Directive &other) const {
      return value == other.value;
    }

    const char *toString() {
      static const char *DirectiveStrings[] = {
          [ALIGN] = ".align", [ASCIIZ] = ".asciiz", [BYTE] = ".byte",
          [DATA] = ".data",   [SPACE] = ".space",   [TEXT] = ".text",
          [WORD] = ".word",
      };
      return DirectiveStrings[value];
    }
  };

public:
  /**
   * after writing to the buffer call this to "release" the program
   */
  void writeProgram(std::ostream &os) {
    os << dataSection;
    os << textSection;
  }

  void writeTo(WriteTarget t, const std::string &s) {
    if (t == WriteTarget::TEXT) {
      for (int i = 0; i < idntLvlTxtSection; i++) {
        textSection.append("\t");
      }
      textSection.append(s);
    } else {
      for (int i = 0; i < idntLvlDtaSection; i++) {
        dataSection.append("\t");
      }
      dataSection.append(s);
    }
  }

  void writeTo(WriteTarget t, const char c) {
    char s[2];
    s[0] = c;
    s[1] = '\0';
    writeTo(t, s);
  }

  void writeToNoIdnt(WriteTarget t, const std::string &s) {
    if (t == WriteTarget::TEXT) {
      textSection.append(s);
    } else {
      dataSection.append(s);
    }
  }

  void writeToNoIdnt(WriteTarget t, const char s) {
    if (t == WriteTarget::TEXT) {
      textSection += s;
    } else {
      dataSection += s;
    }
  }

  void changeIdnt(WriteTarget t, int diff) {
    if (t == WriteTarget::TEXT) {
      idntLvlTxtSection += diff;
    } else {
      idntLvlDtaSection += diff;
    }
  }

  void setIdnt(WriteTarget t, int val) {
    if (t == WriteTarget::TEXT) {
      idntLvlTxtSection = val;
    } else {
      idntLvlDtaSection = val;
    }
  }

  int getIdnt(WriteTarget t) {
    if (t == WriteTarget::TEXT) {
      return idntLvlTxtSection;
    } else {
      return idntLvlDtaSection;
    }
  }

  void writeNewline(WriteTarget t) {
    writeToNoIdnt(t, '\n');
    setIdnt(t, 2);
  }

  void writeIndent(WriteTarget t) { writeToNoIdnt(t, '\t'); }

  void writeDirective(WriteTarget t, Directive d) {
    bool idnted = d == Directive::DATA || d == Directive::TEXT;

    if (idnted)
      setIdnt(t, 0);
    writeTo(t, d.toString());
    writeTo(t, ' ');
    if (idnted) {
      writeNewline(t);
      setIdnt(t, 2);
    } else {
      setIdnt(t, 0);
    }
  }

  void writeQuotedString(WriteTarget t, std::string &s) {
    writeToNoIdnt(t, '\"' + s + "\"");
    writeNewline(t);
  }

  void writeInt(WriteTarget t, int i) {
    writeToNoIdnt(t, "" + std::to_string(i) + ' ');
    writeNewline(t);
  }

  void writeComment(WriteTarget t, std::string &comment) {
    writeToNoIdnt(t, '\t');
    writeToNoIdnt(t, "# " + comment);
  }

  void writeCommentNl(WriteTarget t, std::string &comment) {
    setIdnt(WriteTarget::TEXT, 2);
    writeTo(t, "# " + comment);
    writeNewline(t);
  }

  void writeLabel(WriteTarget t, std::string &labelName) {
    setIdnt(t, 1);
    writeTo(t, labelName + ": ");
    setIdnt(t, 0);
  }

  void writeLabelNl(WriteTarget t, std::string &labelName) {
    setIdnt(t, 1);
    writeTo(t, labelName + ": ");
    writeNewline(t);
  }

  void writeLabelInlineNl(WriteTarget t, std::string &labelName) {
    setIdnt(t, 2);
    writeTo(t, labelName + ": ");
    writeNewline(t);
  }

  ///// these assume writing to text segment

  /** add */
  void writeAdd(Register rd, Register rs, Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "add " + rd.to_string() + ',' + rs.to_string() +
                                   ',' + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** add unsigned */
  void writeAddU(Register rd, Register rs, Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "addu " + rd.to_string() + ',' + rs.to_string() +
                                   ',' + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** add immediate */
  void writeAddI(Register rt, Register rs, int imm, std::string &comment) {
    writeTo(WriteTarget::TEXT, "addi " + rt.to_string() + ',' + rs.to_string() +
                                   ',' + std::to_string(imm));
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** add unsigned immediate */
  void writeAddIU(Register rt, Register rs, int imm, std::string &comment) {
    writeTo(WriteTarget::TEXT, "addiu " + rt.to_string() + ',' +
                                   rs.to_string() + ',' + std::to_string(imm));
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** bitwise and */
  void writeAnd(Register rd, Register rs, Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "and " + rd.to_string() + ',' + rs.to_string() +
                                   ',' + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** bitwise and immediate*/
  void writeAndI(Register rt, Register rs, int imm, std::string &comment) {
    writeTo(WriteTarget::TEXT, "andi " + rt.to_string() + ',' + rs.to_string() +
                                   ',' + std::to_string(imm));
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** branch if equal */
  void writeBeq(Register rt, Register rs, std::string &label,
                std::string &comment) {
    writeTo(WriteTarget::TEXT,
            "beq " + rt.to_string() + ',' + rs.to_string() + ',' + label);
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** branch if greater or equal to zero */
  void writeBgez(Register rt, std::string &label, std::string &comment) {
    writeTo(WriteTarget::TEXT, "bgez " + rt.to_string() + ',' + label);
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** branch if greater than to zero */
  void writeBgtz(Register rt, std::string &label, std::string &comment) {
    writeTo(WriteTarget::TEXT, "bgtz " + rt.to_string() + ',' + label);
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** branch if less than zero */
  void writeBltz(Register rt, std::string &label, std::string &comment) {
    writeTo(WriteTarget::TEXT, "bltz " + rt.to_string() + ',' + label);
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** branch if less than or equal to zero */
  void writeBlez(Register rt, std::string &label, std::string &comment) {
    writeTo(WriteTarget::TEXT, "blez " + rt.to_string() + ',' + label);
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** branch if not equal */
  void writeBne(Register rt, Register rs, std::string &label,
                std::string &comment) {
    writeTo(WriteTarget::TEXT,
            "bne " + rt.to_string() + ',' + rs.to_string() + ',' + label);
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** division with overflow, divide then set LO to quotient and HI to remainder
   */
  void writeDiv(Register rt, Register rs, std::string &comment) {
    writeTo(WriteTarget::TEXT, "div " + rt.to_string() + ',' + rs.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** division unsigned with overflow, divide then set LO to quotient and HI to
   * remainder */
  void writeDivU(Register rt, Register rs, std::string &comment) {
    writeTo(WriteTarget::TEXT, "divu " + rt.to_string() + ',' + rs.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** jump to label */
  void writeJ(std::string &target, std::string &comment) {
    writeTo(WriteTarget::TEXT, "j " + target);
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** jump to label and link (set $ra to PC)*/
  void writeJal(std::string &target, std::string &comment) {
    writeTo(WriteTarget::TEXT, "jal " + target);
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** jump to address in t1 and link (set $ra to PC)*/
  void writeJalr(Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "jalr " + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** jump to register */
  void writeJr(Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "jr " + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** load byte (sign-extend) into lower 8 bits */
  void writeLb(Register rt, Register rs, int offset, std::string &comment) {
    writeTo(WriteTarget::TEXT, "lb " + rt.to_string() + ',' +
                                   std::to_string(offset) + '(' +
                                   rs.to_string() + ')');
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** load byte (zero-extend) into lower 8 bits */
  void writeLbU(Register rt, Register rs, int offset, std::string &comment) {
    writeTo(WriteTarget::TEXT, "lbu " + rt.to_string() + ',' +
                                   std::to_string(offset) + '(' +
                                   rs.to_string() + ')');
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** load word (sign-extend) into target */
  void writeLw(Register rt, Register rs, int offset, std::string &comment) {
    writeTo(WriteTarget::TEXT, "lw " + rt.to_string() + ',' +
                                   std::to_string(offset) + '(' +
                                   rs.to_string() + ')');
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** load byte (zero-extend) into target */
  void writeLwU(Register rt, Register rs, int offset, std::string &comment) {
    writeTo(WriteTarget::TEXT, "lwu " + rt.to_string() + ',' +
                                   std::to_string(offset) + '(' +
                                   rs.to_string() + ')');
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** move from hi */
  void writeMfhi(Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "mfhi " + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** move from lo */
  void writeMflo(Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "mflo " + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** multiplication  (set HI to higher 32 bits, LO to lower 32 bits)*/
  void writeMult(Register rt, Register rs, std::string &comment) {
    writeTo(WriteTarget::TEXT, "mult " + rt.to_string() + ',' + rs.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** multiplication unsigned (set HI to higher 32 bits, LO to lower 32 bits)*/
  void writeMultU(Register rt, Register rs, std::string &comment) {
    writeTo(WriteTarget::TEXT,
            "multu " + rt.to_string() + ',' + rs.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** no op */
  void writeNop(std::string &comment) {
    writeTo(WriteTarget::TEXT, "nop ");
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** bitwise nor */
  void writeNor(Register rd, Register rs, Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "nor " + rd.to_string() + ',' + rs.to_string() +
                                   ',' + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** bitwise or */
  void writeOr(Register rd, Register rs, Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "or " + rd.to_string() + ',' + rs.to_string() +
                                   ',' + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** bitwise or immediate*/
  void writeOrI(Register rt, Register rs, int imm, std::string &comment) {
    writeTo(WriteTarget::TEXT, "ori " + rt.to_string() + ',' + rs.to_string() +
                                   ',' + std::to_string(imm));
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** store byte */
  void writeSb(Register rt, Register rs, int offset, std::string &comment) {
    writeTo(WriteTarget::TEXT, "sb " + rt.to_string() + ',' +
                                   std::to_string(offset) + '(' +
                                   rs.to_string() + ')');
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** shift left logical  */
  void writeSll(Register rt, Register rs, int imm, std::string &comment) {
    writeTo(WriteTarget::TEXT, "sll " + rt.to_string() + ',' + rs.to_string() +
                                   ',' + std::to_string(imm));
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** set rd to 1 if rs less than rt and 0 otherwise */
  void writeSlt(Register rd, Register rs, Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "slt " + rd.to_string() + ',' + rs.to_string() +
                                   ',' + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** set rd to 1 if rs less than rt and 0 otherwise using unsigned comparison*/
  void writeSltU(Register rd, Register rs, Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "sltu " + rd.to_string() + ',' + rs.to_string() +
                                   ',' + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** set rt to 1 if rs less than imm and 0 otherwise */
  void writeSltI(Register rt, Register rs, int imm, std::string &comment) {
    writeTo(WriteTarget::TEXT, "slti " + rt.to_string() + ',' + rs.to_string() +
                                   ',' + std::to_string(imm));
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** set rt to 1 if rs less than imm and 0 otherwise using unsigned
   * comparison*/
  void writeSltIU(Register rt, Register rs, int imm, std::string &comment) {
    writeTo(WriteTarget::TEXT, "sltiu " + rt.to_string() + ',' +
                                   rs.to_string() + ',' + std::to_string(imm));
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** shift right arithmetic (sign extended) */
  void writeSra(Register rt, Register rs, int imm, std::string &comment) {
    writeTo(WriteTarget::TEXT, "sra " + rt.to_string() + ',' + rs.to_string() +
                                   ',' + std::to_string(imm));
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** shift right logical */
  void writeSrl(Register rd, Register rs, Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "srl " + rd.to_string() + ',' + rs.to_string() +
                                   ',' + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** substract */
  void writeSub(Register rd, Register rs, Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "sub " + rd.to_string() + ',' + rs.to_string() +
                                   ',' + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** substract unsigned*/
  void writeSubU(Register rd, Register rs, Register rt, std::string &comment) {
    writeTo(WriteTarget::TEXT, "subu " + rd.to_string() + ',' + rs.to_string() +
                                   ',' + rt.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** store word */
  void writeSw(Register rt, Register rs, int offset, std::string &comment) {
    writeTo(WriteTarget::TEXT, "sw " + rt.to_string() + ',' +
                                   std::to_string(offset) + '(' +
                                   rs.to_string() + ')');
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** syscall */
  void writeSyscall(std::string &comment) {
    writeTo(WriteTarget::TEXT, "syscall ");
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** xor logical  */
  void writeXor(Register rd, Register rt, Register rs, std::string &comment) {
    writeTo(WriteTarget::TEXT, "xor " + rd.to_string() + ',' + rt.to_string() +
                                   ',' + rs.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** xor logical immediate  */
  void writeXorI(Register rd, Register rt, Register rs, std::string &comment) {
    writeTo(WriteTarget::TEXT, "xori " + rd.to_string() + ',' + rt.to_string() +
                                   ',' + rs.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** store immediate in upper 2 bytes of rt */
  void writeLui(Register rt, int imm, std::string &comment) {
    writeTo(WriteTarget::TEXT,
            "lui " + rt.to_string() + ',' + std::to_string(imm));
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  // PSEUDO INSTRUCTIONS

  void writeLa(Register rt, std::string &label, std::string &comment) {
    writeTo(WriteTarget::TEXT, "la " + rt.to_string() + ',' + label);
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  void writeMove(Register rt, Register rs, std::string &comment) {
    writeTo(WriteTarget::TEXT, "move " + rt.to_string() + ',' + rs.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** set t1 to 1 if rest is equal or to 0 otherwise */
  void writeSeq(Register rd, Register rt, Register rs, std::string &comment) {
    writeTo(WriteTarget::TEXT, "seq " + rd.to_string() + ',' + rt.to_string() +
                                   ',' + rs.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** set t1 to 1 if rest is greater equal or to 0 otherwise */
  void writeSge(Register rd, Register rt, Register rs, std::string &comment) {
    writeTo(WriteTarget::TEXT, "sge " + rd.to_string() + ',' + rt.to_string() +
                                   ',' + rs.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** set t1 to 1 if rest is greater than 0 otherwise */
  void writeSgt(Register rd, Register rt, Register rs, std::string &comment) {
    writeTo(WriteTarget::TEXT, "sgt " + rd.to_string() + ',' + rt.to_string() +
                                   ',' + rs.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** set t1 to 1 if rest is less than or equal or to 0 otherwise */
  void writeSle(Register rd, Register rt, Register rs, std::string &comment) {
    writeTo(WriteTarget::TEXT, "sle " + rd.to_string() + ',' + rt.to_string() +
                                   ',' + rs.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** set t1 to 1 if rest is less than or equal or to 0 otherwise */
  void writeSne(Register rd, Register rt, Register rs, std::string &comment) {
    writeTo(WriteTarget::TEXT, "sne " + rd.to_string() + ',' + rt.to_string() +
                                   ',' + rs.to_string());
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** branch if greater or equal  */
  void writeBge(Register rd, int imm, std::string &label,
                std::string &comment) {
    writeTo(WriteTarget::TEXT,
            "bge " + rd.to_string() + ',' + std::to_string(imm) + ',' + label);
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** branch if less than  */
  void writeBlt(Register rd, int imm, std::string &label,
                std::string &comment) {
    writeTo(WriteTarget::TEXT,
            "blt " + rd.to_string() + ',' + std::to_string(imm) + ',' + label);
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

  /** branch equal  */
  void writeBeq(Register rd, int imm, std::string &label,
                std::string &comment) {
    writeTo(WriteTarget::TEXT,
            "beq " + rd.to_string() + ',' + std::to_string(imm) + ',' + label);
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }
  /** unconditional branch */
  void writeB(std::string &label, std::string &comment) {
    writeTo(WriteTarget::TEXT, "b " + label);
    writeComment(WriteTarget::TEXT, comment);
    writeNewline(WriteTarget::TEXT);
  }

private:
  std::string dataSection;
  std::string textSection;

  int idntLvlDtaSection = 0;
  int idntLvlTxtSection = 0;
};