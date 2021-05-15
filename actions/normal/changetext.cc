#include "changetext.h"
#include "../../model.h"
#include "movement.h"
#include "quickmove.h"
#include "../insert/insertion.h"

using namespace std;

matchInfo ChangeText::matches(const vector<int> &input) {
    if (input.size() == 1 && singleValid.count(input[0])) {
        lastInput = input;
        if (input[0] == 's' || input[0] == 'S') return matchInfo{true, true, Mode::INSERT};
        else return matchInfo{true};
    }
    else if (input.size() == 2 && partialValid.count(input[0])) {
        if (input[0] == 'r') {
            lastInput = input;
            return matchInfo{true};
        }
        else if (endValid.count(input[1]) || input[1] == input[0]) {
            lastInput = input;
            if (input[0] == 'c') return matchInfo{true, true, Mode::INSERT};
            else return matchInfo{true};
        }
    }
    return matchInfo{false};
}

bool ChangeText::partialMatches(const vector<int> &input) {
    return (input.size() == 1 && partialValid.count(input[0]));
}

void ChangeText::removeCharAt(VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractHistory *history = vmstate->getHistory();
    CursorPos pos = file->getCursorPos();
    string line = file->getLine();

    if (line.length() && pos.charIndex < line.length()) {
        char c = line[pos.charIndex];
        string s(1, c);
        LineChange change{s, false, pos.lineIndex, pos.charIndex};
        Diff diff{vector<LineChange>{change}};
        history->addDiff(diff);
        file->remove(1);
    }
}

void ChangeText::removeLine(VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractHistory *history = vmstate->getHistory();

    CursorPos pos = file->getCursorPos();
    string s = file->getLine();

    if (pos.lineIndex < file->getNumLines() - 1) {
        string s = file->getLine();
        LineChange change{s, false, pos.lineIndex, 0, true};
        Diff diff{vector<LineChange>{change}};
        history->addDiff(diff);
    }
    else {
        string s = file->getLine();
        LineChange change{s, false, pos.lineIndex, 0};
        LineChange nlchange{"\n", false, pos.lineIndex - 1, file->getContent()[pos.lineIndex - 1].length(), true};
        Diff diff{vector<LineChange>{change, nlchange}};
        history->addDiff(diff);
    }
    file->deleteLine();
}

void ChangeText::removeMotion(const vector<int> &lastInput, VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractHistory *history = vmstate->getHistory();

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
            
            LineChange change1{s1, false, min, 0, true};
            LineChange change2{s2, false, min, 0, true};
            Diff diff{vector<LineChange>{change1, change2}};
            history->addDiff(diff);
            
            file->deleteLine();
            file->deleteLine();
        }
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

            if (max.lineIndex == min.lineIndex + 1) {
                vector<LineChange> tempChanges;
                string line = file->getContent()[min.lineIndex];
                string s = line.substr(min.charIndex);

                LineChange change{s, false, min.lineIndex, min.charIndex};
                Diff diff{vector<LineChange>{change}};
                history->addDiff(diff);

                file->moveCursor(min.lineIndex, min.charIndex);
                file->remove(s.length());
                file->cmoveCursor(-1);
            }
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

            LineChange change{s, false, min.lineIndex, min.charIndex};
            Diff diff{vector<LineChange>{change}};
            history->addDiff(diff);

            if (max.lineIndex == file->getNumLines()-1 &&
            max.charIndex == file->getContent()[max.lineIndex].length()-1) {
                ++max.charIndex;
                s.push_back(' ');
            }

            file->moveCursor(max.lineIndex, max.charIndex);
            file->remove(-s.length());
        }
    }
}

void ChangeText::action(VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractEditorState *editor = vmstate->getEditor();
    AbstractHistory *history = vmstate->getHistory();
    CursorPos pos;

    if (lastInput.size() == 1) {
        if (lastInput[0] == 's') {
            removeCharAt(vmstate);
            editor->setCommandBar("--INSERT--");
        }
        else if (lastInput[0] == 'x') {
            removeCharAt(vmstate);
        }
        else if (lastInput[0] == 'J') {
            pos = file->getCursorPos();
            string line = file->getLine();

            if (pos.lineIndex < file->getNumLines()-1) {
                LineChange change{"\n", false, pos.lineIndex, line.length(), true};
                Diff diff{vector<LineChange>{change}};
                history->addDiff(diff);

                file->moveCursor(pos.lineIndex+1, 0);
                file->remove(-1);
                file->insertAfter(" ");
            }
        }
        else if (lastInput[0] == 'S') {
            removeLine(vmstate);
            pos = file->getCursorPos();

            Diff &diff = history->getUndo();
            LineChange change{"\n", true, pos.lineIndex, 0, true};
            diff.changes.push_back(change);

            file->addLineAbove();
            file->moveCursor(pos.lineIndex, 0);

            editor->setCommandBar("--INSERT--");
        }
        else if (lastInput[0] == 'X') {
            pos = file->getCursorPos();
            if (pos.charIndex) {
                file->cmoveCursor(-1);
                removeCharAt(vmstate);
            }
        }
    }
    else if (lastInput.size() == 2) {
        if (lastInput[0] == 'r') {
            if (lastInput[1] < 128 && lastInput[1] != 27) {
                pos = file->getCursorPos();
                string line = file->getLine();
                if (line.length() && pos.charIndex < line.length()) {
                    removeCharAt(vmstate);
                    editor->setInsertion(true);
                    vector<int> temp{lastInput[1]};
                    Insertion::insertionActionHelper(temp, vmstate);
                    editor->setInsertion(false);
                    file->cmoveCursor(-1);
                }
            }
        }
        else if (lastInput[0] == 'c') {
            if (lastInput[1] == 'c') {
                removeLine(vmstate);
                pos = file->getCursorPos();

                Diff &diff = history->getUndo();
                LineChange change{"\n", true, pos.lineIndex, 0, true};
                diff.changes.push_back(change);

                file->addLineAbove();
                file->moveCursor(pos.lineIndex, 0);
            }
            else if (lastInput[1] == 'j' || lastInput[1] == 'k') {
                removeMotion(lastInput, vmstate);
                pos = file->getCursorPos();

                Diff &diff = history->getUndo();
                LineChange change{"\n", true, pos.lineIndex, 0, true};
                diff.changes.push_back(change);

                file->addLineAbove();
                file->moveCursor(pos.lineIndex, 0);
            }
            else removeMotion(lastInput, vmstate);

            editor->setCommandBar("--INSERT--");
        }
        else if (lastInput[0] == 'd') {
            if (lastInput[1] == 'd') removeLine(vmstate);
            else removeMotion(lastInput, vmstate);
        }
    }

    pos = file->getCursorPos();
    if (file->getLine().length() && pos.charIndex == file->getLine().length()) {
        file->cmoveCursor(-1);
        --pos.charIndex;
    }

    file->setVirtualPos(pos.lineIndex, pos.charIndex);

    file->updateContent();
    vmstate->updateViews(false);
    vmstate->updateViewsCursor(true, pos.lineIndex, pos.charIndex);
    vmstate->updateViews(true);
}
