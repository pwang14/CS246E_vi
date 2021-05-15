#include "commandswitch.h"
#include "../../model.h"
#include "../normal/search.h"
#include <ncurses.h>

using namespace std;

matchInfo CommandSwitch::matches(const vector<int> &input) {
    if (input.size() == 3) {
        if (input[2] == 27) {
            lastInput = input;
            return matchInfo{true, true, Mode::NORMAL};
        }
        else if (input[2] == 10) {
            lastInput = input;
            return matchInfo{true, true, Mode::NORMAL};
        }
        else if (input[2] == KEY_BACKSPACE && input[0] == 1) {
            lastInput = input;
            return matchInfo{true, true, Mode::NORMAL};
        }
    }
    return matchInfo{false};
}

bool CommandSwitch::partialMatches(const vector<int> &input) { return false; }

void CommandSwitch::action(VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractEditorState *editor = vmstate->getEditor();

    if (lastInput[2] == 27) {
        editor->setCommandBar("");
        editor->setCursor(0);
        editor->setCursorOn(false);
    }
    else if (lastInput[2] == KEY_BACKSPACE && lastInput[0] == 1) {
        editor->setCommandBar("");
        editor->setCursor(0);
        editor->setCursorOn(false);
    }
    else if (lastInput[2] == 10 && editor->getCommandBar()[0] == ':') {
        string line = editor->getCommandBar();
        
        editor->setCommandBar("");
        editor->setCursor(0);
        editor->setCursorOn(false);
        
        int a = -1;
        try {
            a = stoi(line.substr(1));
        }
        catch (...) {}
        
        if (a > 0) {
            file->moveCursor(static_cast<size_t>(a-1), 0);
        }
        else if (line.length() == 2) {
            if (line[1] == 'w') file->saveFile();
            else if (line[1] == 'q') {
                if (file->changeMade()) {
                    editor->setCommandBar("E37: No write since last change (add ! to override)");
                }
                else vmstate->exit();
            }
            else if (line[1] == 'r') {
                const vector<string> &content = file->getContent();
                LineChange change;
                Diff diff;
                CursorPos tempPos = file->getCursorPos();
                for (auto i : content) {
                    change = LineChane{i, true, ++tempPos.lineIndex, 0, true};
                    diff.changes.push_back(change);
                    file->addLineBelow();
                    file->lmoveCursor(1);
                    file->insertBefore(i);
                }
                vmstate->getHistory()->addDiff(diff);
                file->updateContent();
                tempPos = file->getCursorPos();
                file->setVirtualPos(tempPos.lineIndex, tempPos.charIndex);
                vmstate->updateViews(true);
            }
            else if (line[1] == '0') {
                file->moveCursor(0, 0);
            }
            else if (line[1] == '$') {
                size_t numLines = file->getNumLines();
                file->moveCursor(numLines-1, 0);
            }
        }
        else if (line.length() == 3) {
            if (line[1] == 'w' && line[2] == 'q') {
                file->saveFile();
                vmstate->exit();
            }
            else if (line[1] == 'q' && line[2] == '!') vmstate->exit();
        }
    }
    else if (lastInput[2] == 10 && editor->getCommandBar()[0] == '/') {
        editor->setCursor(0);
        editor->setCursorOn(false);

        string s = editor->getCommandBar();
        editor->setSearchTerm(s.substr(1));
        editor->setForwardSearch(true);

        vector<int> temp{static_cast<int>('n')};
        Search::searchActionHelper(temp, vmstate);
    }
    else if (lastInput[2] == 10 && editor->getCommandBar()[0] == '?') {
        editor->setCursor(0);
        editor->setCursorOn(false);
        
        string s = editor->getCommandBar();
        editor->setSearchTerm(s.substr(1));
        editor->setForwardSearch(false);

        vector<int> temp{static_cast<int>('N')};
        Search::searchActionHelper(temp, vmstate);
    }
    
    CursorPos pos = file->getCursorPos();

    vmstate->updateViews(false);
    vmstate->updateViewsCursor(true, pos.lineIndex, pos.charIndex);
}
