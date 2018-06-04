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

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <climits>
#include "omp.h"

using namespace std;

#define ROWS 6
#define COLS 7
#define DEPTH 5
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
void drawBoard(char board[ROWS][COLS]) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}
bool checkForWinner(char board[ROWS][COLS], bool isPlayer, int row, int col) {
    if(row >= 0 && row < ROWS && col >= 0 && col < COLS) {
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
    }
    return false;
}
int makeMove(char board[ROWS][COLS], int col, bool isPlayer) {
    char pawn = isPlayer ? PLAYER : COMPUTER;
    for(int i = ROWS - 1; i >= 0; i--) {
        if(board[i][col] != PLAYER && board[i][col] != COMPUTER) {
            board[i][col] = pawn;
            return i;
        }
    }
    return -1;
}
vector<int> getPossibleMoves(char board[ROWS][COLS]) {
    vector<int> moves;
    for(int i = 0; i < COLS; i++) {
        if(!isInvalidMove(board, i)) {
            moves.push_back(i);
        }
    }
    return moves;
}
int generateRandomMove(int scores[COLS]){
    int move;
    srand (time(NULL));
    do {
        move = rand() % 7;
    } while(scores[move] != 0);
    return move;
}
int minimax(char board[ROWS][COLS], int depth, bool isMaxPlayer, int row, int col) {
    vector<int> moves = getPossibleMoves(board);
    if(checkForWinner(board, false, row, col) && !isMaxPlayer) {
        return 1;
    }
    if(checkForWinner(board, true, row, col) && isMaxPlayer) {
        return -1;
    }
    if(depth == DEPTH || moves.size() == 0) {
        return 0;
    }
    depth++;
    if(isMaxPlayer) {
        int bestScore = INT_MIN;
        int score;
        #pragma omp parallel for private(score) shared(board, bestScore, moves, depth) reduction(max:score)
        for(int i = 0; i < moves.size(); i++) {
            char boardCopy[ROWS][COLS];
            copy(&board[0][0], &board[0][0]+ROWS*COLS, &boardCopy[0][0]);
            int row = makeMove(boardCopy, moves[i], false);
            score = minimax(boardCopy, depth, !isMaxPlayer, row, moves[i]);
            bestScore = max(bestScore, score);
        }
        return bestScore;
    } else {
        int bestScore = INT_MAX;
        int score;
        #pragma omp parallel for private(score) shared(board, bestScore, moves, depth) reduction(min:score)
        for(int i = 0; i < moves.size(); i++) {
            char boardCopy[ROWS][COLS];
            copy(&board[0][0], &board[0][0]+ROWS*COLS, &boardCopy[0][0]);
            int row = makeMove(boardCopy, moves[i], true);
            score = minimax(boardCopy, depth, !isMaxPlayer, row, moves[i]);
            bestScore = min(bestScore, score);
        }
        return bestScore;
    }
}
int computerMove(char board[ROWS][COLS]) {
    int bestMove = 0;
    int bestScore = INT_MIN;
    for(int move : getPossibleMoves(board)) {
        int row = makeMove(board, move, false);
        int score = minimax(board, 0, false, row, move);
        board[row][move] = '0';
        if(score > bestScore) {
            bestMove = move;
            bestScore = score;
        }
    }
    int row = makeMove(board, bestMove, false);
    return checkForWinner(board, false, row, bestMove);
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
    bool isInvalid;
    while(!isWinner) {
       drawBoard(board);
       do {
           cout << "Wybierz numer kolumny" << endl;
           cin >> col;
           isInvalid = isInvalidMove(board, col - 1);
           if(isInvalid) {
                cout << "Błędna kolumna!!" << endl;
           }
       } while(isInvalid);
       row = makeMove(board, col - 1, true);
       if(getPossibleMoves(board).size() == 0) {
           cout << "REMIS" << endl;
           break;
       }
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
       if(getPossibleMoves(board).size() == 0) {
           cout << "REMIS" << endl;
           break;
       }
    }
    return 0;
}

