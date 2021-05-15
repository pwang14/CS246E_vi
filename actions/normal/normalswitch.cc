#include "normalswitch.h"
#include "../../model.h"

using namespace std;

matchInfo NormalSwitch::matches(const vector<int> &input) {
    if (input.size() == 1) {
        if (insertValid.count(input[0])) {
            lastInput = input;
            return matchInfo{true, true, Mode::INSERT};
        }
        else if (input[0] == 'R') {
            lastInput = input;
            return matchInfo{true, true, Mode::REPLACE};
        }
        else if (input[0] == ':' || input[0] == '/' || input[0] == '?') {
            lastInput = input;
            return matchInfo{true, true, Mode::COMMAND, 1, 1};
        }
    }
    return matchInfo{false};
}

bool NormalSwitch::partialMatches(const vector<int> &input) { return false; }

void NormalSwitch::action(VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractEditorState *editor = vmstate->getEditor();
    AbstractHistory *history = vmstate->getHistory();

    if (lastInput[0] == 'i') {
        editor->setCommandBar("--INSERT--");
    }
    else if (lastInput[0] == 'a') {
        file->cmoveCursor(1);
        editor->setCommandBar("--INSERT--");
    }
    else if (lastInput[0] == 'o') {
        CursorPos pos = file->getCursorPos();
        LineChange change{"\n", true, pos.lineIndex, 0, true};
        Diff diff{vector<LineChange>{change}};
        history->addDiff(diff);

        file->addLineBelow();
        file->lmoveCursor(1);
        editor->setCommandBar("--INSERT--");
    }
    else if (lastInput[0] == 'A') {
        string line = file->getLine();
        file->cmoveCursor(line.length());
        editor->setCommandBar("--INSERT--");
    }
    else if (lastInput[0] == 'I') {
        CursorPos pos = file->getCursorPos();
        file->moveCursor(pos.lineIndex, 0);
        editor->setCommandBar("--INSERT--");
    }
    else if (lastInput[0] == 'O') {
        CursorPos pos = file->getCursorPos();
        LineChange change{"", true, pos.lineIndex, 0, true};
        Diff diff{vector<LineChange>{change}};
        history->addDiff(diff);

        file->addLineAbove();
        file->lmoveCursor(-1);
        editor->setCommandBar("--INSERT--");
    }
    else if (lastInput[0] == 'R') {
        editor->setCommandBar("--REPLACE--");
    }
    else if (lastInput[0] == ':') {
        editor->setCursorOn(true);
        editor->setCommandBar(":");
        editor->setCursor(1);
        editor->setCursor(true);
    }
    else if (lastInput[0] == '/') {
        editor->setCursorOn(true);
        editor->setCommandBar("/");
        editor->setCursor(1);
        editor->setCursor(true);
    }
    else if (lastInput[0] == '?') {
        editor->setCursorOn(true);
        editor->setCommandBar("?");
        editor->setCursor(1);
        editor->setCursor(true);
    }

    file->updateContent();

    CursorPos pos = file->getCursorPos();
    file->setVirtualPos(pos.lineIndex, pos.charIndex);

    vmstate->updateViews(false);
    vmstate->updateViewsCursor(false, 0, editor->getCursor());
    vmstate->updateViewsCursor(true, pos.lineIndex, pos.charIndex);
    vmstate->updateViews(true);
}
