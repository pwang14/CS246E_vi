#include "inputparser.h"
#include "../action.h"

#include "../actions/normal/movement.h"
#include "../actions/normal/quickmove.h"
#include "../actions/normal/normalswitch.h"
#include "../actions/normal/changetext.h"
#include "../actions/normal/undoredo.h"
#include "../actions/normal/copypaste.h"
#include "../actions/normal/search.h"
#include "../actions/normal/normalmisc.h"

#include "../actions/insert/insertion.h"
#include "../actions/insert/insertswitch.h"

#include "../actions/replace/replacement.h"
#include "../actions/replace/replaceswitch.h"

#include "../actions/command/command.h"
#include "../actions/command/commandswitch.h"

#include <ncurses.h>

using namespace std;

InputParser::~InputParser() {}

vector<Action*> InputParser::parseAction(vector<int> &input) { return action(input); }

VmParser::VmParser(): mode{Mode::NORMAL} {
    normalActions.push_back(make_unique<NoneAction>());
    normalActions.push_back(make_unique<NormalSwitch>());
    normalActions.push_back(make_unique<MovementAction>());
    normalActions.push_back(make_unique<QuickMovement>());
    normalActions.push_back(make_unique<ChangeText>());
    normalActions.push_back(make_unique<UndoRedo>());
    normalActions.push_back(make_unique<CopyPaste>());
    normalActions.push_back(make_unique<Search>());
    normalActions.push_back(make_unique<NormalMisc>());

    commandActions.push_back(make_unique<NoneAction>());
    commandActions.push_back(make_unique<CommandSwitch>());
    commandActions.push_back(make_unique<Command>());
    
    insertActions.push_back(make_unique<NoneAction>());
    insertActions.push_back(make_unique<InsertSwitch>());
    insertActions.push_back(make_unique<Insertion>());

    replaceActions.push_back(make_unique<NoneAction>());
    replaceActions.push_back(make_unique<ReplaceSwitch>());
    replaceActions.push_back(make_unique<Replacement>());
}

Action* VmParser::loopThroughActions(vector<int> &input, const vector<unique_ptr<Action>> &actions) {
    bool partialMatch = false;

    for (auto &i : actions) {
        matchInfo temp = i->matches(input);
        if (temp.matches) {
            input.clear();
            if (temp.ensureMode) mode = temp.mode;
            if (temp.extraSize) commandSize = temp.extraSize;
            if (temp.extraIndex) commandIndex = temp.extraIndex;
            return i.get();
        }
        else if (i->partialMatches(input)) partialMatch = true;
    }
    if (!partialMatch) input.clear();

    return actions.front().get();
}

vector<Action*> VmParser::action(vector<int> &input) {
    if (mode == Mode::INSERT) {
        vector<Action*> ret{loopThroughActions(input, insertActions)};
        return ret;
    }
    else if (mode == Mode::REPLACE) {
        vector<Action*> ret{loopThroughActions(input, replaceActions)};
        return ret;
    }
    else if (mode == Mode::COMMAND) {
        input.insert(input.begin(), commandIndex);
        input.insert(input.begin(), commandSize);
        vector<Action*> ret{loopThroughActions(input, commandActions)};
        return ret;
    }
    else {
        if (input.size() == 1 && isdigit(input.back()) && input.back() != '0') {
            multiplier = multiplier * 10 + static_cast<int>(input.back() - '0');
            input.clear();
        }
        else if (input.size() == 1 && input.back() == 27) {
            input.clear();
            multiplier = 0;
        }
        else if (input.size() == 1 && input.back() == '.' && lastNormalAction) {
            if (lastMultiplier && lastNormalAction != normalActions.front().get()) {
                return vector<Action*>(lastMultiplier, lastNormalAction);
            }
            return vector<Action*>(1, lastNormalAction);
        }

        Action* act = loopThroughActions(input, normalActions);
        
        if (act != normalActions.front().get()) {
            lastNormalAction = act;
            lastMultiplier = multiplier;
        }

        if (multiplier && act != normalActions.front().get()) {
            int temp = multiplier;
            multiplier = 0;
            return vector<Action*>(temp, act);
        }
        return vector<Action*>(1, act);
    }
}
