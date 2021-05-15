#include "ckeyboard.h"

#include <ncurses.h>

using namespace std;

CursesKeyboard::CursesKeyboard(): parser{make_unique<VmParser>()},
binding{make_unique<NormalBinding>()} {}

vector<Action*> CursesKeyboard::action() {
    cbreak();
    noecho();

    int c = getch();

    buffer.push_back(binding->getKey(c));

    return parser->parseAction(buffer);
}
