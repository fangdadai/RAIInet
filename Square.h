#ifndef __SQUARE__H__
#define __SQUARE__H__

// This class is used to represent each of the square (in total of 64) on the Board
class Square {
    int row, col;  // position
    char content;  // <A~H>, <a~h>, <.>, <m or w> 
    char fwContent;
    bool linkOn = false;  // whether there is a Link placed on this Square
    bool isSP = false;  // whether this Square is a Server Port
    int fwOwner;
    bool isFW = false;
  public:
    Square (int r, int c);  // ctor to give the position of the Square

    // Accessor
    int getRow() const;
    int getCol() const;
    char getContent() const;
    char getFwContent() const;
    bool getLinkOn() const;
    bool getIsSP() const;
    bool getIsFW() const;
    int getFwOnwer() const;

    // Mutator
    void setContent(char newContent);
    void setFwContent(char newFW);
    void toggleLinkOn();
    void toggleIsFW();
    void setFwOwner(int owner);
};

#endif
