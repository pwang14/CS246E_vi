#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "../../action.h"
#include <set>

class MovementAction: public Action {
    std::set<int> valid = {(int)'h', (int)'j', (int)'k', (int)'l'};
    std::vector<int> lastInput;

    void action(VmState *vmstate) override;

  public:
    static void movementActionHelper(const std::vector<int> &lastInput, VmState *vmstate);

    matchInfo matches(const std::vector<int> &input) override;
    bool partialMatches(const std::vector<int> &input) override;
};

#endif
