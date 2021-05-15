#include "controller.h"
#include "action.h"

Controller::~Controller() {}

std::vector<Action*> Controller::getAction() { return action(); }
