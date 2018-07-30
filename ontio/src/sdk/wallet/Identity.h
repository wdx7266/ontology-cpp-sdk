#ifndef IDENTIFY_H
#define IDENTIFY_H

#if __cplusplus < 201103L
#error "use --std=c++11 option for compile."
#endif

#include <string>
#include <list>

#include "Control.h"

class Identity
{
  public:
    std::string label;
    std::string ontid;
    bool isDefault;
    bool lock;
    std::list<Control> controls;
    // Object extra;
    Identity()
    {
        label = "";
        ontid = "";
        isDefault = false;
        lock = false;
    }
};
#endif // !IDENTIFY_H