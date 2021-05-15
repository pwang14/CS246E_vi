#include "copypaste.h"
#include "../../model.h"
#include "movement.h"
#include "quickmove.h"

using namespace std;

matchInfo CopyPaste::matches(const vector<int> &input) {
    if (input.size() == 1 && singleValid.count(input[0])) {
        lastInput = input;
        return matchInfo{true};
    }
    else if (input.size() == 2 && partialValid.count(input[0])) {
        if (endValid.count(input[1]) || input[1] == input[0]) {
            lastInput = input;
            return matchInfo{true};
        }
    }
    return matchInfo{false};
}

bool CopyPaste::partialMatches(const vector<int> &input) {
    return (input.size() == 1 && partialValid.count(input[0]));
}

void CopyPaste::paste(VmState *vmstate, bool shifted) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractHistory *history = vmstate->getHistory();
    AbstractClipboard *clipboard = vmstate->getClipboard();

    CursorPos pos = file->getCursorPos();
    const vector<string> &lines = clipboard->getClipboard();
    if (!lines.empty()) {
        vector<LineChange> changes;
        size_t tempChar = pos.charIndex;
        
        if (shifted) {
            LineChange change{"", true, pos.lineIndex, pos.charIndex-1};
            changes.push_back(change);
        }

        if (lines.size() > 1) {
            LineChange initChange{"", true, pos.lineIndex, 0};
            changes.push_back(initChange);
            
            file->addLineBelow();
            file->lmoveCursor(1);

            if (lines[0] != "\n") {
                initChange = {" ", true, pos.lineIndex + 1, 0};
                changes.push_back(initChange);
                
                for (size_t i=0; i<lines.size()-1; ++i) {
                    LineChange change{lines[i], true, pos.lineIndex + i + 1, tempChar};
                    changes.push_back(change);
                    tempChar = 0;
                        
                    LineChange nlchange{"\n", true, pos.lineIndex + i + 1, 0, true};
                    changes.push_back(nlchange);

                    file->insertBefore(lines[i]);
                    file->insertNewLine();
                }
                
                size_t end = lines.size()-1;
                LineChange change{lines[end], true, pos.lineIndex + end + 1, 0};
                changes.push_back(change);

                file->insertBefore(lines[end]);
                file->moveCursor(pos.lineIndex + 1, 0);
            }
            else {
                LineChange initnl = {"\n", true, pos.lineIndex + 1, 0, true};
                changes.push_back(initnl);

                LineChange change{lines[1], true, pos.lineIndex + 1, 0};
                changes.push_back(change);

                file->insertBefore(lines[1]);
                file->moveCursor(pos.lineIndex + 1, 0);
            }
        }
        else {
            LineChange change{lines[0], true, pos.lineIndex, pos.charIndex};
            changes.push_back(change);

            file->insertBefore(lines[0]);
        }
        Diff diff{changes};
        history->addDiff(diff);
    }
}

void CopyPaste::copyMotion(const vector<int> &lastInput, VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractClipboard *clipboard = vmstate->getClipboard();

    if (lastInput[1] == 'j' || lastInput[1] == 'k') {
        CursorPos start = file->getCursorPos();
        vector<int> temp{lastInput[1]};
        MovementAction::movementActionHelper(temp, vmstate);
        CursorPos end = file->getCursorPos();

        if (start.lineIndex != end.lineIndex) {
            size_t min = start.lineIndex;
            if (end.lineIndex < min) min = end.lineIndex;
            
            file->moveCursor(min, 0);
            
            string s1 = file->getContent()[min];
            string s2 = file->getContent()[min+1];
            
            clipboard->clearClipboard();
            clipboard->appendToClipboard(s1);
            clipboard->appendToClipboard(s2);
        }

        file->moveCursor(start.lineIndex, start.charIndex);
    }
    else {
        CursorPos start = file->getCursorPos();
        vector<int> temp{lastInput[1]};
        MovementAction::movementActionHelper(temp, vmstate);
        QuickMovement::quickMovementActionHelper(temp, vmstate);
        if (lastInput[1] == '$') file->cmoveCursor(1);
        CursorPos end = file->getCursorPos();

        if (start.lineIndex != end.lineIndex) {
            CursorPos min = start;
            CursorPos max = end;
            if (end.lineIndex < start.lineIndex) {
                min = end;
                max = start;
            }

            string s1 = file->getContent()[min.lineIndex].substr(min.charIndex);
            string sf = file->getContent()[max.lineIndex].substr(0, max.charIndex);

            clipboard->clearClipboard();
            clipboard->appendToClipboard(s1);
            for (size_t i=1; i<max.lineIndex-min.lineIndex; ++i) {
                string temp = file->getContent()[min.lineIndex + i];
                clipboard->appendToClipboard(temp);
            }
            clipboard->appendToClipboard(sf);
        }
        else if (start.lineIndex == end.lineIndex && start.charIndex != end.charIndex) {
            CursorPos min = start;
            CursorPos max = end;
            if (end.charIndex < start.charIndex) {
                min = end;
                max = start;
            }

            string line = file->getContent()[min.lineIndex];
            string s = line.substr(min.charIndex, max.charIndex - min.charIndex);

            clipboard->clearClipboard();
            clipboard->appendToClipboard(s);
        }

        file->moveCursor(start.lineIndex, start.charIndex);
    }
}

void CopyPaste::action(VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractClipboard *clipboard = vmstate->getClipboard();

    if (lastInput.size() == 1) {
        if (lastInput[0] == 'p') {
            file->cmoveCursor(1);
            paste(vmstate, true);
        }
        else if (lastInput[0] == 'P') {
            paste(vmstate);
        }
    }
    else if (lastInput.size() == 2) {
        if (lastInput[0] == 'y') {
            if (lastInput[1] == 'y') {
                string line = file->getLine();

                clipboard->clearClipboard();
                clipboard->appendToClipboard("\n");
                clipboard->appendToClipboard(line);
            }
            else {
                copyMotion(lastInput, vmstate);
            }
        }
    }

    CursorPos pos = file->getCursorPos();
    file->setVirtualPos(pos.lineIndex, pos.charIndex);

    file->updateContent();
    vmstate->updateViews(false);
    vmstate->updateViewsCursor(true, pos.lineIndex, pos.charIndex);
    vmstate->updateViews(true);
}
