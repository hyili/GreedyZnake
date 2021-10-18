#define __GAMEAGENT_HEADER__

#include <cstdlib>
#include <mutex>
#include <string>
#include <vector>

#ifndef __TYPEDEF_HEADER__
#include "typedef.h"
#endif

class GameAgentBase {
  protected:
    int id;

  public:
    int getID();

    virtual void reset() = 0;
};
