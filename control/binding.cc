#include "binding.h"

CursesKeyBinding::~CursesKeyBinding() {}

int NormalBinding::getKey(int input) { return input; }
