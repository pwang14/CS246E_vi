#include "command.h"
#include "../../model.h"
#include <vector>
#include <assert.h>

using namespace std;

matchInfo Command::matches(const vector<int> &input) {
    if (input.size() == 3) {
        if (input[2] != 27 && input[2] != 10 && input[2] != KEY_BACKSPACE) {
            lastInput = input;
            if (input[2] == KEY_UP || input[2] == KEY_DOWN) return matchInfo{true};
            else if (input[2] == KEY_LEFT) {
                if (input[1] > 1) return matchInfo{true, false, Mode::COMMAND, input[0], input[1] - 1};
                else return matchInfo{true};
            }
            else if (input[2] == KEY_RIGHT) {
                if (input[1] == input[0]) return matchInfo{true};
                else return matchInfo{true, false, Mode::COMMAND, input[0], input[1] + 1};
            }
            else return matchInfo{true, false, Mode::COMMAND, input[0] + 1, input[1] + 1};
        }
        else if (input[2] == KEY_BACKSPACE && input[0] > 1) {
            lastInput = input;
            if (input[1] > 1) return matchInfo{true, false, Mode::COMMAND, input[0] - 1, input[1] - 1};
            else return matchInfo{true};
        }
    }
    return matchInfo{false};
}

bool Command::partialMatches(const vector<int> &input) { return false; }

void Command::action(VmState *vmstate) {
    AbstractEditorState *editor = vmstate->getEditor();

    if (lastInput[2] < 128) {
        size_t pos = editor->getCursor();
        assert(pos == static_cast<size_t>(lastInput[1]));

        string s = editor->getCommandBar();
        string ins(1, static_cast<char>(lastInput[2]));
        s.insert(pos, ins);
        editor->setCommandBar(s);
        editor->setCursor(pos + 1);
    }
    else if (lastInput[2] == KEY_BACKSPACE) {
        size_t pos = editor->getCursor();
        assert(pos == static_cast<size_t>(lastInput[1]));

        if (pos > 1) {
            string s = editor->getCommandBar();
            s.erase(pos - 1, 1);
            editor->setCommandBar(s);
            editor->setCursor(pos - 1);
        }
    }
    else if (lastInput[2] == KEY_LEFT) {
        size_t pos = editor->getCursor();
        assert(pos == static_cast<size_t>(lastInput[1]));

        if (pos > 1) editor->setCursor(pos - 1);
    }
    else if (lastInput[2] == KEY_RIGHT) {
        size_t pos = editor->getCursor();
        assert(pos == static_cast<size_t>(lastInput[1]));

        string s = editor->getCommandBar();
        if (pos < s.length()) editor->setCursor(pos + 1);
    }

    vmstate->updateViews(false);
    vmstate->updateViewsCursor(false, 0, editor->getCursor());
}
