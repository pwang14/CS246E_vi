#ifndef VIEW_H
#define VIEW_H

#include <string>
#include <vector>

class AbstractFileState;

class View {
  public:
    virtual ~View() = 0;

    virtual void update(bool forFile) = 0;
    virtual void updateLine(size_t lineIndex, int previousLength);
    virtual void updateBlock(size_t startIndex);
    virtual void updateCursor(bool forFile, size_t lineIndex, size_t charIndex, bool setScreen = false) = 0;

    virtual void displayView(bool forFile) = 0;
};

#endif
