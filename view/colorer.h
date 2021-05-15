#ifndef COLORER_H
#define COLORER_H

#include "syntaxitem.h"
#include <map>

class Colorer {
    virtual short color(SyntaxItem input) = 0;

  public:
    virtual ~Colorer() = 0;

    short performColor(SyntaxItem input);
};

class VmColorer: public Colorer {
    std::map<SyntaxItem, short> colorMap;

    short color(SyntaxItem input) override;

  public:
    VmColorer();
};

class NoColor: public Colorer {
  short color(SyntaxItem input) override;
};

#endif
