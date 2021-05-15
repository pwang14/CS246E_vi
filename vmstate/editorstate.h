#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "cursorpos.h"
#include <string>
#include <unordered_map>

class AbstractEditorState {
  public:
    virtual ~AbstractEditorState() = 0;

    virtual const std::string &getFileName() = 0;
    virtual void setFileName(std::string fname) = 0;

    virtual const std::string &getCommandBar() = 0;
    virtual void setCommandBar(std::string input) = 0;

    virtual size_t getCursor() = 0;
    virtual void setCursor(size_t pos) = 0;

    virtual bool previousInsertion() = 0;
    virtual void setInsertion(bool input) = 0;

    virtual std::string getSearchTerm() = 0;
    virtual void setSearchTerm(std::string input) = 0;

    virtual bool getForwardSearch() = 0;
    virtual void setForwardSearch(bool forward) = 0;

    virtual bool cursorOn() = 0;
    virtual void setCursorOn(bool input) = 0;

    virtual std::unordered_map<size_t, std::unordered_map<size_t, char>> &getReplacementMap() = 0;
    virtual void clearReplacementMap() = 0;

    virtual bool getLastFForward() = 0;
    virtual void setLastFFoward(bool input) = 0;

    virtual char getLastFChar() = 0;
    virtual void setLastFChar(char c) = 0; 

    virtual int getMaxY() = 0;
    virtual int getMaxX() = 0;
    virtual void setMax(int y, int x) = 0;
};

class EditorState: public AbstractEditorState {
    std::string fileName;

    int maxY;
    int maxX;

    std::string commandBar;
    size_t cursorPos;

    bool insertion;

    bool cursorVisibility;
    std::string searchTerm;
    bool forwardSearch;

    std::unordered_map<size_t, std::unordered_map<size_t, char>> replacementMap;

    bool lastFFoward = true;
    char lastFChar = static_cast<char>(0);

  public:
    EditorState(std::string fname, int maxY, int maxX);

    const std::string &getFileName() override;
    void setFileName(std::string fname) override;

    const std::string &getCommandBar() override;
    void setCommandBar(std::string input) override;

    size_t getCursor() override;
    void setCursor(size_t pos) override;

    bool previousInsertion() override;
    void setInsertion(bool input) override;

    std::string getSearchTerm() override;
    void setSearchTerm(std::string input) override;

    bool getForwardSearch() override;
    void setForwardSearch(bool forward) override;

    bool cursorOn() override;
    void setCursorOn(bool input) override;

    std::unordered_map<size_t, std::unordered_map<size_t, char>> &getReplacementMap() override;
    void clearReplacementMap() override;

    bool getLastFForward() override;
    void setLastFFoward(bool input) override;

    char getLastFChar() override;
    void setLastFChar(char c) override; 

    int getMaxY() override;
    int getMaxX() override;
    void setMax(int y, int x) override;
};

#endif
