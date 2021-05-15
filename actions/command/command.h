#ifndef COMMAND_H
#define COMMAND_H

#include "../../action.h"
#include <string>
#include <set>
#include <ncurses.h>

class Command: public Action {
    std::vector<int> lastInput;

    void action(VmState *vmstate) override;

  public:
    matchInfo matches(const std::vector<int> &input) override;
    bool partialMatches(const std::vector<int> &input) override;
};

#endif
