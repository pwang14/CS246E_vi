#include "syntaxparser.h"
#include <algorithm>
#include <regex>
//#include <boost/regex.hpp>

using namespace std;
//using namespace boost;

//===== SyntaxParser =====

SyntaxParser::~SyntaxParser() {}

void SyntaxParser::updateInfo(const vector<string> &content) {}

ParseItem SyntaxParser::performParse(const string &input, size_t startIndex) { return parse(input, startIndex); }

//===== DefaultParser ======

ParseItem DefaultParser::parse(const string &input, size_t startIndex) {
    return ParseItem{SyntaxItem::ELSE, input.length() - startIndex};
}

//===== CppParser =====

void CppParser::updateInfo(const vector<string> &content) {
    identifiers.clear();

    regex reg(" *[a-zA-Z][a-zA-Z0-9_]* [a-zA-Z][a-zA-Z0-9_]*( = .+|\\{.*\\}|\\(.*\\))?;");

    for (auto &i : content) {
        if (regex_match(i, reg)) {
            size_t start = 0;
            while (start < i.length() && i[start] == ' ') ++start;

            size_t typeEnd = i.find(' ', start);
            if (typeEnd != string::npos && typeEnd + 1 < i.length() && i[typeEnd + 1] != ' ') {
                size_t nextSpace = i.find(' ', typeEnd + 1);
                size_t nextParenth = i.find('{', typeEnd + 1);
                size_t nextBracket = i.find('(', typeEnd + 1);
                size_t idEnd = min(nextSpace, min(nextParenth, nextBracket));

                if (idEnd != string::npos) {
                    if (i[idEnd] == '{' || '(') --idEnd;
                    identifiers.push_back(i.substr(typeEnd + 1, idEnd - typeEnd));
                }
                else identifiers.push_back(i.substr(typeEnd + 1, i.length() - 1 - typeEnd));
            }
        }
    }
}

bool CppParser::matchBegin(string input, const string &line, size_t startIndex) {
    if (input.length() > line.length() - startIndex) return false;
    for (size_t i=0; i < input.length(); ++i) {
        if (input[i] != line[startIndex + i]) return false;
    }
    return true;
}

ParseItem CppParser::parse(const string &input, size_t startIndex) {
    if (input.length()) {
        if (input[0] == '#') {
            for (auto i : preprocessorRegex) {
                regex reg(i);
                if (regex_match(input, reg)) {
                    return ParseItem{SyntaxItem::PREPROCESSOR, input.length() - startIndex};
                }
            }
            return ParseItem{SyntaxItem::ELSE, input.length() - startIndex};
        }
        else if (input[startIndex] == '/' && input.length() > 1) {
            if (input[startIndex + 1] == '/') {
                return ParseItem{SyntaxItem::COMMENT, input.length() - startIndex};
            }
        }
        else if (isdigit(input[startIndex])) {
            return ParseItem{SyntaxItem::NUMERIC, 1};
        }
        else if (input[startIndex] == '"') {
            size_t endIndex = startIndex + 1;
            while (endIndex < input.length() && input[endIndex] != '"') ++endIndex;
            return ParseItem{SyntaxItem::STRING, endIndex - startIndex + 1};
        }
        else if (keywords.count(input[startIndex])) {
            vector<string> possibleKeywords = keywords[input[startIndex]];
            for (auto i : possibleKeywords) {
                if (matchBegin(i, input, startIndex)) {
                    bool valid = true;
                    if (startIndex && isalpha(input[startIndex - 1])) valid = false;
                    if (startIndex < input.length() - 1 && isalpha(input[startIndex + 1])) valid = false;
                    if (valid) return ParseItem{SyntaxItem::KEYWORD, i.length()};
                }
            }
        }

        if (matchBegin("final", input, startIndex) || matchBegin("override", input, startIndex)) {
            for (auto i : contextKeywordsRegex) {
                regex reg(i);
                if (regex_match(input, reg)) {
                    if (matchBegin("final", input, startIndex)) return ParseItem{SyntaxItem::KEYWORD, 5};
                    else return ParseItem{SyntaxItem::KEYWORD, 8};
                }
            }
        }

        for (auto &i : identifiers) {
            if (matchBegin(i, input, startIndex)) {
                bool valid = true;
                if (startIndex && isalpha(input[startIndex - 1])) valid = false;
                if (startIndex < input.length() - 1 && isalpha(input[startIndex + 1])) valid = false;
                if (valid) return ParseItem{SyntaxItem::ID, i.length()};
            }
        }
    }
    
    return ParseItem{SyntaxItem::ELSE, 1};
}
