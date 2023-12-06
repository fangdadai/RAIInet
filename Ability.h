#ifndef __ABILITY__H__
#define __ABILITY__H__

#include <string>

enum AbilityName {LinkBoost, Firewall, Download, Polarize, Scan, BoostStrength, ExchangeLocation, Taunt};
/*  Three Extra Abilities Description:
  1. BoostStrength: takes a char, increment the strength of the Link represented by that char by 1
  2. ExchangeLocation: takes two chars, exchange their locations.
        requirement: Links must be owned by the players themselves, must exist, must NOT sationed on a Firewall
  3. Taunt: takes two chars, let them start a legendary battle right away, even far part from each other
        requirement: Links must be owned by different players, must exist
*/

class Ability {
    AbilityName name;
    int id;
    bool isUsed = false;  // default
  public:
    // Constructor
    Ability(int id, char initial);

    // Accessor
    AbilityName getName() const;
    int getID() const;
    bool getIsUsed() const;

    // Mutator
    void toggleIsUsed();
};

#endif

