#ifndef NORMALSWITCH_H
#define NORMALSWITCH_H

#include "../../action.h"
#include <set>

class NormalSwitch: public Action {
    std::set<int> insertValid{(int)'a', (int)'i', (int)'o', (int)'A', (int)'I', (int)'O'};

    std::vector<int> lastInput;

    void action(VmState *vmstate) override;

  public:
    matchInfo matches(const std::vector<int> &input) override;
    bool partialMatches(const std::vector<int> &input) override;
};

#endif
