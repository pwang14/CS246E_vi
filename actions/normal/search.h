#ifndef SEARCH_H
#define SEARCH_H

#include "../../action.h"
#include <string>
#include <set>

class Search: public Action {
    std::set<int> singleValid = {(int)'n', (int)'N', (int)';'};

    std::vector<int> lastInput;

    void action(VmState *vmstate) override;

    static void searchForward(std::string s, VmState *vmstate);
    static void searchBackward(std::string s, VmState *vmstate);

  public:
    static void searchActionHelper(const std::vector<int> &lastInput, VmState *vmstate);

    matchInfo matches(const std::vector<int> &input) override;
    bool partialMatches(const std::vector<int> &input) override; 
};

#endif
