#ifndef __BOARD__H__
#define __BOARD__H__

#include <vector>
#include <memory>
#include "Player.h"
#include "Square.h"


// used for moving a Link
enum Direction {UP, DOWN, LEFT, RIGHT};

class Board {
    int boardSize = 8;
    int currentPlayer = 1;  // Track the current player (p1 or p2), start with player 1
    bool isOver = false;  // Flag to indicate if the game is over
    int winner = 0;

  public:
    vector<vector<Square>> board;
    vector<shared_ptr<Player>> players;
    // Constructor:
    Board(string links1, string links2, string abilities1, string abilities2);
    // Accessor:
    int getCurrent() const;
    bool getIsOver() const;
	  int getWinner() const;

    // Mutator:
    void setWinner(int playerNumber);
    void toggleIsOver();
    void switchPlayer();
    void updateGameState(int playerNumber);
    // Other methods:
    void moveLink(char letter, Direction dir);  // Move a link on the board
    bool battle(char letter1, char letter2);
    void downloadLink(char letter);
    void ownDownload(char letter);
    bool useAbility(int ID, istream& in); // return whether used an Ability successfully or not
};

#endif
