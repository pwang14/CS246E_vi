#include "undoredo.h"
#include "../../model.h"

using namespace std;

matchInfo UndoRedo::matches(const vector<int> &input) {
    if (input.size() == 1 && input[0] == 'u') {
        lastInput = input;
        return matchInfo{true};
    }
    return matchInfo{false};
}

bool UndoRedo::partialMatches(const vector<int> &input) { return false; }

void UndoRedo::action(VmState *vmstate) {
    AbstractFileState *file = vmstate->getCurrentFile();
    AbstractHistory *history = vmstate->getHistory();
    AbstractEditorState *editor = vmstate->getEditor();

    if (lastInput[0] == 'u') {
        if (history->isUndoEmpty()) {
            editor->setCommandBar("Already at oldest change");
        }
        else {
            Diff lastDiff = history->undoAction();
            vector<LineChange> changes = lastDiff.changes;

            for (auto i = changes.rbegin(); i != changes.rend(); ++i) {
                string line = i->line;
                bool added = i->added;
                CursorPos pos = CursorPos{i->lineIndex, i->charIndex};
                bool lineChanged = i->lineChanged;

                if (added) {
                    if (!lineChanged) {
                        file->moveCursor(pos.lineIndex, pos.charIndex);
                        file->remove(line.length());
                    }
                    else {
                        if (line == "\n") {
                            file->moveCursor(pos.lineIndex + 1, 0);
                            file->remove(-1);
                        }
                        else {
                            file->moveCursor(pos.lineIndex, 0);
                            file->deleteLine();
                        }
                    }
                }
                else {
                    if (!lineChanged) {
                        file->moveCursor(pos.lineIndex, pos.charIndex);
                        file->insertAfter(line);
                    }
                    else {
                        if (line == "\n") {
                            file->moveCursor(pos.lineIndex, pos.charIndex);
                            file->insertNewLine();
                        }
                        else {
                            file->moveCursor(pos.lineIndex, pos.charIndex);
                            file->insertNewLine();
                            file->lmoveCursor(-1);
                            file->insertAfter(line);
                        }
                    }
                }
            }
        }
    }
    
    file->updateContent();
    CursorPos pos = file->getCursorPos();
    file->setVirtualPos(pos.lineIndex, pos.charIndex);

    vmstate->updateViews(false);
    vmstate->updateViewsCursor(true, pos.lineIndex, pos.charIndex);
    vmstate->updateViews(true);
}
