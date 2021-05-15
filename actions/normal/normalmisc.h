#ifndef NORMALMISC_H
#define NORMALMISC_H

#include "../../action.h"

class NormalMisc: public Action {
    std::vector<char> validChars{'b', 'd', 'f', 'g', 'u'};
    std::vector<int> lastInput;

    void action(VmState *vmstate) override;

  public:
    matchInfo matches(const std::vector<int> &input) override;
    bool partialMatches(const std::vector<int> &input) override;
};

#endif
