#ifndef CKEYBOARD_H
#define CKEYBOARD_H

#include "../controller.h"
#include "binding.h"
#include "inputparser.h"
#include <string>
#include <vector>
#include <memory>

class Action;

class CursesKeyboard: public Controller {
    std::unique_ptr<InputParser> parser;
    std::unique_ptr<CursesKeyBinding> binding;
    std::vector<int> buffer;

    std::vector<Action*> action() override;

  public:
    CursesKeyboard();
};

#endif
