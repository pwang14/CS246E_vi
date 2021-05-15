#ifndef REVERSEGAPBUFFER_H
#define REVERSEGAPBUFFER_H

#include "filecontent.h"
#include <string>
#include <vector>
#include <deque>

class ReverseGapBuffer: public AbstractFileContent {
    std::vector<std::string> before;
    std::deque<std::string> after;

    size_t lineIndex;
    size_t charIndex;

  public:
    ReverseGapBuffer();
    ReverseGapBuffer(const std::string &fname);

    CursorPos getCursorPos() override;

    size_t getNumLines() override;
    size_t getNumChars() override;

    CursorPos findNext(std::string input) override;
    CursorPos findPrevious(std::string input) override;

    void moveUp() override;
    void moveUp(size_t amount) override;

    void moveDown() override;
    void moveDown(size_t amount) override;

    void moveLeft() override;
    void moveLeft(size_t amount) override;

    void moveRight() override;
    void moveRight(size_t amount) override;

    void insertAfter(char c) override;
    void insertAfter(std::string input) override;

    void insertBefore(char c) override;
    void insertBefore(std::string input) override;

    void addLineAbove() override;
    void addLineBelow() override;
    void insertNewLine() override;

    void removeAfter() override;
    void removeAfter(size_t amount) override;

    void removeBefore() override;
    void removeBefore(size_t amount) override;

    void removeLine() override;

    std::string getLine() override;
    std::string getLineRelative(int offset) override;
    std::string getLine(size_t index) override;

    std::vector<std::string> getLines(int num) override;

    std::vector<std::string> getContent() override;
};

#endif
