#include "Square.h"

Square::Square(int r, int c): row{r}, col{c} {
    if (row == 0 && ((col == 3) || (col == 4))) {  // position of the Server Port (Player 0)
        content = 'S';
        isSP = true;
    } else if (row == 7  && ((col == 3) || (col == 4))) {  // position of the Server Port (Player 1)
        content = 'S';
        isSP = true;
    } else if ((row == 0 && col <= 2) || 
               (row == 1 && (col >= 3 && col <= 4)) || 
               (row == 0 && col >= 5)) {  // positions for Links at initialization (Player 0)
        content = 'a' + col;
        linkOn = true;
    } else if ((row == 7 && col <= 2) || 
               (row == 6 && (col >= 3 && col <= 4)) || 
               (row == 7 && col >= 5)) {  // positions for Links at initialization (Player 1)
        content = 'A' + col;
        linkOn = true;
    } else {
        content = '.';  // no Link is placed at this position at initialization
    }
}
// Accessor
int Square::getRow() const { return row; }
char Square::getContent() const { return content; }
char Square::getFwContent() const { return fwContent; }
bool Square::getLinkOn() const { return linkOn; }
bool Square::getIsSP() const { return isSP; }
bool Square::getIsFW() const { return isFW; }
int Square::getFwOnwer() const { return fwOwner; }

// Mutator
void Square::setContent(char newContent) { content = newContent; }
void Square::toggleLinkOn() {
    linkOn? linkOn = false : linkOn = true;
}
void Square::toggleIsFW() {
    isFW? isFW = false : isFW = true;
}
void Square::setFwContent(char newFW) { fwContent = newFW; }

void Square::setFwOwner(int owner) { fwOwner = owner; }
