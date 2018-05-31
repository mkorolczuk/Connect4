/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: michal
 *
 * Created on 28 maja 2018, 18:16
 */

#include <cstdlib>
#include <iostream>

using namespace std;

#define ROWS 6
#define COLS 7
#define PLAYER 'X'
#define COMPUTER 'Y'

bool isInvalidMove(char board[ROWS][COLS], int col) {
    if(col >= 0 && col < COLS) {
        for(int i = ROWS - 1; i >= 0; i--) {
            if(board[i][col] != PLAYER && board[i][col] != COMPUTER) {
                return false;
            }
        }
    }
    cout << "Błędna kolumna!!" << endl;
    return true;
}
bool checkVertically(char board[ROWS][COLS], char pawn, int row, int col) {
    int count = 1;
    for(int i = row + 1; i < ROWS; i++) {
        if(board[i][col] == pawn) {
            count++;
        } else {
            break;
        }
    }
    for(int i = row - 1; i >=0; i--) {
        if(board[i][col] == pawn) {
            count++;
        } else {
            break;
        }
    }
    return count > 3 ? true : false;
}
bool checkHorizontally(char board[ROWS][COLS], char pawn, int row, int col) {
    int count = 1;
    for(int i = col + 1; i < COLS; i++) {
        if(board[row][i] == pawn) {
            count++;
        } else {
            break;
        }
    }
    for(int i = col - 1; i >=0; i--) {
        if(board[row][i] == pawn) {
            count++;
        } else {
            break;
        }
    }
    return count > 3 ? true : false;
}
bool checkDiagonally(char board[ROWS][COLS], char pawn, int row, int col) {
    int count = 1;
    int i = row + 1;
    int j = col + 1;
    while(i < ROWS && j < COLS) {
        if(board[i][j] == pawn) {
            count++;
        } else {
            break;
        }
        i++;
        j++;
    }
    i = row - 1;
    j = col - 1;
    while(i >= 0 && j >=0) {
        if(board[i][j] == pawn) {
            count++;
        } else {
            break;
        }
        i--;
        j--;
    }
    count = 1;
    i = row + 1;
    j = col - 1;
    while(i < ROWS && j >= 0) {
        if(board[i][j] == pawn) {
            count++;
        } else {
            break;
        }
        i++;
        j--;
    }
    i = row - 1;
    j = col + 1;
    while(i >= 0 && j < COLS) {
        if(board[i][j] == pawn) {
            count++;
        } else {
            break;
        }
        i--;
        j++;
    }
    return count > 3 ? true : false;
}
bool checkForWinner(char board[ROWS][COLS], bool isPlayer, int row, int col) {
    char pawn = isPlayer ? PLAYER : COMPUTER;
    if(checkVertically(board, pawn, row, col)) {
        return true;
    }
    if(checkHorizontally(board, pawn, row, col)) {
        return true;
    }
    if(checkDiagonally(board, pawn, row, col)) {
        return true;
    }
    return false;
}
void drawBoard(char board[ROWS][COLS]) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}
int playerMove(char board[ROWS][COLS], int col) {
    for(int i = ROWS - 1; i >= 0; i--) {
        if(board[i][col] != PLAYER && board[i][col] != COMPUTER) {
            board[i][col] = PLAYER;
            return i;
        }
    }
    return -1;
}
int minimax(char board[ROWS][COLS]) {
    
}
bool computerMove(char board[ROWS][COLS]) {
    srand (time(NULL));
    int col = rand() % 6;
    int x, y;
    for(int i = 5; i >= 0; i--) {
        if(board[i][col] != PLAYER && board[i][col] != COMPUTER) {
            board[i][col] = COMPUTER;
            x = i;
            y = col;
            break;
        }
    }
    return checkForWinner(board, false, x, y);
}
int main(int argc, char** argv) {
    char board[ROWS][COLS];
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            board[i][j] = '0';
        }
    }
    int col, row;
    bool isWinner = false;
    while(!isWinner) {
       drawBoard(board);
       do {
           cout << "Wybierz numer kolumny" << endl;
           cin >> col;
       } while(isInvalidMove(board, col - 1));
       row = playerMove(board, col - 1);
       isWinner = checkForWinner(board, true, row, col-1);
       if(isWinner) {
           cout << "Zwyciężył gracz numer 1" << endl;
           drawBoard(board);
           break;
       }
       isWinner = computerMove(board);
       if(isWinner) {
           cout << "Zwyciężył gracz numer 2" << endl;
           drawBoard(board);
           break;
       }
    }
    return 0;
}

