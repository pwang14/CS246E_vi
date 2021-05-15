#ifndef INSERTION_H
#define INSERTION_H

#include "../../action.h"

class Insertion: public Action {
    std::vector<int> lastInput;

    void action(VmState *vmstate) override;

  public:
    static void insertionActionHelper(const std::vector<int> &lastInput, VmState *vmstate);

    matchInfo matches(const std::vector<int> &input) override;
    bool partialMatches(const std::vector<int> &input) override;
};

#endif
