#ifndef MODEL_H
#define MODEL_H

#include "view.h"
#include "controller.h"
#include "action.h"

#include "vmstate/filestate.h"
#include "vmstate/clipboard.h"
#include "vmstate/history.h"
#include "vmstate/editorstate.h"

#include <string>
#include <vector>
#include <memory>

class View;

class Controller;

class Action;

class Model {
    std::vector<View*> views;
    Controller* controller;

    bool exitVal = false;

  protected:
    std::vector<Action*> getAction();

  public:
    virtual ~Model();

    virtual void run() = 0;

    void addView(View *view);
    void addController(Controller *control);

    void updateViews(bool forFile);
    void updateViewsLine(size_t lineIndex, int previousLength);
    void updateViewsBlock(size_t startIndex);
    void updateViewsCursor(bool forFile, size_t lineIndex, size_t charIndex, bool setScreen = false);
    
    void displayViews(bool forFile);

    void initialDisplay();

    void exit();
    bool getExit();
};

class VmState: public Model {
    AbstractFileState *currentFile;
    std::vector<std::unique_ptr<AbstractFileState>> files;

    std::unique_ptr<AbstractClipboard> clipboard;
    std::unique_ptr<AbstractHistory> history;
    std::unique_ptr<AbstractEditorState> editor;

  public:
    VmState(const std::string &fname, int maxY = 0, int maxX = 0);
    
    void run() override;

    AbstractFileState *getFile(size_t index);
    AbstractFileState *getCurrentFile();
    AbstractClipboard *getClipboard();
    AbstractHistory *getHistory();
    AbstractEditorState *getEditor();
};

#endif
