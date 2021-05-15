#ifndef SYNTAXPARSER_H
#define SYNTAXPARSER_H

#include "syntaxitem.h"
#include <string>
#include <vector>
#include <unordered_map>

class SyntaxParser {
    virtual ParseItem parse(const std::string &input, size_t startIndex) = 0;

  public:
    virtual ~SyntaxParser() = 0;

    virtual void updateInfo(const std::vector<std::string> &content);
    ParseItem performParse(const std::string &input, size_t startIndex);
};

class CppParser: public SyntaxParser {
    std::unordered_map<char, std::vector<std::string>> keywords {
      {'a', std::vector<std::string>{"auto"}},
      {'b', std::vector<std::string>{"bool", "break"}},
      {'c', std::vector<std::string>{
        "case",
        "catch",
        "char",
        "class",
        "concept",
        "const",
        "continue"
      }},
      {'d', std::vector<std::string>{
        "default",
        "do",
        "double",
        "dynamic_cast"
      }},
      {'e', std::vector<std::string>{
        "else",
        "enum",
        "explicit",
        "extern"
      }},
      {'f', std::vector<std::string>{
        "false",
        "float",
        "for",
        "friend"
      }},
      {'g', std::vector<std::string>{"goto"}},
      {'i', std::vector<std::string>{
        "if",
        "inline",
        "int"
      }},
      {'l', std::vector<std::string>{"long"}},
      {'m', std::vector<std::string>{"mutable"}},
      {'n', std::vector<std::string>{
        "namespace",
        "new",
        "noexcept",
        "not",
        "nullptr"
      }},
      {'o', std::vector<std::string>{"operator", "or"}},
      {'p', std::vector<std::string>{
        "private",
        "protected",
        "public"
      }},
      {'r', std::vector<std::string>{
        "register",
        "reinterpret_cast",
        "return"
      }},
      {'s', std::vector<std::string>{
        "short",
        "signed",
        "sizeof",
        "static",
        "static_cast"
        "struct",
        "switch"
      }},
      {'t', std::vector<std::string>{
        "template",
        "this",
        "throw",
        "try",
        "typedef",
        "typename"
      }},
      {'u', std::vector<std::string>{
        "union",
        "unsigned",
        "using"
      }},
      {'v', std::vector<std::string>{"virtual", "void"}},
      {'w', std::vector<std::string>{"while"}}
    };

    std::vector<std::string> preprocessorRegex{
      "#define [a-zA-Z][a-zA-Z0-9]*",
      "#define [a-zA-Z][a-zA-Z0-9]* ([a-zA-Z]|[0-9])+",
      "#(el)?if ([a-zA-Z][a-zA-Z0-9]*|[0-9]+) (==|<=|>=|>|<) ([a-zA-Z][a-zA-Z0-9]*|[0-9]+)",
      "#else",
      "#endif",
      "#if(n)?def [a-zA-Z][a-zA-Z0-9]*",
      "#include <[a-zA-Z][a-zA-Z0-9]*>"
    };

    std::vector<std::string> contextKeywordsRegex{
      " *(class|struct) [a-zA-Z][a-zA-Z0-9_]* final( .+)?",
      " *[a-zA-Z][a-zA-Z0-9_]* [a-zA-Z][a-zA-Z0-9]*(.*) final;",
      " *[a-zA-Z][a-zA-Z0-9_]* [a-zA-Z][a-zA-Z0-9]*(.*) override;"
    };

    std::vector<std::string> identifiers;

    bool matchBegin(std::string input, const std::string &line, size_t startIndex);

    ParseItem parse(const std::string &input, size_t startIndex) override;

  public:
    void updateInfo(const std::vector<std::string> &content) override;
};

class DefaultParser: public SyntaxParser {
  ParseItem parse(const std::string &input, size_t startIndex) override;
};

#endif
