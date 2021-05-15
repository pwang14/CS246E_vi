#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>

class Action;

class Controller {
    virtual std::vector<Action*> action() = 0;

  public:
    virtual ~Controller() = 0;

    std::vector<Action*> getAction();
};

#endif
