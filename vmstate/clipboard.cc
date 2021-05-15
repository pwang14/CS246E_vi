#include "clipboard.h"

using namespace std;

//===== AbstractClipboard =====

AbstractClipboard::~AbstractClipboard() {}

//===== Clipboard =====

void Clipboard::clearClipboard() { data.clear(); }

void Clipboard::appendToClipboard(string input) { data.push_back(input); }

const vector<string> &Clipboard::getClipboard() { return data; }
