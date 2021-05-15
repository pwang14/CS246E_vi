#include "movement.h"
#include "../../model.h"

using namespace std;

matchInfo MovementAction::matches(const vector<int> &input) {
    if (input.size() == 1 && valid.count(input[0])) {
        lastInput = input;
        return matchInfo{true};
    }
    return matchInfo{false};
}

bool MovementAction::partialMatches(const vector<int> &input) { return false; }

void MovementAction::movementActionHelper(const vector<int> &lastInput, VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    CursorPos pos;
    CursorPos virtualPos;

    if (lastInput[0] == 'h') {
        file->cmoveCursor(-1);
        pos = file->getCursorPos();
        file->setVirtualPos(pos.lineIndex, pos.charIndex);
    }
    else if (lastInput[0] == 'j') {
        virtualPos = file->getVirtualPos();
        if (virtualPos.lineIndex < file->getNumLines()-1) file->moveCursor(virtualPos.lineIndex+1, virtualPos.charIndex);
        file->setVirtualPos(file->getCursorPos().lineIndex, virtualPos.charIndex);
    }
    else if (lastInput[0] == 'k') {
        virtualPos = file->getVirtualPos();
        if (virtualPos.lineIndex > 0) file->moveCursor(virtualPos.lineIndex-1, virtualPos.charIndex);
        file->setVirtualPos(file->getCursorPos().lineIndex, virtualPos.charIndex);
    }
    else if (lastInput[0] == 'l') {
        file->cmoveCursor(1);
        pos = file->getCursorPos();
        if (file->getLine().length() && pos.charIndex == file->getLine().length()) {
            file->cmoveCursor(-1);
            --pos.charIndex;
        }
        file->setVirtualPos(pos.lineIndex, pos.charIndex);
    }

    pos = file->getCursorPos();

    if (file->getLine().length() && pos.charIndex == file->getLine().length()) {
        file->cmoveCursor(-1);
        --pos.charIndex;
    }
}

void MovementAction::action(VmState *vmstate) {
    movementActionHelper(lastInput, vmstate);

    AbstractFileState *file = vmstate->getCurrentFile();
    CursorPos pos = file->getCursorPos();

    vmstate->updateViews(false);
    vmstate->updateViewsCursor(true, pos.lineIndex, pos.charIndex);
}
