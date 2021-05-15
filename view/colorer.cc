#include "colorer.h"

#include <ncurses.h>

using namespace std;

//===== Colorer =====

Colorer::~Colorer() {}

short Colorer::performColor(SyntaxItem input) { return color(input); }

//===== VmColorer =====

VmColorer::VmColorer():
colorMap{
    map<SyntaxItem, short>{
        {SyntaxItem::KEYWORD, COLOR_CYAN},
        {SyntaxItem::NUMERIC, COLOR_RED},
        {SyntaxItem::STRING, COLOR_GREEN},
        {SyntaxItem::ID, COLOR_BLUE},
        {SyntaxItem::COMMENT, COLOR_YELLOW},
        {SyntaxItem::PREPROCESSOR, COLOR_MAGENTA},
        {SyntaxItem::ELSE, COLOR_WHITE}
    }
} {}

short VmColorer::color(SyntaxItem input) { return colorMap[input]; }

//===== NoColor =====

short NoColor::color(SyntaxItem input) { return COLOR_WHITE; }
