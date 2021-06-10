#define __GAMEAGENT_HEADER__

#include <cstdlib>
#include <list>
#include <mutex>
#include <string>
#include <vector>

using namespace std;

class GameAgentBase {
  protected:
    int id;

  public:
    int getID();

    virtual void reset() = 0;
};
