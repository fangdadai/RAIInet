#include "Ability.h"
#include <string>
#include <iostream>

using namespace std;

Ability::Ability(int id, char initial): id{id} {
    if (initial == 'L') name = AbilityName::LinkBoost;
    else if (initial == 'F') name = AbilityName::Firewall;
    else if (initial == 'D') name = AbilityName::Download;
    else if (initial == 'P') name = AbilityName::Polarize;
    else if (initial == 'S') name = AbilityName::Scan;
    else if (initial == 'B') name = AbilityName::BoostStrength;
    else if (initial == 'E') name = AbilityName::ExchangeLocation;
    else if (initial == 'T') name = AbilityName::Taunt;
    else cout << "Invalid Ability Name" << endl;
}

AbilityName Ability::getName() const { return name; }
int Ability::getID() const { return id; }

bool Ability::getIsUsed() const { return isUsed; }

void Ability::toggleIsUsed() {  // negate
    (isUsed)? isUsed = false : isUsed = true;
}



