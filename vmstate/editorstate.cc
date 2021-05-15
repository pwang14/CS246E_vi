#include "editorstate.h"

using namespace std;

AbstractEditorState::~AbstractEditorState() {}

EditorState::EditorState(string fname, int maxY, int maxX): fileName{fname}, maxY{maxY}, maxX{maxX}, cursorPos{0}, insertion{false}, cursorVisibility{false}, forwardSearch{true} {}

const string &EditorState::getFileName() { return fileName; }

void EditorState::setFileName(string fname) { fileName = fname; }

const string &EditorState::getCommandBar() { return commandBar; }

void EditorState::setCommandBar(string input) { commandBar = input; }

size_t EditorState::getCursor() { return cursorPos; }

void EditorState::setCursor(size_t pos) { cursorPos = pos; }

bool EditorState::previousInsertion() { return insertion; }

void EditorState::setInsertion(bool input) { insertion = input; }

string EditorState::getSearchTerm() { return searchTerm; }

void EditorState::setSearchTerm(string input) { searchTerm = input; }

bool EditorState::getForwardSearch() { return forwardSearch; }

void EditorState::setForwardSearch(bool forward) { forwardSearch = forward; }

bool EditorState::cursorOn() { return cursorVisibility; }

void EditorState::setCursorOn(bool input) { cursorVisibility = input; }

unordered_map<size_t, unordered_map<size_t, char>> &EditorState::getReplacementMap() { return replacementMap; }

void EditorState::clearReplacementMap() { replacementMap.clear(); }

bool EditorState::getLastFForward() { return lastFFoward; }

void EditorState::setLastFFoward(bool input) { lastFFoward = input; }

char EditorState::getLastFChar() { return lastFChar; }

void EditorState::setLastFChar(char c) { lastFChar = c; }

int EditorState::getMaxY() { return maxY; }

int EditorState::getMaxX() { return maxX; }

void EditorState::setMax(int y, int x) {
    maxY = y;
    maxX = x;
}
