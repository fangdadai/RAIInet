#include "Board.h"

using namespace std;

Board::Board(string links1, string links2, string abilities1, string abilities2) {
    for (int i = 0; i < boardSize; ++i) {
        vector<Square> row;
        for (int j = 0; j < boardSize; ++j) {
            Square s{i, j};
            row.emplace_back(s);
        }
        board.emplace_back(row);
    }
    players.emplace_back(make_shared<Player>(1, links1, abilities1));
    players.emplace_back(make_shared<Player>(2, links2, abilities2));
}

int Board::getCurrent() const { return currentPlayer; }

int Board::getWinner() const { return winner; }

bool Board::getIsOver() const { return isOver; }

void Board::setWinner(int playerNumber) { winner = playerNumber; }

void Board::toggleIsOver() { isOver = !isOver; }

void Board::switchPlayer() {
    currentPlayer = (currentPlayer == 2) ? 1 : 2;
}

void Board::updateGameState(int playerNumber) {
    if (players[playerNumber - 1]->getData() == 4) {
        toggleIsOver();
        setWinner(playerNumber);
    } else if (players[playerNumber - 1]->getViruses() == 4) {
        toggleIsOver();
        if (playerNumber == 1) {
            setWinner(playerNumber + 1);
        } else {
            setWinner(playerNumber - 1);
        }
    }
}

void Board::moveLink(char letter, Direction dir){
    int i = (currentPlayer == 1) ? (letter - 'a') : (letter - 'A');
    auto lp = players[currentPlayer - 1]->links[i];
    if (lp->getState()) {
        cout << "The link has been downloaded" << endl;
        cout << endl;
        return;
    }
    int row = lp->getRow();
    int col = lp->getCol();
    int newRow = row;
    int newCol = col;

    switch (dir) {
        case UP:    newRow -= lp->getRange(); break;
        case DOWN:  newRow += lp->getRange(); break;
        case LEFT:  newCol -= lp->getRange(); break;
        case RIGHT: newCol += lp->getRange(); break;
    }
    if ((newRow == 0 || newRow == 7) && (newCol == 3 || newCol == 4)) {  // move on to server port
        if ((currentPlayer == 1 && newRow == 7) || (currentPlayer == 2 && newRow == 0)) {
            downloadLink(letter);
        } else {
            cout << "You cannot move your link on top of your own server ports" << endl;
            return;
        }
    } else if (newRow >= 0 && newRow < boardSize && newCol >= 0 && newCol < boardSize) {  // valid move
        char otherLetter = board[newRow][newCol].getContent();
        bool isLinkOn = board[newRow][newCol].getLinkOn();
        if (board[newRow][newCol].getIsFW()) {  // move onto a Firewall
            // And another Link owned by oneself is already stationed in that Firewall
            if (isLinkOn) {
                if (currentPlayer == 1 && (otherLetter >= 'a' && otherLetter <= 'h')) {
                    cout << "You cannot move a link on top of another link owned by yourself" << endl;
                    return;
                } else if (currentPlayer == 2 && (otherLetter >= 'A' && otherLetter <= 'H')) {
                    cout << "You cannot move a link on top of another link owned by yourself" << endl;
                    return;
                }
            }
            // (p1 OR p2)-Link moves onto his/her/their/its/other's own Firewall nothing happens
            int owner = board[newRow][newCol].getFwOnwer();
            if ((currentPlayer == 1 && owner == 1) || (currentPlayer == 2 && owner == 2)) {  
                board[newRow][newCol].toggleLinkOn();        
                board[newRow][newCol].setContent(letter);
            } 
            // p1 Link moves onto p2 Firewall, revealed and downloaded by p1 if it's a Virus-Link
            else if (currentPlayer == 1 && owner == 2) {  
                if (players[0]->links[letter - 'a']->getType() == 'D') {  // Data-Link
                    board[newRow][newCol].toggleLinkOn();
                    players[0]->links[letter - 'a']->toggleVisbility();   // just reaveal
                    board[newRow][newCol].setContent(letter);
                } else {  // Virus-Link
                    ownDownload(letter);  // reveal and download (download means it is revealed as well)
                }
            }
            // p2 Link moves onto p1 Firewall
            else if (currentPlayer == 2 && owner == 1) {  
                if (players[1]->links[letter - 'A']->getType() == 'D') {  // Data-Link
                    board[newRow][newCol].toggleLinkOn();
                    players[1]->links[letter - 'A']->toggleVisbility();   // just reaveal
                    board[newRow][newCol].setContent(letter);
                } else {  // Virus-Link
                    ownDownload(letter);  // reveal and download (download means it is revealed as well)
                }
            }
        } else if (!isLinkOn) {  // move onto an empty square
            board[newRow][newCol].toggleLinkOn();
            board[newRow][newCol].setContent(letter);
        } else if (isLinkOn) {  // move onto a square where there is already a Link
            if (currentPlayer == 1 && (otherLetter >= 'a' && otherLetter <= 'h')) {
                cout << "You cannot move a link on top of another link owned by yourself" << endl;
                return;
            } else if (currentPlayer == 2 && (otherLetter >= 'A' && otherLetter <= 'H')) {
                cout << "You cannot move a link on top of another link owned by yourself" << endl;
                return;
            } else {  // [A~H] moves ontop of [a~h], battle
                if(battle(letter, otherLetter)) {
                    board[newRow][newCol].setContent(letter);
                }
            }
        }
    } else if ((newRow >= 8 && currentPlayer == 1) || (newRow <= -1 && currentPlayer == 2)) {  // move off from opponent's egde
        ownDownload(letter);
    } else {  // move off from other egdes => i.e. invalid move
        cout << "Invalid move! Out of boundary" << endl;
        return;
    }

    if (board[row][col].getIsFW()) {
        board[row][col].setContent(board[row][col].getFwContent());
        board[row][col].toggleLinkOn();
    } else {
        board[row][col].setContent('.');
        board[row][col].toggleLinkOn();
    }
    lp->setRow(newRow);
    lp->setCol(newCol);
    switchPlayer();
}

