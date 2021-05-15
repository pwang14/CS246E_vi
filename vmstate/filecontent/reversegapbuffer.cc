#include "reversegapbuffer.h"
#include "../cursorpos.h"
#include <fstream>
#include <algorithm>
#include <cassert>

using namespace std;

//===== ReverseGapBuffer =====

ReverseGapBuffer::ReverseGapBuffer(): before{vector<string>{""}}, after{deque<string>{""}},
lineIndex{0}, charIndex{0} {}

ReverseGapBuffer::ReverseGapBuffer(const string &fname): before{vector<string>{""}}, lineIndex{0}, charIndex{0} {
    ifstream f{fname.c_str()};

    string line;
    while (getline(f, line)) {
        reverse(line.begin(), line.end());
        after.push_back(line);
    }

    if (after.empty()) after.push_back("");
}

CursorPos ReverseGapBuffer::getCursorPos() {
    assert(lineIndex == before.size()-1);
    assert(charIndex == before.back().length());
    
    return CursorPos{lineIndex, charIndex};
}

size_t ReverseGapBuffer::getNumLines() { return before.size() + after.size() - 1; }

size_t ReverseGapBuffer::getNumChars() { return before.back().length() + after.front().length(); }

CursorPos ReverseGapBuffer::findNext(string input) {
    size_t fLine = lineIndex;
    size_t fChar = charIndex;
    size_t offset = before.back().length();

    reverse(input.begin(), input.end());
    for (auto &i : after) {
        fChar = i.rfind(input);
        if (fChar != string::npos)
            return CursorPos{fLine, offset+i.length()-fChar-input.length()};
        
        ++fLine;
        offset = 0;
    }

    return CursorPos{lineIndex, charIndex};
}

CursorPos ReverseGapBuffer::findPrevious(string input) {
    size_t fLine = lineIndex;
    size_t fChar = charIndex;
    
    for (auto i = before.rbegin(); i != before.rend(); ++i) {
        fChar = i->rfind(input);
        if (fChar != string::npos) return CursorPos{fLine, fChar};
        
        if (fLine) --fLine;
    }

    return CursorPos{lineIndex, charIndex};
}

void ReverseGapBuffer::insertAfter(char c) { after.front().push_back(c); }

void ReverseGapBuffer::insertAfter(string input) {
    reverse(input.begin(), input.end());
    after.front().append(input);
}

void ReverseGapBuffer::insertBefore(char c) {
    before.back().push_back(c);
    ++charIndex;
}

void ReverseGapBuffer::insertBefore(string input) {
    before.back().append(input);
    charIndex += input.length();
}

void ReverseGapBuffer::addLineAbove() {
    string temp = before.back();
    before.back() = "";
    before.push_back(temp);
    ++lineIndex;
}

void ReverseGapBuffer::addLineBelow() {
    string temp = after.front();
    after.front() = "";
    after.push_front(temp);
}

void ReverseGapBuffer::insertNewLine() {
    before.push_back("");
    ++lineIndex;
    charIndex = 0;
}

void ReverseGapBuffer::removeAfter() {
    if (!after.front().empty()) after.front().pop_back();
    else if (after.size() > 1) after.pop_front();
}

void ReverseGapBuffer::removeAfter(size_t amount) {
    while (after.size() > 1 && amount > after.front().size()) {
        amount -= after.front().size() + 1;
        after.pop_front();
    }
    for (; after.front().length() >= 0 && amount > 0; --amount) after.front().pop_back();
}

void ReverseGapBuffer::removeBefore() {
    if (!before.back().empty()) {
        before.back().pop_back();
        --charIndex;
    }
    else if (before.size() > 1) {
        before.pop_back();
        charIndex = before.back().length()-1;
        --lineIndex;
    }
}

void ReverseGapBuffer::removeBefore(size_t amount) {
    while (before.size() > 1 && amount > before.back().size()) {
        amount -= before.back().size() + 1;
        before.pop_back();
        --lineIndex;
    }
    for (; before.back().size() > 0 && amount > 0; --amount) before.back().pop_back();
    charIndex = before.back().length();
}

void ReverseGapBuffer::removeLine() {
    if (getNumLines() == 1) {
        before.back() = "";
        after.front() = "";
    }
    else if (after.size() == 1) {
        before.pop_back();
        string line = before.back();
        reverse(line.begin(), line.end());
        after.front() = line;
        before.back() = "";
        --lineIndex;
    }
    else {
        before.back() = "";
        after.pop_front();
    }
    charIndex = 0;
}

void ReverseGapBuffer::moveUp() {
    if (before.size() > 1) {
        string temp = before.back();
        reverse(temp.begin(), temp.end());
        after.front().append(temp);
        before.pop_back();

        temp = before.back();
        reverse(temp.begin(), temp.end());
        after.push_front(temp);
        before.back() = "";
        
        --lineIndex;
        charIndex = 0;
    }
}

void ReverseGapBuffer::moveUp(size_t amount) {
    for (; amount > 0; --amount) moveUp();
}

void ReverseGapBuffer::moveDown() {
    if (after.size() > 1) {
        string temp = after.front();
        reverse(temp.begin(), temp.end());
        before.back().append(temp);
        after.pop_front();

        before.push_back("");

        ++lineIndex;
        charIndex = 0;
    }
}

void ReverseGapBuffer::moveDown(size_t amount) {
    for(; amount > 0; --amount) moveDown();
}

void ReverseGapBuffer::moveLeft() {
    if (before.back().length() > 0) {
        after.front().push_back(before.back().back());
        before.back().pop_back();

        --charIndex;
    }
}

void ReverseGapBuffer::moveLeft(size_t amount) {
    for (; amount > 0; --amount) moveLeft();
}

void ReverseGapBuffer::moveRight() {
    if (after.front().length() > 0) {
        before.back().push_back(after.front().back());
        after.front().pop_back();

        ++charIndex;
    }
}

void ReverseGapBuffer::moveRight(size_t amount) {
    for (; amount > 0; --amount) moveRight();
}

string ReverseGapBuffer::getLine() {
    string tempBefore = before.back();
    string tempAfter = after.front();
    reverse(tempAfter.begin(), tempAfter.end());
    
    return tempBefore + tempAfter;
}

string ReverseGapBuffer::getLineRelative(int offset) {
    if (offset == 0) return getLine();
    else if (offset < 0 && static_cast<size_t>(-offset) <= lineIndex) return before[lineIndex + offset];
    else if (offset > 0 && static_cast<size_t>(offset) < after.size()) {
        string temp = after[offset];
        reverse(temp.begin(), temp.end());
        return temp;
    }

    throw;
}

string ReverseGapBuffer::getLine(size_t index) {
    int current = static_cast<int>(lineIndex);
    int target = static_cast<int>(index);
    
    return getLineRelative(target - current);
}

vector<string> ReverseGapBuffer::getLines(int num) {
    vector<string> ret;

    if (num < 0) {
        for (int i=0; static_cast<size_t>(-i) <= lineIndex && i >= num; --i)
            ret.push_back(getLineRelative(i));
    }
    else if (num == 0) ret.push_back(getLine());
    else {
        for (int i=0; static_cast<size_t>(i) < after.size() && i <= num; ++i)
            ret.push_back(getLineRelative(i));
    }

    return ret;
}

vector<string> ReverseGapBuffer::getContent() {
    vector<string> ret;

    for (size_t i = 0; i < getNumLines(); ++i) ret.push_back(getLine(i));

    return ret;
}
