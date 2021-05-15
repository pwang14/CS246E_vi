#ifndef COPYPASTE_H
#define COPYPASTE_H

#include "../../action.h"
#include <set>

class CopyPaste: public Action {
    std::set<int> singleValid = {(int)'p', (int)'P'};
    std::set<int> partialValid = {(int)'y'};
    std::set<int> endValid = {(int)'h', (int)'j', (int)'k', (int)'l', (int)'b', (int)'w', (int)'^', (int)'$', (int)'0'};

    std::vector<int> lastInput;

    void action(VmState *vmstate) override;

    void paste(VmState *vmstate, bool shifted = false);
    void copyMotion(const std::vector<int> &lastInput, VmState *vmstate);

  public:
    static void movementActionHelper(const std::vector<int> &lastInput, VmState *vmstate);

    matchInfo matches(const std::vector<int> &input) override;
    bool partialMatches(const std::vector<int> &input) override;
};

#endif
