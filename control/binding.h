#ifndef BINDING_H
#define BINDING_H

#include <string>
#include <map>

class CursesKeyBinding {
  public:
    virtual ~CursesKeyBinding() = 0;
    
    virtual int getKey(int input) = 0;
};

class NormalBinding: public CursesKeyBinding {
  public:
    int getKey(int input) override;
};

#endif
