#include "replacement.h"
#include "../../model.h"
#include "../insert/insertion.h"
#include <ncurses.h>

using namespace std;

matchInfo Replacement::matches(const vector<int> &input) {
    if (input[0] != 27) {
        lastInput = input;
        return matchInfo{true};
    }
    return matchInfo{false};
}

bool Replacement::partialMatches(const vector<int> &input) { return false; }

void Replacement::action(VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractEditorState *editor = vmstate->getEditor();
    AbstractHistory *history = vmstate->getHistory();
    unordered_map<size_t, unordered_map<size_t, char>> &replacementMap = editor->getReplacementMap();

    string tempStr;
    if (lastInput[0] == KEY_BACKSPACE) tempStr = "I am the senate";
    else tempStr = "This is where the fun begins";

    editor->setCommandBar(tempStr);

    if (lastInput[0] < 128) {
        Insertion::insertionActionHelper(lastInput, vmstate);

        CursorPos pos = file->getCursorPos();
        string line = file->getLine();
        char c = line[pos.charIndex];
        string s(1, c);

        replacementMap[pos.lineIndex][pos.charIndex] = c;

        Diff &lastDiff = history->getUndo();
        LineChange change{s, false, pos.lineIndex, pos.charIndex};
        lastDiff.changes.push_back(change);

        file->remove(1);
    }
    else if (lastInput[0] == KEY_BACKSPACE) {
        CursorPos pos = file->getCursorPos();
        if (replacementMap.count(pos.lineIndex)) {
            if (replacementMap[pos.lineIndex].count(pos.charIndex)) {
                char c = replacementMap[pos.lineIndex][pos.charIndex];
                string s(1, c);

                Insertion::insertionActionHelper(lastInput, vmstate);
                CursorPos pos = file->getCursorPos();

                Diff &lastDiff = history->getUndo();
                LineChane change{s, true, pos.lineIndex, pos.charIndex};
                lastDiff.changes.push_back(change);

                file->insertAfter(s);
            }
            else {
                vector<int> temp{KEY_LEFT};
                Insertion::insertionActionHelper(temp, vmstate); 
            }
        }
        else {
            vector<int> temp{KEY_LEFT};
            Insertion::insertionActionHelper(temp, vmstate);
        }
    }
    else if (lastInput[0] == KEY_LEFT) {
        Insertion::insertionActionHelper(lastInput, vmstate);
    }
    else if (lastInput[0] == KEY_RIGHT) {
        Insertion::insertionActionHelper(lastInput, vmstate);
    }
    else if (lastInput[0] == KEY_UP) {
        Insertion::insertionActionHelper(lastInput, vmstate);
    }
    else if (lastInput[0] == KEY_DOWN) {
        Insertion::insertionActionHelper(lastInput, vmstate);
    }

    file->updateContent();
    CursorPos pos = file->getCursorPos();

    vmstate->updateViews(false);
    vmstate->updateViewsCursor(true, pos.lineIndex, pos.charIndex);
    vmstate->updateViews(true);
}
