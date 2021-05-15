#include "model.h"
#include <cstdlib>

using namespace std;

//===== Model =====

void Model::addView(View *view) { views.push_back(view); }

void Model::addController(Controller *control) { controller = control; }

std::vector<Action*> Model::getAction() { return controller->getAction(); }

Model::~Model() {}

void Model::updateViews(bool forFile) {
    for (auto i : views) i->update(forFile);
}

void Model::updateViewsLine(size_t lineIndex, int previousLength) {
    for (auto i : views) i->updateLine(lineIndex, previousLength);
}

void Model::updateViewsCursor(bool forFile, size_t lineIndex, size_t charIndex, bool setScreen) {
    for (auto i : views) i->updateCursor(forFile, lineIndex, charIndex, setScreen);
}

void Model::updateViewsBlock(size_t startIndex) {
    for (auto i : views) i->updateBlock(startIndex);
}

void Model::displayViews(bool forFile) {
    for (auto i : views) i->displayView(forFile);
}

void Model::initialDisplay() {
    updateViews(false);
    updateViewsCursor(true, 0, 0);
    updateViews(true);

    displayViews(false);
    displayViews(true);
}

void Model::exit() { exitVal = true; }

bool Model::getExit() { return exitVal; }

//===== VmState =====

VmState::VmState(const string &fname, int maxY, int maxX): clipboard{make_unique<Clipboard>()},
history{make_unique<History>()}, editor{make_unique<EditorState>(fname, maxY, maxX)} {
    files.push_back(make_unique<FileState>(fname));
    currentFile = files[0].get();

    int numLines = currentFile->getNumLines();
    int numChars = numLines;
    for (auto &i : currentFile->getContent()) {
        numChars += i.length();
    }

    string opening = "\"" + fname + "\" " + to_string(numLines) + "L, " + to_string(numChars) + "C";
    editor->setCommandBar(move(opening));
}

void VmState::run() {
    vector<Action*> temp = getAction();
    for (auto &i : temp) i->performAction(this);
    
    displayViews(false);
    displayViews(true);
    if (editor->cursorOn()) displayViews(false);
}

AbstractFileState *VmState::getCurrentFile() { return currentFile; }

AbstractFileState *VmState::getFile(size_t index) { return files.at(index).get(); }

AbstractClipboard *VmState::getClipboard() { return clipboard.get(); }

AbstractHistory *VmState::getHistory() { return history.get(); }

AbstractEditorState *VmState::getEditor() { return editor.get(); }
