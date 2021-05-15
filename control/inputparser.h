#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include "../action.h"
#include <vector>
#include <string>
#include <memory>
#include <set>

class InputParser {
    virtual std::vector<Action*> action(std::vector<int> &input) = 0;

  public:
    virtual ~InputParser() = 0;

    std::vector<Action*> parseAction(std::vector<int> &input);
};

class VmParser: public InputParser {
    Mode mode;
    std::vector<std::unique_ptr<Action>> normalActions;
    std::vector<std::unique_ptr<Action>> commandActions;
    std::vector<std::unique_ptr<Action>> insertActions;
    std::vector<std::unique_ptr<Action>> replaceActions;
    
    int multiplier = 0;

    int commandSize = 0;
    int commandIndex = 0;

    int lastMultiplier = 0;
    Action *lastNormalAction = nullptr;

    Action* loopThroughActions(std::vector<int> &input, const std::vector<std::unique_ptr<Action>> &actions);

    std::vector<Action*> action(std::vector<int> &input) override;

  public:
    VmParser();
};

#endif
