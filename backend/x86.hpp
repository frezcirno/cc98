#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include "backend.hpp"
#include "../type.hpp"

class X86Register : public Register
{
public:
  X86Register(const char* name)
    : name(name)
  {}

  const char* getName() const override
  {
    return name;
  }

public:
  static const Register *dil, *sil, *dl, *cl, *r8b, *r9b;
  static const Register *di, *si, *dx, *cx, *r8w, *r9w;
  static const Register *edi, *esi, *edx, *ecx, *r8d, *r9d;
  static const Register *rdi, *rsi, *rdx, *rcx, *r8, *r9;

private:
  const char* name;
};

class X86Writer
{
public:
  enum class WriteTarget
  {
    TEXT,
    DATA,
  };

  class Directive
  {
  public:
    enum _Value
    {
      ALIGN = 0,
      ASCIIZ,
      BYTE,
      DATA,
      SPACE,
      TEXT,
      WORD
    } value;

    Directive(_Value v)
      : value(v)
    {}

    bool operator==(const Directive& other) const
    {
      return value == other.value;
    }

    const char* toString()
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
      return DirectiveStrings[value];
    }
  };

public:
  /**
   * after writing to the buffer call this to "release" the program
   */
  void writeProgram(std::ostream& os)
  {
    os << dataSection;
    os << textSection;
  }

  void writeTo(WriteTarget t, const std::string& s)
  {
    if (t == WriteTarget::TEXT) {
      for (int i = 0; i < textIndent; i++) {
        textSection.append("\t");
      }
      textSection.append(s);
    } else {
      for (int i = 0; i < dataIndent; i++) {
        dataSection.append("\t");
      }
      dataSection.append(s);
    }
  }

  void writeTo(WriteTarget t, const char c)
  {
    char s[2];
    s[0] = c;
    s[1] = '\0';
    writeTo(t, s);
  }

  void writeToNoIndent(WriteTarget t, const std::string& s)
  {
    if (t == WriteTarget::TEXT) {
      textSection.append(s);
    } else {
      dataSection.append(s);
    }
  }

  void writeToNoIndent(WriteTarget t, const char s)
  {
    if (t == WriteTarget::TEXT) {
      textSection += s;
    } else {
      dataSection += s;
    }
  }

  void changeIndent(WriteTarget t, int diff)
  {
    if (t == WriteTarget::TEXT) {
      textIndent += diff;
    } else {
      dataIndent += diff;
    }
  }

  void setIndent(WriteTarget t, int val)
  {
    if (t == WriteTarget::TEXT) {
      textIndent = val;
    } else {
      dataIndent = val;
    }
  }

  int getIndent(WriteTarget t)
  {
    if (t == WriteTarget::TEXT) {
      return textIndent;
    } else {
      return dataIndent;
    }
  }

  void writeNewline(WriteTarget t)
  {
    writeToNoIndent(t, '\n');
    setIndent(t, 2);
  }

  void writeIndent(WriteTarget t)
  {
    writeToNoIndent(t, '\t');
  }

  void writeDirective(WriteTarget t, Directive d)
  {
    bool idnted = d == Directive::DATA || d == Directive::TEXT;

    if (idnted)
      setIndent(t, 0);
    writeTo(t, d.toString());
    writeTo(t, ' ');
    if (idnted) {
      writeNewline(t);
      setIndent(t, 2);
    } else {
      setIndent(t, 0);
    }
  }

  void writeQuotedString(WriteTarget t, const std::string& s)
  {
    writeToNoIndent(t, '\"' + s + "\"");
    writeNewline(t);
  }

  void writeInt(WriteTarget t, int i)
  {
    writeToNoIndent(t, "" + std::to_string(i) + ' ');
    writeNewline(t);
  }

  void writeComment(WriteTarget t, const std::string& comment)
  {
    writeToNoIndent(t, '\t');
    writeToNoIndent(t, "# " + comment);
  }

  void writeCommentNl(WriteTarget t, const std::string& comment)
  {
    setIndent(WriteTarget::TEXT, 2);
    writeTo(t, "# " + comment);
    writeNewline(t);
  }

  void writeLabel(WriteTarget t, const std::string& labelName)
  {
    setIndent(t, 1);
    writeTo(t, labelName + ": ");
    setIndent(t, 0);
  }

  void writeLabelNl(WriteTarget t, const std::string& labelName)
  {
    setIndent(t, 1);
    writeTo(t, labelName + ": ");
    writeNewline(t);
  }

  void writeLabelInlineNl(WriteTarget t, const std::string& labelName)
  {
    setIndent(t, 2);
    writeTo(t, labelName + ": ");
    writeNewline(t);
  }

  void emitLoad(Symbol* sym) {}

private:
  std::string dataSection;
  std::string textSection;

  int dataIndent = 0;
  int textIndent = 0;
};