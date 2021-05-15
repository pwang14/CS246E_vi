#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <vector>
#include <stack>

typedef struct LineChane{
    std::string line;
    bool added;
    size_t lineIndex;
    size_t charIndex;
    bool lineChanged = false;
} LineChange;

typedef struct Diff {
    std::vector<LineChange> changes;
} Diff;

class AbstractHistory {
  public:
    virtual ~AbstractHistory();

    virtual void addDiff(Diff diff) = 0;

    virtual bool isUndoEmpty() = 0;
    virtual Diff &getUndo() = 0;
    virtual void popUndo() = 0;

    virtual Diff undoAction() = 0;
    virtual Diff redoAction() = 0;
    
    virtual void clearRedo() = 0;
};

class History: public AbstractHistory {
    std::stack<Diff> undoStack;
    std::stack<Diff> redoStack;

  public:
    void addDiff(Diff diff) override;
    
    bool isUndoEmpty() override;
    Diff &getUndo() override;
    void popUndo() override;

    Diff undoAction() override;
    Diff redoAction() override;
    
    void clearRedo();
};

#endif
