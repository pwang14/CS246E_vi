#include "action.h"

Action::~Action() {}

void Action::performAction(VmState *vmstate) { action(vmstate); }

void NoneAction::action(VmState *vmstate) {}

matchInfo NoneAction::matches(const std::vector<int> &input) { return matchInfo{false}; }

bool NoneAction::partialMatches(const std::vector<int> &input) { return false; }
