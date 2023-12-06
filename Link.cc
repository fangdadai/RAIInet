#include "Link.h"


using namespace std;

Link::Link(char type, char letter, int strength, int owner):
    type{type}, letter{letter}, strength{strength}, owner{owner} {}

// Accessor
char Link::getType() const { return type; } 
char Link::getLetter() const { return letter; }
int Link::getStrength() const { return strength; }
int Link::getRow() const { return row; }
int Link::getCol() const { return col; }
int Link::getOwner() const { return owner; }
int Link::getRange() const { return range; }
bool Link::getState() const { return isDownload; }
bool Link::getVisibility() const { return visibility; }

// Mutator
void Link::toggleType() {
    (type == 'D') ? type = 'V' : type = 'D';  // change 'D' to 'V', 'V' to 'D'
}

void Link::boostRange() { range = 2; }  // change the range to 2, after using "LinkBoost"
void Link::setRow(int r) { row = r; }
void Link::setCol(int c) { col = c; }
void Link::toggleState() {
    isDownload = (isDownload) ? false : true;  // negate the isDownload field
}
void Link::toggleVisbility() { visibility = true; }
void Link::setVisFalse() { 
    visibility = false; 
}

void Link::downloaded() {
    toggleState();
    toggleVisbility();
}

void Link::incStrength() {
    ++strength;
}

void Link::setType(char t) {
    type = t;
}

void Link::setVis(bool b) {
    visibility = b;
}

void Link::setStrength(int s) {
    strength = s;
}


