#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include "../../action.h"

class Replacement: public Action {
    std::vector<int> lastInput;

    void action(VmState *vmstate) override;

  public:
    matchInfo matches(const std::vector<int> &input) override;
    bool partialMatches(const std::vector<int> &input) override;
};

#endif
