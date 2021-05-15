#ifndef CHANGETEXT_H
#define CHANGETEXT_H

#include "../../action.h"
#include <set>

class ChangeText: public Action {
    std::set<int> singleValid = {(int)'s', (int)'x', (int)'J', (int)'S', (int)'X'};

    std::set<int> partialValid = {(int)'r', (int)'c', (int)'d'};
    std::set<int> endValid = {(int)'h', (int)'j', (int)'k', (int)'l', (int)'b', (int)'w', (int)'^', (int)'$', (int)'0'};

    std::vector<int> lastInput;

    void action(VmState *vmstate) override;

    void removeCharAt(VmState *vmstate);
    void removeLine(VmState *vmstate);
    void removeMotion(const std::vector<int> &lastInput, VmState *vmstate);

  public:
    matchInfo matches(const std::vector<int> &input) override;
    bool partialMatches(const std::vector<int> &input) override;
};

#endif
