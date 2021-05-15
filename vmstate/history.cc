#include "history.h"

//===== AbstractHistory =====

AbstractHistory::~AbstractHistory() {}

//===== History =====

void History::addDiff(Diff diff) { undoStack.push(diff); }

bool History::isUndoEmpty() { return undoStack.empty(); }

Diff &History::getUndo() { return undoStack.top(); }

Diff History::undoAction() {
    Diff ret = undoStack.top();
    redoStack.push(ret);
    undoStack.pop();
    return ret;
}

void History::popUndo() { undoStack.pop(); }

Diff History::redoAction() {
    Diff ret = redoStack.top();
    undoStack.push(ret);
    redoStack.pop();
    return ret;
}

void History::clearRedo() {
    while (!redoStack.empty()) redoStack.pop();
}
