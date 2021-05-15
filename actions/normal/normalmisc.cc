#include "normalmisc.h"
#include "../../model.h"

using namespace std;

matchInfo NormalMisc::matches(const vector<int> &input) {
    if (input.size() == 1) {
        for (auto i : validChars) {
            if (input[0] == (i & 0x1F)) {
                lastInput = input;
                return matchInfo{true};
            }
        }
    }
    return matchInfo{false};
}

bool NormalMisc::partialMatches(const vector<int> &input) { return false; }

void NormalMisc::action(VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractEditorState *editor = vmstate->getEditor();
    CursorPos pos = file->getCursorPos();
    size_t maxY = static_cast<size_t>(editor->getMaxY());

    if (lastInput[0] == ('b' & 0x1F)) {
        if (pos.lineIndex >= maxY/2) {
            file->moveCursor(pos.lineIndex - maxY/2, pos.charIndex);
        }
        else {
            file->moveCursor(0, pos.charIndex);
        }
    }
    else if (lastInput[0] == ('d' & 0x1F)) {
        file->moveCursor(pos.lineIndex + maxY/2, pos.charIndex);
    }
    else if (lastInput[0] == ('f' & 0x1F)) {
        file->moveCursor(pos.lineIndex + maxY, pos.charIndex);
    }
    else if (lastInput[0] == ('g' & 0x1F)) {
        string fname = editor->getFileName();
        int numLines = static_cast<int>(file->getNumLines());
        int percent = 100.0*((double) pos.lineIndex + 1.0)/numLines;

        string commandBar = "\"" + fname + "\" " + to_string(numLines) + " lines --" + to_string(percent) + "&--";
        editor->setCommandBar(commandBar);
    }
    else if (lastInput[0] == ('u' & 0x1F)) {
        if (pos.lineIndex >= maxY/2) {
            file->moveCursor(pos.lineIndex - maxY/2, pos.charIndex);
        }
        else {
            file->moveCursor(0, pos.charIndex);
        }
    }

    pos = file->getCursorPos();

    if (file->getLine().length() && pos.charIndex == file->getLine().length()) {
        file->cmoveCursor(-1);
        --pos.charIndex;
    }

    if (lastInput[0] != ('g' & 0x1F)) file->setVirtualPos(pos.lineIndex, pos.charIndex);

    vmstate->updateViews(false);
    if (lastInput[0] == ('f' &0x1F)) vmstate->updateViewsCursor(true, pos.lineIndex, pos.charIndex, true);
    else vmstate->updateViewsCursor(true, pos.lineIndex, pos.charIndex);
}
