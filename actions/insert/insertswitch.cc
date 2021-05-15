#include "insertswitch.h"
#include "../../model.h"

using namespace std;

matchInfo InsertSwitch::matches(const vector<int> &input) {
    if (input.size() == 1 && input[0] == 27) {
        lastInput = input;
        return matchInfo{true, true, Mode::NORMAL};
    }
    return matchInfo{false};
}

bool InsertSwitch::partialMatches(const vector<int> &input) { return false; }

void InsertSwitch::action(VmState *vmstate) {
    AbstractEditorState *editor = vmstate->getEditor();
    AbstractFileState *file = vmstate->getCurrentFile();

    editor->setCommandBar("");
    editor->setInsertion(false);

    CursorPos pos = file->getCursorPos();
    
    if (file->getLine().length() && pos.charIndex == file->getLine().length()) {
        file->cmoveCursor(-1);
        --pos.charIndex;
    }

    vmstate->updateViews(false);
    vmstate->updateViewsCursor(true, pos.lineIndex, pos.charIndex);
}
