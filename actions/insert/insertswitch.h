#ifndef INSERTSWITCH_H
#define INSERTSWITCH_H

#include "../../action.h"

class InsertSwitch: public Action {
    std::vector<int> lastInput;

    void action(VmState *vmstate) override;

  public:
    matchInfo matches(const std::vector<int> &input) override;
    bool partialMatches(const std::vector<int> &input) override;
};

#endif