bool Board::battle(char letter1, char letter2) {
    int i;
    int j;
    if (currentPlayer == 1) {
        i = letter1 - 'a';
        j = letter2 - 'A';
    } else {
        i = letter1 - 'A';
        j = letter2 - 'a';
    }
    int index = (currentPlayer == 1) ? 0 : 1;
    players[index]->links[i]->toggleVisbility();
    players[1 - index]->links[j]->toggleVisbility();
    int strength1 = players[index]->links[i]->getStrength();
    int strength2 = players[1 - index]->links[j]->getStrength();
    if (strength1 >= strength2) {
        downloadLink(letter2);
        return true;
    } else {
        downloadLink(letter1);
        return false;
    }
}

void Board::downloadLink(char letter) {
    int index;
    int i;
    if (letter >= 'A' && letter <= 'H') {
        index = letter - 'A';
        i = 0;
    } else {
        index = letter - 'a';
        i = 1;
    }
    players[1 - i]->links[index]->downloaded();
    if (players[1 - i]->links[index]->getType() == 'D') {
        players[i]->IncreData();
    } else {
        players[i]->IncreViruses();
    }
    updateGameState(i + 1);
}

void Board::ownDownload(char letter) {
    int index;
    int i;
    if (letter >= 'A' && letter <= 'H') {
        index = letter - 'A';
        i = 1;
    } else {
        index = letter - 'a';
        i = 0;
    }
    players[i]->links[index]->downloaded();
    if (players[i]->links[index]->getType() == 'D') {
        players[i]->IncreData();
    } else {
        players[i]->IncreViruses();
    }
    updateGameState(i + 1);
}


