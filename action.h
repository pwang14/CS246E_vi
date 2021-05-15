#ifndef ACTION_H
#define ACTION_H

#include <vector>

class VmState;

enum Mode { NORMAL, COMMAND, INSERT, REPLACE };

struct matchInfo {
  bool matches;
  bool ensureMode = false;
  Mode mode = Mode::NORMAL;
  int extraSize = 0;
  int extraIndex = 0;
};

class Action {
    virtual void action(VmState *vmstate) = 0;

  public:
    virtual ~Action() = 0;

    void performAction(VmState *vmstate);
    
    virtual matchInfo matches(const std::vector<int> &input) = 0;
    virtual bool partialMatches(const std::vector<int> &input) = 0;
};

class NoneAction: public Action {
    void action(VmState *vmstate) override;

  public:
    matchInfo matches(const std::vector<int> &input) override;
    bool partialMatches(const std::vector<int> &input) override;
};

#endif
