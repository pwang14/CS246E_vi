#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <ncurses.h>
#include <regex>
//#include <boost/regex.hpp>

#include "model.h"
#include "view.h"
#include "view/fileview.h"
#include "view/colorer.h"
#include "view/syntaxparser.h"
#include "view/commandbarview.h"
#include "controller.h"
#include "control/ckeyboard.h"

using namespace std;
//using namespace boost;

int main(int argc, char* argv[]) {
    if (argc == 1) return 1;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);

    if (has_colors() == FALSE) {
        endwin();
        return 0;
    }

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    unique_ptr<VmState> state = make_unique<VmState>(argv[1], maxY, maxX);

    regex reg{"[a-zA-Z0-9]+.(h|cc)"};

    unique_ptr<View> fileView;
    if (regex_match(argv[1], reg)) fileView = make_unique<FileView>(state.get(), state->getCurrentFile(), make_unique<CppParser>(), make_unique<VmColorer>(), maxY-1, maxX);
    else fileView = make_unique<FileView>(state.get(), state->getCurrentFile(), make_unique<DefaultParser>(), make_unique<VmColorer>(), maxY-1, maxX);

    unique_ptr<View> commandBarView = make_unique<CommandBarView>(state.get(), maxX, maxY-1);

    unique_ptr<Controller> keyboard = make_unique<CursesKeyboard>();

    state->addView(fileView.get());
    state->addView(commandBarView.get());

    state->addController(keyboard.get());

    state->initialDisplay();

    while (!state->getExit()) {
        state->run();
    }

    endwin();

    return 0;
}
