#ifndef __PLAYER__H__
#define __PLAYER__H__

#include <vector>
#include <string>
#include <memory>
#include "Link.h"
#include "Ability.h"
using namespace std;

class Player {
    int playerNumber;
    int downloadedData = 0;  // the number of data that has been downloaded
    int downloadedVirus = 0;  // the number of virus that has been downloded
    int numAbility = 5; // the number of Abilities that has not been used
  public:
    vector<shared_ptr<Link>> links;
    vector<shared_ptr<Ability>> abilities;
    Player(int playerNumber, string AllLinks, string AllAbilities);
    ~Player();
    int getPlayerNumber() const;
    int getData() const;
    int getViruses() const;
    int getNumAbility() const;
    void IncreData();
    void IncreViruses();
    void DecreAbility();
    string playerState();
    string myViewLinks();
    string oppoViewLinks();
};

#endif
