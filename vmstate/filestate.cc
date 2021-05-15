#include "filestate.h"

#include "filecontent/reversegapbuffer.h"
#include <fstream>
#include <algorithm>

using namespace std;

//===== PermissionBehaviour =====

PermissionBehaviour::~PermissionBehaviour() {}

bool StrictPermissionBehaviour::allowEdit(bool isReadOnly) { return !isReadOnly; }

bool ChillPermissionBehaviour::allowEdit(bool isReadOnly) { return true; }

//===== AbstractFileState =====

AbstractFileState::AbstractFileState(const string &fname, bool readOnly, unique_ptr<PermissionBehaviour> &&behaviour):
fname{fname}, readOnly{readOnly}, behaviour{move(behaviour)} {}

AbstractFileState::~AbstractFileState() {}

string AbstractFileState::getFileName() { return fname; }

bool AbstractFileState::isReadOnly() { return readOnly; }

PermissionBehaviour *AbstractFileState::getPermissionBehaviour() { return behaviour.get(); }

void AbstractFileState::setPermissionBehaviour(std::unique_ptr<PermissionBehaviour> &&newBehaviour)
{ behaviour = move(newBehaviour); }

//===== FileState =====

FileState::FileState(const string &fname): AbstractFileState{fname}, content{make_unique<ReverseGapBuffer>(fname)},
contentCopy{content->getContent()}, virtualPos{0, 0} {
    ifstream f{fname.c_str()};

    string line;
    while (getline(f, line)) original.push_back(line);
}

void FileState::saveFile() {
    ofstream os;
    os.open(getFileName().c_str(), ios::trunc);
    
    updateContent();
    original = contentCopy;

    for (auto i : contentCopy) {
        os << i << "\n";
    }
}

bool FileState::changeMade() {
    updateContent();
    if (original.size() != contentCopy.size()) return true;
    for (size_t i=0; i < original.size(); ++i) {
        if (original[i] != contentCopy[i]) return true;
    }
    return false;
}

void FileState::updateContent() { contentCopy = content->getContent(); }

const std::vector<std::string> &FileState::getContent() { return contentCopy; }

CursorPos FileState::getVirtualPos() { return virtualPos; }

void FileState::setVirtualPos(size_t lineIndex, size_t charIndex) {
    virtualPos.lineIndex = lineIndex;
    virtualPos.charIndex = charIndex;
}

CursorPos FileState::getCursorPos() { return content->getCursorPos(); }

size_t FileState::getNumLines() { return content->getNumLines(); }

size_t FileState::getNumChars() { return content->getNumChars(); }

CursorPos FileState::findNext(std::string input) { return content->findNext(input); }

CursorPos FileState::findPrevious(std::string input) { return content->findPrevious(input); }

void FileState::moveCursor(size_t lineIndex, size_t charIndex) {
    CursorPos pos = content->getCursorPos();

    int lamount = static_cast<int>(lineIndex) - static_cast<int>(pos.lineIndex);
    lmoveCursor(lamount);
    
    CursorPos after = content->getCursorPos();

    if (after.lineIndex == pos.lineIndex) {
        int camount = static_cast<int>(charIndex) - static_cast<int>(after.charIndex);
        cmoveCursor(camount);
    }
    else cmoveCursor(charIndex);
}

void FileState::lmoveCursor(int amount) {
    if (amount >= 0) content->moveDown(amount);
    else content->moveUp(-amount);
}

void FileState::cmoveCursor(int amount) {
    if (amount >= 0) content->moveRight(amount);
    else content->moveLeft(-amount);
}

void FileState::insertAfter(string input) {
    if (input.length() == 1) content->insertAfter(input[0]);
    else content->insertAfter(input);
}

void FileState::insertBefore(string input) {
    if (input.length() == 1) content->insertBefore(input[0]);
    else content->insertBefore(input);
}

void FileState::remove(int amount) {
    if (amount < 0) content->removeBefore(-amount);
    else content->removeAfter(amount);
}

void FileState::addLineAbove() { content->addLineAbove(); }

void FileState::addLineBelow() { content->addLineBelow(); }

void FileState::insertNewLine() { content->insertNewLine(); }

void FileState::deleteLine() { content->removeLine(); }

void FileState::deleteLines(int num) {
    if (num >= 0) {
        for (int i=0; i<num; ++i) content->removeLine();
    }
    else {
        for (int i=0; i>num; --i) {
            content->removeLine();
            content->moveUp();
        }
    }
}

string FileState::getLine() { return content->getLine(); }

string FileState::getLine(size_t lineIndex) { return content->getLine(lineIndex); }

string FileState::getLineRelative(int offset) { return content->getLineRelative(offset); }

vector<string> FileState::getLines(int num) { return content->getLines(num); }
