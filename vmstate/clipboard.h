#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <string>
#include <vector>

class AbstractClipboard {
  public:
    virtual ~AbstractClipboard() = 0;

    virtual void clearClipboard() = 0;
    virtual void appendToClipboard(std::string input) = 0;
    virtual const std::vector<std::string> &getClipboard() = 0;
};

class Clipboard: public AbstractClipboard {
    std::vector<std::string> data;

  public:
    void clearClipboard() override;
    void appendToClipboard(std::string input) override;
    const std::vector<std::string> &getClipboard() override;
};

#endif
