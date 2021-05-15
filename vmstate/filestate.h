#ifndef FILESTATE_H
#define FILESTATE_H

#include "cursorpos.h"
#include "filecontent/filecontent.h"
#include <string>
#include <vector>
#include <deque>
#include <memory>

class PermissionBehaviour {
  public:
    virtual ~PermissionBehaviour() = 0;
    
    virtual bool allowEdit(bool isReadOnly) = 0;
};

class StrictPermissionBehaviour: public PermissionBehaviour {
  public:
    bool allowEdit(bool isReadOnly) override;
};

class ChillPermissionBehaviour: public PermissionBehaviour {
  public:
    bool allowEdit(bool isReadOnly) override;
};

class AbstractFileState {
    std::string fname;
    bool readOnly;
    std::unique_ptr<PermissionBehaviour> behaviour;

  public:
    AbstractFileState(const std::string &fname, bool readOnly = false, std::unique_ptr<PermissionBehaviour> &&behaviour = std::make_unique<ChillPermissionBehaviour>());
    
    virtual ~AbstractFileState() = 0;

    std::string getFileName();
    bool isReadOnly();
    PermissionBehaviour *getPermissionBehaviour();
    void setPermissionBehaviour(std::unique_ptr<PermissionBehaviour> &&newBehaviour);

    virtual void saveFile() = 0;

    virtual bool changeMade() = 0;

    virtual void updateContent() = 0;
    virtual const std::vector<std::string> &getContent() = 0;

    virtual CursorPos getVirtualPos() = 0;
    virtual void setVirtualPos(size_t lineIndex, size_t charIndex) = 0;

    virtual CursorPos getCursorPos() = 0;

    virtual size_t getNumLines() = 0;
    virtual size_t getNumChars() = 0;

    virtual CursorPos findNext(std::string input) = 0;
    virtual CursorPos findPrevious(std::string input) = 0;

    virtual void moveCursor(size_t lineIndex, size_t charIndex) = 0;

    virtual void lmoveCursor(int amount) = 0;
    virtual void cmoveCursor(int amount) = 0;

    virtual void insertAfter(std::string input) = 0;
    virtual void insertBefore(std::string input) = 0;

    virtual void remove(int amount = -1) = 0;

    virtual void addLineAbove() = 0;
    virtual void addLineBelow() = 0;

    virtual void insertNewLine() = 0;

    virtual void deleteLine() = 0;
    virtual void deleteLines(int num) = 0;

    virtual std::string getLine() = 0;
    virtual std::string getLine(size_t lineIndex) = 0;
    virtual std::string getLineRelative(int offset) = 0;

    virtual std::vector<std::string> getLines(int num) = 0;
};

class FileState: public AbstractFileState {
    std::unique_ptr<AbstractFileContent> content;
    std::vector<std::string> contentCopy;
    CursorPos virtualPos;

    bool insertMode;

    std::vector<std::string> original;

  public:
    FileState(const std::string &fname);

    void saveFile() override;
    
    bool changeMade() override;

    void updateContent() override;
    const std::vector<std::string> &getContent() override;

    CursorPos getVirtualPos() override;
    void setVirtualPos(size_t lineIndex, size_t charIndex) override;

    CursorPos getCursorPos() override;

    size_t getNumLines() override;
    size_t getNumChars() override;

    CursorPos findNext(std::string input) override;
    CursorPos findPrevious(std::string input) override;

    void moveCursor(size_t lineIndex, size_t charIndex) override;

    void lmoveCursor(int amount) override;
    void cmoveCursor(int amount) override;

    void insertAfter(std::string input) override;
    void insertBefore(std::string input) override;

    void remove(int amount = -1) override;

    void addLineAbove() override;
    void addLineBelow() override;
    void insertNewLine() override;

    void deleteLine() override;
    void deleteLines(int num) override;

    std::string getLine() override;
    std::string getLine(size_t lineIndex) override;
    std::string getLineRelative(int offset) override;

    std::vector<std::string> getLines(int num) override;
};

#endif
