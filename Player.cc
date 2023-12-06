#include <sstream>
#include "Player.h"

Player::Player(int playerNumber, string AllLinks, string AllAbilities): playerNumber{playerNumber} {
    char letter = (playerNumber == 1) ? 'a' : 'A';
    int n = AllLinks.length();
    int i = 0;
    for (int j = 0; j < n; j+=2) {
        char type = AllLinks[j];
        int strength = AllLinks[j+1] - '0';
        links.emplace_back(make_shared<Link>(type, letter, strength, playerNumber));
        links[i]->setCol(i);
        if (playerNumber == 1) {
            if (i == 3 || i == 4) {
                links[i]->setRow(1);
            } else {
                links[i]->setRow(0);
            }
        } else {
            if (i == 3 || i == 4) {
                links[i]->setRow(6);
            } else {
                links[i]->setRow(7);
            }
        }
        ++i;
        letter += 1;
    }
    int abLength = AllAbilities.length();
    for (int k = 0; k < abLength; ++k) {
        abilities.emplace_back(make_shared<Ability>(k + 1, AllAbilities[k]));
    }
}

Player::~Player() {}

int Player::getPlayerNumber() const { return playerNumber; }
int Player::getData() const { return downloadedData; }
int Player::getViruses() const { return downloadedVirus; }
int Player::getNumAbility() const { return numAbility; }
void Player::IncreData() { ++downloadedData; }
void Player::IncreViruses() { ++downloadedVirus; }
void Player::DecreAbility() { --numAbility; }

string Player::playerState() {
    ostringstream oss;
    oss << "Player " << playerNumber << ':' << endl;
    oss << "Downloaded: " << downloadedData << "D, " << downloadedVirus << 'V' << endl;
    oss << "Abilities: " << numAbility << endl;
    return oss.str();
}

string Player::myViewLinks() {
    ostringstream oss;
    char letter = (playerNumber == 1) ? 'a' : 'A';
    for (int i = 0; i < 8; ++i) {
        oss << letter << ": " << links[i]->getType() << links[i]->getStrength();
        if (i == 3 || i == 7) {
            oss << endl;
        } else {
            oss << ' ';
        }
        letter += 1;
    }
    return oss.str();
}

string Player::oppoViewLinks() {
    ostringstream oss1;
    ostringstream oss2;
    char letter1 = (playerNumber == 1) ? 'a' : 'A';
    char letter2 = (playerNumber == 1) ? 'e' : 'E';
    for (int i = 0; i < 4; ++i) {
        if (!(links[i]->getVisibility()) && !(links[i+4]->getVisibility())) {
            oss1 << letter1 << ": ?";
            oss2 << letter2 << ": ?";
        } else if (links[i]->getVisibility() && links[i+4]->getVisibility()) {
            oss1 << letter1 << ": " << links[i]->getType() << links[i]->getStrength();
            oss2 << letter2 << ": " << links[i+4]->getType() << links[i+4]->getStrength();
        } else if (links[i]->getVisibility()) {
            oss1 << letter1 << ": " << links[i]->getType() << links[i]->getStrength();
            oss2 << letter2 << ": ? ";
        } else {
            oss1 << letter1 << ": ? ";
            oss2 << letter2 << ": " << links[i+4]->getType() << links[i+4]->getStrength();
        }
        if (i != 3) {
            oss1 << ' ';
            oss2 << ' ';
        } else {
            oss1 << endl;
            oss2 << endl;
        }
        letter1 += 1;
        letter2 += 1;
    }
    return oss1.str() + oss2.str();
}
