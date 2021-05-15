#ifndef QUICKMOVE_H
#define QUICKMOVE_H

#include "../../action.h"
#include <string>
#include <set>

class QuickMovement: public Action {
    std::set<int> valid = {(int)'b', (int)'w', (int)'^', (int)'$', (int)'0'};
    std::vector<int> lastInput;

    void action(VmState *vmstate) override;

  public:
    static void quickMovementActionHelper(const std::vector<int> &lastInput, VmState *vmstate);

    matchInfo matches(const std::vector<int> &input) override;
    bool partialMatches(const std::vector<int> &input) override;
};

#endif
