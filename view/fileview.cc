#include "fileview.h"
#include "../model.h"
#include "../vmstate/filestate.h"

#include <string>
#include <vector>

using namespace std;

//===== FileView =====

FileView::FileView(VmState *vmstate, AbstractFileState *file, unique_ptr<SyntaxParser> &&parser,
std::unique_ptr<Colorer> &&colorer, int height, int width, int starty, int startx): vmstate{vmstate}, file{file},
height{height}, width{width}, startLine{0}, translatedStartLine{0}, cursorLine{0}, cursorChar{0}, win{newwin(height, width, starty, startx)},
parser{move(parser)}, colorer{move(colorer)} {
    refresh();
    start_color();
    keypad(win, true);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_RED + 4, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN + 4, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW + 4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE + 4, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA + 4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN + 4, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE + 4, COLOR_WHITE, COLOR_BLACK);
}

FileView::~FileView() {
    delwin(win);
    win = nullptr;
}

void FileView::clearLine(size_t index) {
    if (index < static_cast<size_t>(height)) {
        wmove(win, index, 0);
        for (int i=0; i < width; ++i) waddch(win, ' ');
    }
}

void FileView::clearBlock(size_t index) {
    for (; index < static_cast<size_t>(height); ++index) clearLine(index);
}

void FileView::printString(std::string str, short color) {
    wattron(win, COLOR_PAIR(color + 4));
    wprintw(win, str.c_str());

    wattron(win, COLOR_PAIR(1));
}

void FileView::printSection(size_t start, size_t end) {
    int y, x;
    getyx(win, y, x);

    for (size_t i=start; i<=end; ++i) {
        string line = file->getContent()[i];

        if (y < height) {
            size_t startIndex = 0;
            while (startIndex < line.length()) {
                ParseItem token = parser->performParse(line, startIndex);
                short color = colorer->performColor(token.item);
                printString(line.substr(startIndex, token.itemLength), color);
                startIndex += token.itemLength;
            }

            getyx(win, y, x);

            if (!x && line.length()) --y;
            wmove(win, ++y, 0);
        }
        else break;
    }

    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    wattron(win, COLOR_PAIR(3));
    while (y < height) {
        waddch(win, '~');
        wmove(win, ++y, 0);
    }
    
    wattron(win, COLOR_PAIR(1));
}

size_t FileView::translateLine(size_t lineIndex) {
    size_t tempLine = 0;

    for (size_t i=0; i<lineIndex; ++i) {
        int length = file->getContent()[i].length();
        if (!length) ++tempLine;
        else tempLine += 1 + (length-1)/width;
    }

    return tempLine;
}

size_t FileView::reverseTranslateLine(size_t lineIndex) {
    size_t tempLine = 0;
    size_t ret = 0;

    for (; tempLine < lineIndex && ret < file->getNumLines(); ++ret) {
        int length = file->getContent()[ret].length();
        if (!length) ++tempLine;
        else tempLine += 1 + (length-1)/width;
    }

    return ret;
}

void FileView::update(bool forFile) {
    parser->updateInfo(file->getContent());

    if (forFile && file == vmstate->getCurrentFile()) {
        werase(win);
        wmove(win, 0, 0);
        printSection(startLine, file->getNumLines()-1);

        wmove(win, cursorLine, cursorChar);
    }
}

void FileView::updateLine(size_t lineIndex, int previousLength) {
    parser->updateInfo(file->getContent());

    if (previousLength > width) updateBlock(lineIndex);
    else {
        size_t temp = translateLine(lineIndex);
        if (temp >= translatedStartLine && temp < translatedStartLine + static_cast<size_t>(height)) {
            temp -= translatedStartLine;

            clearLine(temp);
            wmove(win, temp, 0);
            printSection(lineIndex, lineIndex);

            wmove(win, cursorLine, cursorChar);
        }
    }
}

void FileView::updateBlock(size_t startIndex) {
    parser->updateInfo(file->getContent());

    size_t temp = translateLine(startIndex);
    if (temp >= translatedStartLine && temp < translatedStartLine + static_cast<size_t>(height)) {
        temp -= translatedStartLine;
        
        clearBlock(temp);
        wmove(win, temp, 0);
        printSection(startIndex, file->getNumLines()-1);

        wmove(win, cursorLine, cursorChar);
    }
}

void FileView::updateCursor(bool forFile, size_t lineIndex, size_t charIndex, bool setScreen) {
    if (forFile && file == vmstate->getCurrentFile()) {
        cursorLine = translateLine(lineIndex);
        cursorChar = charIndex % width;

        int length = file->getContent()[lineIndex].length();
        size_t tempLine = cursorLine;
        if (length) tempLine += (length-1)/width;

        if (setScreen) {
            startLine = lineIndex;
            translatedStartLine = cursorLine;
            update(true);
        }
        else if (cursorLine < translatedStartLine) {
            startLine = lineIndex;
            translatedStartLine = cursorLine;
            update(true);
        }
        else if (tempLine >= translatedStartLine + static_cast<size_t>(height)) {
            startLine = reverseTranslateLine(tempLine - height + 1);
            translatedStartLine = translateLine(startLine);
            update(true);
        }

        cursorLine += charIndex/width;
        cursorLine -= translatedStartLine;

        wmove(win, cursorLine, cursorChar);
    }
}

void FileView::displayView(bool forFile) {
    if (forFile && file == vmstate->getCurrentFile()) {
        curs_set(1);
        wrefresh(win);
    }
}
