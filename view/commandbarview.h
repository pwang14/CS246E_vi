#ifndef COMMANDBARVIEW_H
#define COMMANDBARVIEW_H

#include "../view.h"
#include <ncurses.h>
#include <string>

class VmState;

class AbstractEditorState;

class CommandBarView: public View {
    const size_t CURSOR_INFO_OFFSET = 20;

    VmState *vmstate;
    AbstractEditorState *editor;

    int width;

    size_t cursorLine;
    size_t cursorChar;

    WINDOW *win;

    void printString(std::string input);
    void printCursor(size_t lineIndex, size_t charIndex, std::string overwrite = "");

  public:
    CommandBarView(VmState *vmstate, int width, int starty, int startx = 0);

    void update(bool forFile) override;
    void updateCursor(bool forFile, size_t lineIndex, size_t charIndex, bool setScreen = false) override;
    
    void displayView(bool forFile) override;
};

#endif
