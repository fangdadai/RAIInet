#include <iostream>
#include <string>
#include <sstream>
#include "DisplayBoard.h"
#include "Square.h"

using namespace std;

DisplayBoard::DisplayBoard(int playerNumber, bool graphics, vector<shared_ptr<Player>> players):
    playerNumber{playerNumber}, graphics{graphics}, players{players} {
    for (int i = 0; i < boardSize; ++i) {
        vector<char> row;
        for (int j = 0; j < boardSize; ++j) {
            if ((j == 3 || j == 4) && (i == 0 || i == 7)) {
                row.emplace_back('S');
                continue;
            }
            bool empty = true;
            int n = 0;
            for (int k = 0; k < 8; ++k) {
                int r = players[n]->links[k]->getRow();
                int c = players[n]->links[k]->getCol();
                if ((r == i) && (c == j)) {
                    empty = !empty;
                    char letter = players[n]->links[k]->getLetter();
                    row.emplace_back(letter);
                    break;
                }
                if (k == 7 && n == 0) {
                    k = -1;
                    n = 1;
                }
            }
            if (empty) {
                row.emplace_back('.');
            }
        }
        theDisplay.emplace_back(row);
    }
    if (graphics) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                char c = theDisplay[i][j];
                int x = j * 80;
                int y = i * 80;
                if (c == 'S') {
                    xw.fillRectangle(x, y, 80, 80, Xwindow::Red);
                } else if (c == 'm') {
                    xw.drawRectangle(x, y, 80, 80, Xwindow::Blue);
                } else if (c == 'w') {
                    xw.drawRectangle(x, y, 80, 80, Xwindow::Green);
                } else {
                    xw.fillRectangle(x, y, 80, 80, Xwindow::Black);
                }
                if (c >= 'a' && c <= 'h') {
                    xw.fillCircle(x, y, 40, Xwindow::Blue);
                } else if (c >= 'A' && c <= 'H') {
                    xw.fillCircle(x, y, 40, Xwindow::Green);
                }
            }
        }
    }
}

void DisplayBoard::updateDisplay(const Board& b) {
    playerNumber = b.getCurrent();
    vector<vector<char>> temp = theDisplay;
    theDisplay.clear();
    for (int i = 0; i < 8; ++i) {
        vector<char> row;
        for (int j = 0; j < 8; ++j) {
            char c = b.board[i][j].getContent();
            row.emplace_back(c);
            if (graphics) {
                char ch = temp[i][j];
                int x = j * 80;
                int y = i * 80;
                if (ch == c) {
                    continue;
                } else if (c == 'S') {
                    xw.fillRectangle(x, y, 80, 80, Xwindow::Red);
                } else if (c == 'm') {
                    if (!(b.board[i][j].getLinkOn())) {
                        xw.fillRectangle(x, y, 80, 80, Xwindow::Black);
                    }
                    xw.drawRectangle(x, y, 80, 80, Xwindow::Blue);
                } else if (c == 'w') {
                    if (!(b.board[i][j].getLinkOn())) {
                        xw.fillRectangle(x, y, 80, 80, Xwindow::Black);
                    }
                    xw.drawRectangle(x, y, 80, 80, Xwindow::Green);
                } else if (c == '.') {
                    xw.fillRectangle(x, y, 80, 80, Xwindow::Black);
                }
                if (c >= 'a' && c <= 'h') {
                    xw.fillCircle(x, y, 40, Xwindow::Blue);
                } else if (c >= 'A' && c <= 'H') {
                    xw.fillCircle(x, y, 40, Xwindow::Green);
                }
            }
        }
        theDisplay.emplace_back(row);
    }
}

string DisplayBoard::convert(AbilityName name) {
    if (name == LinkBoost) return "LinkBoost";
    else if (name == Firewall) return "Firewall";
    else if (name == Download) return "Download";
    else if (name == Polarize) return "Polarize";
    else if (name == Scan) return "Scan";
    else if (name == BoostStrength) return "BoostStrength";
    else if (name == ExchangeLocation) return "ExchangeLocation";
    else return "Taunt";
}

void DisplayBoard::printAbilities(int playerNumber) {
    cout << "               Player " << playerNumber << endl;
    cout << "------------- ABILITY CARD --------------" << endl;
    for (int i = 0; i < 5; ++i) {
        bool isUsed = players[playerNumber - 1]->abilities[i]->getIsUsed();
        string use;
        AbilityName abName = players[playerNumber - 1]->abilities[i]->getName();
        (isUsed)? use = "Yes" : use = "No";
        if (abName == Scan) {
            cout<<"ID: "<<i+1<<"   Name: "<<convert(abName)<<"               Used: "<<use<<endl; // just to make it align => looks better
        } else if (abName == LinkBoost) {
            cout<<"ID: "<<i+1<<"   Name: "<<convert(abName)<<"          Used: "<<use<<endl; // just to make it align => looks better
        } else if (abName == BoostStrength) {
            cout<<"ID: "<<i+1<<"   Name: "<<convert(abName)<<"      Used: "<<use<<endl; // just to make it align => looks better
        } else if (abName == ExchangeLocation) {
            cout<<"ID: "<<i+1<<"   Name: "<<convert(abName)<<"   Used: "<<use<<endl; // just to make it align => looks better
        } else if (abName == Taunt) {
            cout<<"ID: "<<i+1<<"   Name: "<<convert(abName)<<"              Used: "<<use<<endl; // just to make it align => looks better
        }
        else {
            cout << "ID: " << i + 1 <<
            "   Name: " << convert(abName) <<
            "           Used: " << use << endl;
        }
    }
    cout << "-----------------------------------------" << endl;
}

ostream &operator<<(ostream &out, const DisplayBoard &display) {
    out << display.players[0]->playerState();
    int number = display.playerNumber;
    if (number == 1) {
        out << display.players[0]->myViewLinks();
    } else {
        out << display.players[0]->oppoViewLinks();
    }
    out << "========" << endl;
	for (auto row : display.theDisplay) {
        for (auto c : row) {
            out << c;
        }
        out << endl;
    }
	out << "========" << endl;
    out << display.players[1]->playerState();
    if (number == 1) {
        out << display.players[1]->oppoViewLinks();
    } else {
        out << display.players[1]->myViewLinks();
    }
    return out;
}
