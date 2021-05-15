#ifndef FILECONTENT_H
#define FILECONTENT_H

#include "../cursorpos.h"
#include <string>
#include <vector>

struct CursorPos;

class AbstractFileContent {
  public:
    virtual ~AbstractFileContent() = 0;

    virtual CursorPos getCursorPos() = 0;

    virtual size_t getNumLines() = 0;
    virtual size_t getNumChars() = 0;

    virtual CursorPos findNext(std::string input) = 0;
    virtual CursorPos findPrevious(std::string input) = 0;

    virtual void moveUp() = 0;
    virtual void moveUp(size_t amount) = 0;

    virtual void moveDown() = 0;
    virtual void moveDown(size_t amount) = 0;

    virtual void moveLeft() = 0;
    virtual void moveLeft(size_t amount) = 0;

    virtual void moveRight() = 0;
    virtual void moveRight(size_t amount) = 0;

    virtual void insertAfter(char c) = 0;
    virtual void insertAfter(std::string input) = 0;

    virtual void insertBefore(char c) = 0;
    virtual void insertBefore(std::string input) = 0;

    virtual void addLineAbove() = 0;
    virtual void addLineBelow() = 0;
    virtual void insertNewLine() = 0;

    virtual void removeAfter() = 0;
    virtual void removeAfter(size_t amount) = 0;

    virtual void removeBefore() = 0;
    virtual void removeBefore(size_t amount) = 0;

    virtual void removeLine() = 0;

    virtual std::string getLine() = 0;
    virtual std::string getLineRelative(int offset) = 0;
    virtual std::string getLine(size_t index) = 0;

    virtual std::vector<std::string> getLines(int num) = 0;

    virtual std::vector<std::string> getContent() = 0;
};

#endif
