#include "search.h"
#include "../../model.h"

using namespace std;

matchInfo Search::matches(const vector<int> &input) {
    if (input.size() == 1 && singleValid.count(input[0])) {
        lastInput = input;
        return matchInfo{true};
    }
    else if (input.size() == 2 && (input[0] == 'f' || input[0] == 'F')) {
        if (input[1] < 128) {
            lastInput = input;
            return matchInfo{true};
        }
    }
    return matchInfo{false};
}

bool Search::partialMatches(const vector<int> &input) {
    return (input.size() == 1 && (input[0] == 'f' || input[0] == 'F'));
}

void Search::searchForward(string s, VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractEditorState *editor = vmstate->getEditor();

    file->cmoveCursor(1);
    CursorPos start = file->getCursorPos();
    CursorPos after = file->findNext(s);

    if (after.lineIndex == start.lineIndex && after.charIndex == start.charIndex) {
        file->moveCursor(0, 0);
        after = file->findNext(s);

        if (!after.lineIndex && !after.charIndex) {
            editor->setCommandBar("E486: Pattern not found: " + s);
            file->moveCursor(start.lineIndex, start.charIndex);
        }
        else {
            editor->setCommandBar("search hit BOTTOM, continuing at TOP");
            file->moveCursor(after.lineIndex, after.charIndex);
        }
    }
    else {
        file->moveCursor(after.lineIndex, after.charIndex);
    }
}

void Search::searchBackward(string s, VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractEditorState *editor = vmstate->getEditor();

    CursorPos start = file->getCursorPos();
    if (start.charIndex) file->cmoveCursor(-1);
    else if (start.lineIndex) {
        file->lmoveCursor(-1);
        string line = file->getLine();
        file->cmoveCursor(line.length());
    }
    start = file->getCursorPos();
    CursorPos after = file->findPrevious(s);

    if (after.lineIndex == start.lineIndex && after.charIndex == start.charIndex) {
        size_t numLines = file->getNumLines();
        size_t length = file->getContent().back().length();

        file->moveCursor(numLines - 1, length);
        after = file->findPrevious(s);

        if (after.lineIndex == numLines - 1 && after.charIndex == length) {
            editor->setCommandBar("E486: Pattern not found: " + s);
            file->moveCursor(start.lineIndex, start.charIndex);
        }
        else {
            editor->setCommandBar("search hit TOP, continuing at BOTTOM");
            file->moveCursor(after.lineIndex, after.charIndex);
        }
    }
    else {
        file->moveCursor(after.lineIndex, after.charIndex);
    }
}

void Search::searchActionHelper(const vector<int> &lastInput, VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractEditorState *editor = vmstate->getEditor();

    if (lastInput.size() == 1) {
        string s = editor->getSearchTerm();
        bool forwardSearch = editor->getForwardSearch();

        if (lastInput[0] != ';' && s != "") {
            if (forwardSearch) editor->setCommandBar("/" + s);
            else editor->setCommandBar("?" + s);

            if (lastInput[0] == 'n') {
                if (forwardSearch) Search::searchForward(s, vmstate);
                else Search::searchBackward(s, vmstate);
            }
            else if (lastInput[0] == 'N') {
                if (forwardSearch) Search::searchBackward(s, vmstate);
                else Search::searchForward(s, vmstate);
            }
        }
        else if (lastInput[0] == ';') {
            int c = static_cast<int>(editor->getLastFChar());
            if (c) {
                int temp = 'f';
                if (!editor->getLastFForward()) temp = 'F';

                vector<int> tempInput{temp, c};
                Search::searchActionHelper(tempInput, vmstate);
            }
        }
    }
    else if (lastInput.size() == 2 && lastInput[1] != 10 && lastInput[1] != 27) {
        if (lastInput[0] == 'f') {
            editor->setLastFFoward(true);
            char c = static_cast<char>(lastInput[1]);
            editor->setLastFChar(c);
            string s(1,c);

            file->cmoveCursor(1);
            CursorPos start = file->getCursorPos();
            CursorPos after = file->findNext(s);
            
            if (start.lineIndex == after.lineIndex) {
                file->moveCursor(after.lineIndex, after.charIndex);
            }
            else file->cmoveCursor(-1);
        }
        else if (lastInput[0] == 'F') {
            editor->setLastFFoward(false);
            char c = static_cast<char>(lastInput[1]);
            editor->setLastFChar(c);
            string s(1,c);

            CursorPos start = file->getCursorPos();
            if (start.charIndex) file->cmoveCursor(-1);
            else if (start.lineIndex) {
                file->lmoveCursor(-1);
                string line = file->getLine();
                file->cmoveCursor(line.length());
            }
            start = file->getCursorPos();
            CursorPos after = file->findPrevious(s); 

            if (start.lineIndex == after.lineIndex) {
                file->moveCursor(after.lineIndex, after.charIndex);
            }
            else {
                string line = file->getLine();
                if (start.charIndex == line.length()) {
                    file->lmoveCursor(1);
                }
                else file->cmoveCursor(1);
            }
        }
    }

    CursorPos pos = file->getCursorPos();

    if (file->getLine().length() && pos.charIndex == file->getLine().length()) {
        file->cmoveCursor(-1);
        --pos.charIndex;
    }
}

void Search::action(VmState *vmstate) {
    Search::searchActionHelper(lastInput, vmstate);

    AbstractFileState *file = vmstate->getCurrentFile();
    CursorPos pos = file->getCursorPos();

    vmstate->updateViews(false);
    vmstate->updateViewsCursor(true, pos.lineIndex, pos.charIndex);
}
