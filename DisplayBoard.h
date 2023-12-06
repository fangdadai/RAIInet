#ifndef __DISPLAYBOARD__H__
#define __DISPLAYBOARD__H__

#include <vector>
#include "Board.h"
#include "Window.h"

class Board;

class DisplayBoard {
    std::vector<std::vector<char>> theDisplay;
    int boardSize = 8;
    int playerNumber;
    Xwindow xw;
    bool graphics;
    std::vector<shared_ptr<Player>> players;
  public:
    DisplayBoard(int playerNumber, bool graphics, std::vector<shared_ptr<Player>> players);
    void updateDisplay(const Board& b);
    string convert(AbilityName name);
    void printAbilities(int playerNumber);
    friend std::ostream &operator<<(std::ostream &out, const DisplayBoard &display);
};

#endif