bool Board::useAbility(int ID, istream& in) {
    if (ID < 1 || ID > 5) {
        cout << "Invalid Ability ID: Please enter a number between 1 and 5" << endl;
        return false;
    }
    int curr = getCurrent();
    if (players[curr - 1]->abilities[ID - 1]->getIsUsed()) {
        cout << "Invalid Ability: This Ability has been used... QwQ" << endl;
        return false;
    }
    AbilityName abName = players[curr - 1]->abilities[ID - 1]->getName();

    // LinkBoost
    if (abName == AbilityName::LinkBoost) {
        char linkLetter;
        if (in >> linkLetter) {
            // check if the Link belongs to the Player who used LinkBoost
            if (linkLetter >= 'a' && linkLetter <= 'h' && curr == 1) {
                // check if the Link represented by <linkLetter> still exists
                if (players[curr - 1]->links[linkLetter - 'a']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    // use ability
                    players[curr - 1]->links[linkLetter - 'a']->boostRange();
                    // update info
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                }
            } else if (linkLetter >= 'A' && linkLetter <= 'H' && curr == 2) {
                if (players[curr - 1]->links[linkLetter - 'A']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    players[curr - 1]->links[linkLetter - 'A']->boostRange();
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                }
            }
        }
    }

    // Download
    else if (abName == AbilityName::Download) {
        char oppoLink;
        if (in >> oppoLink) {
            // check if the Link represented by <oppoLink> actually belongs to the opponent
            if (curr == 1 && oppoLink >= 'A' && oppoLink <= 'H') {
                // check if the Link represented by <oppoLink> still exists
                if (players[1]->links[oppoLink - 'A']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                }
                auto theLink = players[1]->links[oppoLink - 'A'];
                int row = theLink->getRow();
                int col = theLink->getCol();
                if (players[1]->links[oppoLink - 'A']->getVisibility()) {
                    downloadLink(oppoLink);
                } else {
                    downloadLink(oppoLink);
                    players[1]->links[oppoLink - 'A']->setVisFalse();
                }
                // update
                board[row][col].setContent('.');
                board[row][col].toggleLinkOn();
                players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                players[curr - 1]->DecreAbility();
            } else if (curr == 2 && oppoLink >= 'a' && oppoLink <= 'h') {
                if (players[0]->links[oppoLink - 'a']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                }
                auto theLink = players[0]->links[oppoLink - 'a'];
                int row = theLink->getRow();
                int col = theLink->getCol();

                if (players[0]->links[oppoLink - 'a']->getVisibility()) {
                    downloadLink(oppoLink);
                } else {
                    downloadLink(oppoLink);
                    players[0]->links[oppoLink - 'a']->setVisFalse();
                }
                // update
                board[row][col].setContent('.');
                board[row][col].toggleLinkOn();
                players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                players[curr - 1]->DecreAbility();
            } else {
                cout << "Invalid Link: Either doesn't exist or isn't owned by your opponenet" << endl;
                return false;
            }
        }
    } 
    
    // Polarize
    else if (abName == AbilityName::Polarize) {
        char linkLetter;
        if (in >> linkLetter) {
            // check if the Link is in the range of [a~h] or [A~H]
            if ((linkLetter < 'a' && linkLetter > 'h') && (linkLetter < 'A' && linkLetter > 'H')) {
                cout << "Invalid Link: This Link doesn't exist" << endl;
                return false;
            } else if (linkLetter >= 'a' && linkLetter <= 'h') {  // in range, yeah~ :3 
                // check if the Link represented by <linkLetter> has been downloaded
                if (players[0]->links[linkLetter - 'a']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    players[0]->links[linkLetter - 'a']->toggleType();
                    // update
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                    
                }
            } else if (linkLetter >= 'A' && linkLetter <= 'H' && curr == 2) {
                if (players[curr - 1]->links[linkLetter - 'A']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    players[1]->links[linkLetter - 'A']->toggleType();
                    // update
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                }
            }
        }
    }

    // Scan
    else if (abName == AbilityName::Scan) {
        char linkLetter;
        if (in >> linkLetter) {
            // check if the Link is in the range of [a~h] or [A~H]
            if ((linkLetter < 'a' && linkLetter > 'h') && (linkLetter < 'A' && linkLetter > 'H')) {
                cout << "Invalid Link: This Link doesn't exist" << endl;
                return false;
            } else if (linkLetter >= 'a' && linkLetter <= 'h') {
                // check if the Link represented by <linkLetter> has been downloaded
                if (players[0]->links[linkLetter - 'a']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    players[0]->links[linkLetter - 'a']->toggleVisbility();
                    // update
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                }
            } else if (linkLetter >= 'A' && linkLetter <= 'H') {
                if (players[curr - 1]->links[linkLetter - 'A']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    players[1]->links[linkLetter - 'A']->toggleVisbility();
                    // update
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                }
            }
        }
    }

    // Firewall
    else if (abName == AbilityName::Firewall) {
        int row, col;
        if (in >> row && in >> col) {
            // first check is the square at (row,col) is empty or not
            if (board[row][col].getLinkOn()) {
                cout << "This square is currently occupied by a Link" << endl;
                return false;
            } 
            if (curr == 1) {
                board[row][col].toggleIsFW();  // indicate the square is a Firewall
                board[row][col].setContent('m'); // m for player 1's Firewall
                board[row][col].setFwContent('m');
                board[row][col].setFwOwner(1);
                // update
                players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                players[curr - 1]->DecreAbility();
            } else if (curr == 2) {
                board[row][col].toggleIsFW();
                board[row][col].setContent('w'); // w for player 2's Firewall
                board[row][col].setFwContent('w');
                board[row][col].setFwOwner(2);
                // update
                players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                players[curr - 1]->DecreAbility();
            } 
        }
    } 

    // BoostStrength
    else if (abName == AbilityName::BoostStrength) {
        char linkLetter;
        if (in >> linkLetter) {
            // check if the Link belongs to the Player who used BoostStrength
            if (linkLetter >= 'a' && linkLetter <= 'h' && curr == 1) {
                // check if the Link represented by <linkLetter> still exists
                if (players[curr - 1]->links[linkLetter - 'a']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    // use ability
                    players[curr - 1]->links[linkLetter - 'a']->incStrength();
                    // update info
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                }
            } else if (linkLetter >= 'A' && linkLetter <= 'H' && curr == 2) {
                if (players[curr - 1]->links[linkLetter - 'A']->getState()) {
                    cout << "Invalid Link: This Link has been downloaded" << endl;
                    return false;
                } else {
                    players[curr - 1]->links[linkLetter - 'A']->incStrength();
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                }
            }
        }
    }

    // ExchangeLocation
    else if (abName == AbilityName::ExchangeLocation) {
        char link1, link2;
        if (in >> link1 && in >> link2) {
            // check if the two Links belongs to the Player who used ExchangeLocation
            if (curr == 1 && link1 >= 'a' && link1 <= 'h' && link2 >= 'a' && link2 <= 'h') {
                if (players[curr - 1]->links[link1 - 'a']->getState() || players[curr - 1]->links[link2 - 'a']->getState()) {
                    cout << "Invalid Links: Some of the Links have been downloaded" << endl;
                    return false;
                } else {

                    int tempRow1 = players[curr - 1]->links[link1 - 'a']->getRow();
                    int tempCol1 = players[curr - 1]->links[link1 - 'a']->getCol();
                    int tempRow2 = players[curr - 1]->links[link2 - 'a']->getRow();
                    int tempCol2 = players[curr - 1]->links[link2 - 'a']->getCol();
                    board[tempRow2][tempCol2].setContent(link1);
                    board[tempRow1][tempCol1].setContent(link2);
                    // // exchange
                    players[curr - 1]->links[link1 - 'a']->setRow(tempRow2);
                    players[curr - 1]->links[link1 - 'a']->setCol(tempCol2);
                    players[curr - 1]->links[link2 - 'a']->setRow(tempRow1);
                    players[curr - 1]->links[link2 - 'a']->setCol(tempCol1);

                    // update
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                }
            } else if (curr == 2 && link1 >= 'A' && link1 <= 'H' && link2 >= 'A' && link2 <= 'H') {
                if (players[curr - 1]->links[link1 - 'A']->getState() || players[curr - 1]->links[link2 - 'A']->getState()) {
                    cout << "Invalid Links: Some of the Links have been downloaded" << endl;
                    return false;
                } else {
                    int tempRow1 = players[curr - 1]->links[link1 - 'A']->getRow();
                    int tempCol1 = players[curr - 1]->links[link1 - 'A']->getCol();
                    int tempRow2 = players[curr - 1]->links[link2 - 'A']->getRow();
                    int tempCol2 = players[curr - 1]->links[link2 - 'A']->getCol();
                    board[tempRow2][tempCol2].setContent(link1);
                    board[tempRow1][tempCol1].setContent(link2);
                    players[curr - 1]->links[link1 - 'A']->setRow(tempRow2);
                    players[curr - 1]->links[link1 - 'A']->setCol(tempCol2);
                    players[curr - 1]->links[link2 - 'A']->setRow(tempRow1);
                    players[curr - 1]->links[link2 - 'A']->setCol(tempCol1);
                    // update
                    players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                    players[curr - 1]->DecreAbility();
                    return true;
                }
            } else {
                cout << "You do not own both of: " << link1 << " and " << link2 << endl;
                return false;
            }
        }
    }

    // Taunt
    else if (abName == AbilityName::Taunt) {  // cannot be owned by the same player
        char link1, link2;
        if (in >> link1 && in >> link2) {
            if ((curr == 1 && (link1 < 'a' || link1 > 'h')) || (curr == 2 && (link1 < 'A' || link1 > 'H'))) {
                cout << "You do not have the link " << link1 << endl;
                return false;
            } else if ((curr == 1 && (link2 < 'A' || link2 > 'H')) || (curr == 2 && (link2 < 'a' || link2 > 'h'))) {
                cout << "Your opponent does not have the link " << link2 << endl;
                return false;
            } else {
                int row;
                int col;
                if (battle(link1, link2)) {
                    if (curr == 1) {
                        row = players[1]->links[link2 - 'A']->getRow();
                        col = players[1]->links[link2 - 'A']->getCol();
                    } else {
                        row = players[0]->links[link2 - 'a']->getRow();
                        col = players[0]->links[link2 - 'a']->getCol();
                    }
                } else {
                    if (curr == 1) {
                        row = players[0]->links[link1 - 'a']->getRow();
                        col = players[0]->links[link1 - 'a']->getCol();
                    } else {
                        row = players[1]->links[link1 - 'A']->getRow();
                        col = players[1]->links[link1 - 'A']->getCol();
                    }
                }
                board[row][col].setContent('.');
                board[row][col].toggleLinkOn();
                players[curr - 1]->abilities[ID - 1]->toggleIsUsed();
                players[curr - 1]->DecreAbility();
                return true;
            }
        }
    }

    // Invalid Ability Name
    else {
        cout << "Unknown Ability" << endl;
        return false;
    }
    return true;
}
