#include "commandbarview.h"
#include "../model.h"
#include "../vmstate/editorstate.h"

#include <string>

using namespace std;

CommandBarView::CommandBarView(VmState *vmstate, int width, int starty, int startx):
vmstate{vmstate}, editor{vmstate->getEditor()}, width{width}, cursorLine{0}, cursorChar{0}, win{newwin(1, width, starty, startx)} {
    refresh();
    start_color();
    keypad(win, true);
}

void CommandBarView::printString(string input) {
    wmove(win, 0, 0);
    for (int i=0; i<width; ++i) waddch(win, ' ');
    wmove(win, 0, 0);
    wprintw(win, input.c_str());
}

void CommandBarView::printCursor(size_t lineIndex, size_t charIndex, string overwrite) {
    if (static_cast<size_t>(width) > CURSOR_INFO_OFFSET) {
        wmove(win, 0, width - CURSOR_INFO_OFFSET);
        if (!overwrite.length()) wprintw(win, "%d,%d", static_cast<int>(lineIndex), static_cast<int>(charIndex));
        else wprintw(win, "%d,%s", static_cast<int>(lineIndex), overwrite.c_str());
    }
}

void CommandBarView::update(bool forFile) {
    if (!forFile) {
        curs_set(0);
        printString(editor->getCommandBar());
        AbstractFileState *file = vmstate->getCurrentFile();
        CursorPos pos = file->getCursorPos();
        string line = file->getLine();
        if (line.length()) printCursor(pos.lineIndex + 1, pos.charIndex + 1);
        else printCursor(pos.lineIndex + 1, 0, "0-1");

        wmove(win, cursorLine, cursorChar);
    }
}

void CommandBarView::updateCursor(bool forFile, size_t lineIndex, size_t charIndex, bool setScreen) {
    if (!forFile) {
        curs_set(1);
        wmove(win, 0, charIndex);
    }
}

void CommandBarView::displayView(bool forFile) {
    if (!forFile) wrefresh(win);
}
