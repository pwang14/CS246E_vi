#include "quickmove.h"
#include "../../model.h"

using namespace std;

matchInfo QuickMovement::matches(const vector<int> &input) {
    if (input.size() == 1 && valid.count(input[0])) {
        lastInput = input;
        return matchInfo{true};
    }
    return matchInfo{false};
}

bool QuickMovement::partialMatches(const vector<int> &input) { return false; }

void QuickMovement::quickMovementActionHelper(const vector<int> &lastInput, VmState *vmstate) {
    std::set<int> punctuation = {(int)'"', (int)',', (int)'.', (int)'?', (int)'!'};

    AbstractFileState *file = vmstate->getCurrentFile();

    if (lastInput[0] == 'b') {
        CursorPos pos = file->getCursorPos();
        if (pos.lineIndex || pos.charIndex) {
            if (pos.charIndex) --pos.charIndex;
            else pos.charIndex = file->getContent()[--pos.lineIndex].length()-1;

            while (file->getContent()[pos.lineIndex][pos.charIndex] == ' ') {
                if (pos.charIndex) --pos.charIndex;
                else pos.charIndex = file->getContent()[--pos.lineIndex].length()-1;
            }

            string line = file->getContent()[pos.lineIndex];
            while (pos.charIndex && line[pos.charIndex] != ' ' &&
            !punctuation.count(line[pos.charIndex])) --pos.charIndex;

            if (line[pos.charIndex] == ' ') file->moveCursor(pos.lineIndex, pos.charIndex+1);
            else file->moveCursor(pos.lineIndex, pos.charIndex);
        }
    }
    else if (lastInput[0] == 'w') {
        CursorPos pos = file->getCursorPos();
        if (pos.lineIndex != file->getNumLines()-1 ||
        (file->getContent().back().length() && pos.charIndex != file->getContent().back().length()-1))
        {
            string line = file->getLine();
            if (line.length() && line[pos.charIndex] != ' ' && !punctuation.count(line[pos.charIndex])) {
                while (pos.charIndex < line.length()-1 && line[pos.charIndex] != ' ' &&
                !punctuation.count(line[pos.charIndex])) ++pos.charIndex;
                if (line[pos.charIndex] == ' ' || punctuation.count(line[pos.charIndex])) --pos.charIndex;
            }

            if (line.length() && pos.charIndex < line.length()-1) ++pos.charIndex;
            else if (pos.lineIndex < file->getNumLines()-1) {
                line = file->getContent()[++pos.lineIndex];
                pos.charIndex = 0;
            }

            while (line.length() && line[pos.charIndex] == ' ') {
                if (pos.charIndex < line.length()-1) ++pos.charIndex;
                else if (pos.lineIndex < file->getNumLines()-1) {
                    line = file->getContent()[++pos.lineIndex];
                    pos.charIndex = 0;
                }
                else break;
            }

            file->moveCursor(pos.lineIndex, pos.charIndex);
        }
    }
    else if (lastInput[0] == '^') {
        CursorPos pos = file->getCursorPos();
        file->moveCursor(pos.lineIndex, 0);
    }
    else if (lastInput[0] == '$') {
        CursorPos pos = file->getCursorPos();
        size_t length = file->getLine().length();
        if (length) file->moveCursor(pos.lineIndex, length-1);
        else file->moveCursor(pos.lineIndex, 0);
    }
    else if (lastInput[0] == '0') {
        CursorPos pos = file->getCursorPos();
        file->moveCursor(pos.lineIndex, 0);
    }
}

void QuickMovement::action(VmState *vmstate) {
    quickMovementActionHelper(lastInput, vmstate);

    AbstractFileState *file = vmstate->getCurrentFile();
    CursorPos pos = file->getCursorPos();
    file->setVirtualPos(pos.lineIndex, pos.charIndex);

    vmstate->updateViews(false);
    vmstate->updateViewsCursor(true, pos.lineIndex, pos.charIndex);
}
