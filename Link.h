#ifndef __LINK__H__
#define __LINK__H__
#include <iostream>
#include <vector>

class Link {
    char type;  // 'D' for data, 'V' for virus
    char letter; 
    int strength;  // strength of the Link, [1, 2, 3, 4]
    int row, col;  // position of the Link
    int owner; // owner of the Linker, this indicates the Player Number
    // the following fields have default values
    int range = 1;  // how many grid can a Link move at a time
    bool visibility = false;  // whether the Link is visible to the opponent
    bool isDownload = false;  // whether the Link has been downloaded

  public:
    // Constructor, default: range == 1, visibility == isDownload == false
    Link(char type, char letter, int strength, int owner);
    
    // Accessor
    char getType() const;
    char getLetter() const;
    int getStrength() const;
    int getRow() const;
    int getCol() const;
    int getRange() const;
    int getOwner() const;
    bool getState() const;  // return isDownload field
    bool getVisibility() const;

    // Mutator
    void toggleType();
    void boostRange();  // for the purpose of the ability "LinkBoost"
    void setRow(int r);
    void setCol(int c);
    void toggleState();
    void toggleVisbility();
    void downloaded();
    void incStrength();
    void setType(char t);
    void setVis(bool b);
    void setStrength(int s);
    void setVisFalse();
};

#endif
