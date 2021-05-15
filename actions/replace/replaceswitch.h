#ifndef REPLACESWITCH_H
#define REPLACESWITCH_H

#include "../../action.h"

class ReplaceSwitch: public Action {
    std::vector<int> lastInput;

    void action(VmState *vmstate) override;

  public:
    matchInfo matches(const std::vector<int> &input) override;
    bool partialMatches(const std::vector<int> &input) override;
};

#endif
