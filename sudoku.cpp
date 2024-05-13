//
// Created by keith on 11/9/22.
//

//
// Created by keith on 11/7/22.
//
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Sudoku {
private:

public:
    Sudoku() {
    }

    ~Sudoku() {
    }

    void print(vector<vector<char>>& board){
        int index = 0;
        for(vector<vector<char>>::iterator iter1 = board.begin(); iter1 != board.end();
            iter1++){
            cout << "[" << index++ << "] ";
            for(vector<char>::iterator iter2 = iter1->begin(); iter2 != iter1->end(); iter2++)
                cout << *iter2 << " ";
            cout << endl;
        }
    }

    void solveSudoku(vector<vector<char>>& board) {
        solve(board);
    }

    bool solve(vector<vector<char>>& board) {
        //Tranversing the given sudoku
        for(int i=0;i<board.size();i++) {
            for(int j=0;j<board[0].size();j++) {
                if(board[i][j] == '.') {    //blank found

                    for(char c = '1';c <= '9'; c++) {   //trying all the possible numbers from 1-9
                        if(isValid(board,i,j,c)) {
                            board[i][j] = c;
                            if(solve(board))  //checking for the next blank
                                return true;
                            board[i][j] = '.';  //if blank cannot be filled after checking then revert the changes
                        }
                    }
                    return false;
                }
            }
        }
        return true;    //if everything is filled then return true
    }

    bool isValid(vector<vector<char>>& board , int row , int col , char c) {
        for(int i=0;i<9;i++) {
            if(board[row][i] == c)  //checking row if the value already there
                return false;
            if(board[i][col] == c)  //checking column if value already there
                return false;
            //checking the 3X3 sub box where the blank value is present to check if element is there
            if(board[3*(row/3) + i/3][3*(col/3) + i%3] == c)
                return false;
        }
        return true;
    }
};

// Driver Code
int main()
{
    // 0 means unassigned cells
    vector<vector<char>> board {
        {'5','3','.','.','7','.','.','.','.'},
        {'6','.','.','1','9','5','.','.','.'},
        {'.','9','8','.','.','.','.','6','.'},
        {'8','.','.','.','6','.','.','.','3'},
        {'4','.','.','8','.','3','.','.','1'},
        {'7','.','.','.','2','.','.','.','6'},
        {'.','6','.','.','.','.','2','8','.'},
        {'.','.','.','4','1','9','.','.','5'},
        {'.','.','.','.','8','.','.','7','9'}
    };
    auto sudo = new Sudoku();
    sudo->solveSudoku(board);
    sudo->print(board);
    return 0;
    // This is code is contributed by Pradeep Mondal P
}