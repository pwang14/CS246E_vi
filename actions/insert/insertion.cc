#include "insertion.h"
#include "../../model.h"
#include <ncurses.h>

using namespace std;

matchInfo Insertion::matches(const vector<int> &input) {
    if (input[0] != 27) {
        lastInput = input;
        return matchInfo{true};
    }
    return matchInfo{false};
}

bool Insertion::partialMatches(const vector<int> &input) { return false; }

void Insertion::insertionActionHelper(const vector<int> &lastInput, VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractHistory *history = vmstate->getHistory();
    AbstractEditorState *editor = vmstate->getEditor();
    
    CursorPos pos;
    CursorPos virtualPos;

    if (lastInput[0] < 128) {
        pos = file->getCursorPos();

        if (lastInput[0] != 10) {
            char c = static_cast<char>(lastInput[0]);
            string s(1, c);
            
            if (editor->previousInsertion()) {
                Diff &lastDiff = history->getUndo();
                LineChange lastChange = lastDiff.changes.back();

                if (lastChange.lineIndex == pos.lineIndex &&
                lastChange.charIndex + 1 == pos.charIndex && lastChange.added) {
                    lastDiff.changes.back().line.push_back(c);
                }
                else {
                    LineChange change{s, true, pos.lineIndex, pos.charIndex};
                    lastDiff.changes.push_back(change);
                }
            }
            else {
                LineChange change{s, true, pos.lineIndex, pos.charIndex};
                Diff diff{vector<LineChange>{change}};
                history->addDiff(diff);
                editor->setInsertion(true);
            }

            file->insertBefore(s);
        }
        else {
            LineChange change{"\n", true, pos.lineIndex, pos.charIndex, true};

            if (editor->previousInsertion()) history->getUndo().changes.push_back(change);
            else {
                Diff diff{vector<LineChange>{change}};
                history->addDiff(diff);
                editor->setInsertion(true);
            }

            file->insertNewLine();
        }

        pos = file->getCursorPos();
        file->setVirtualPos(pos.lineIndex, pos.charIndex);
    }
    else if (lastInput[0] == KEY_BACKSPACE) {
        pos = file->getCursorPos();

        LineChange change;

        if (!pos.lineIndex && !pos.charIndex) return;

        if (editor->previousInsertion()) {
            Diff &lastDiff = history->getUndo();
            LineChange lastChange = lastDiff.changes.back();

            if (pos.charIndex) {
                char c = file->getLine()[pos.charIndex - 1];
                if (lastChange.charIndex == pos.charIndex && !lastChange.added && !lastChange.lineChanged) {
                    lastChange.line = c + lastChange.line;
                    --lastChange.charIndex;
                }
                else {
                    string s(1, c);
                    LineChange change{s, false, pos.lineIndex, pos.charIndex - 1};
                    lastDiff.changes.push_back(change);
                }
            }
            else {
                string temp = file->getContent()[pos.lineIndex-1];
                LineChange change{"\n", false, pos.lineIndex - 1, temp.length(), true};
                lastDiff.changes.push_back(change);
            }
        }
        else {
            LineChange change;

            if (pos.charIndex) {
                char c = file->getLine()[pos.charIndex - 1];
                string s(1, c);
                change = {s, false, pos.lineIndex, pos.charIndex - 1};
            }
            else {
                string temp = file->getContent()[pos.lineIndex-1];
                change = {"\n", false, pos.lineIndex - 1, temp.length(), true};
            }
            
            Diff diff{vector<LineChange>{change}};
            history->addDiff(diff);
            editor->setInsertion(true);
        }

        file->remove();

        pos = file->getCursorPos();
        file->setVirtualPos(pos.lineIndex, pos.charIndex);
    }
    else if (lastInput[0] == KEY_LEFT) {
        file->cmoveCursor(-1);
        pos = file->getCursorPos();
        file->setVirtualPos(pos.lineIndex, pos.charIndex);
    }
    else if (lastInput[0] == KEY_RIGHT) {
        file->cmoveCursor(1);
        pos = file->getCursorPos();
        file->setVirtualPos(pos.lineIndex, pos.charIndex);
    }
    else if (lastInput[0] == KEY_UP) {
        virtualPos = file->getVirtualPos();
        if (virtualPos.lineIndex > 0) file->moveCursor(virtualPos.lineIndex-1, virtualPos.charIndex);
        file->setVirtualPos(file->getCursorPos().lineIndex, virtualPos.charIndex);
    }
    else if (lastInput[0] == KEY_DOWN) {
        virtualPos = file->getVirtualPos();
        if (virtualPos.lineIndex < file->getNumLines()-1) file->moveCursor(virtualPos.lineIndex+1, virtualPos.charIndex);
        file->setVirtualPos(file->getCursorPos().lineIndex, virtualPos.charIndex);
    }
}

void Insertion::action(VmState *vmstate) {
    insertionActionHelper(lastInput, vmstate);

    AbstractFileState *file = vmstate->getCurrentFile();
    file->updateContent();
    CursorPos pos = file->getCursorPos();

    vmstate->updateViews(false);
    vmstate->updateViewsCursor(true, pos.lineIndex, pos.charIndex);
    vmstate->updateViews(true);
}
