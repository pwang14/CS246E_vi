#ifndef FILEVIEW_H
#define FILEVIEW_H

#include "../view.h"
#include "syntaxparser.h"
#include "colorer.h"
#include <memory>
#include <ncurses.h>

class VmState;

class AbstractFileState;

class FileView: public View {
    VmState *vmstate;
    AbstractFileState *file;

    int height, width;

    size_t startLine;
    size_t translatedStartLine;

    size_t cursorLine;
    size_t cursorChar;

    WINDOW *win;

    std::unique_ptr<SyntaxParser> parser;
    std::unique_ptr<Colorer> colorer;

    void clearLine(size_t index);
    void clearBlock(size_t index);

    void printString(std::string str, short color);
    void printSection(size_t start, size_t end);

    size_t translateLine(size_t lineIndex);
    size_t reverseTranslateLine(size_t lineIndex);
    
  public:
    FileView(VmState *vmstate, AbstractFileState *file, std::unique_ptr<SyntaxParser> &&parser,
    std::unique_ptr<Colorer> &&colorer, int height, int width, int starty = 0, int startx = 0);
    
    ~FileView();

    void update(bool forFile) override;
    void updateLine(size_t lineIndex, int previousLength) override;
    void updateBlock(size_t startIndex) override;
    void updateCursor(bool forFile, size_t lineIndex, size_t charIndex, bool setScreen = false) override;
    
    void displayView(bool forFile) override;
};

#endif
