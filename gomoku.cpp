#include <iostream>
#include <cstdlib>
#include <iomanip>
using namespace std;

#define MAX_SIZE 15

const char HORZ = 196;
const char VERT = 179;
const char TL   = 218;
const char TM   = 194;
const char TR   = 191;
const char BL   = 192;
const char BM   = 193;
const char BR   = 217;
const int numW  = 3;
const int charW = 1;
const int numPerLine = 15;
const char VERT_START = 195;
const char VERT_END = 180;
const char VERT_MID = 197;
const string inputCommand = "Previous move/Next move/Stop [p/n/s]: ";
const string endOfHistory = "This is the end of the match.\nPrevious move/Next move/Stop [p/n/s]: ";
const string startOfGame = "This is the start of the match.\nPrevious move/Next move/Stop [p/n/s]: ";
const string invalidInput = "Illegal input, please try again: ";

enum Stone {NA, X, O};

string lineString( char left, char mid, char right, char horz, int wcell, int ncell )
{
   string result = "", cell = string( wcell, horz );
   result += left;
   for ( int i = 0; i < ncell - 1; i++ ) result += cell + mid;
   result += cell + right;
   return result;
}

void displayBoard(Stone arr[][MAX_SIZE], int size) {
    //UPPERLINE
    cout << "   ";
    for(int i = 0; i < size; i++) {
        cout << "  " << (char)(i + 'a') << " ";
    }
    cout << endl;
    cout << "   " << lineString( TL, TM, TR, HORZ, 3, numPerLine ) << "\n";

    //MIDDLE
    for(int i = 0; i < size; i++) {
        cout << setw(2) << size - i << " " << VERT;
        for(int j = 0; j < size; j++) {
            if(arr[i][j] == NA) cout << "   " << VERT;
            if(arr[i][j] == X) cout << " X " << VERT;
            if(arr[i][j] == O) cout << " O " << VERT;
        }
        cout << endl;
        if(i != size - 1) cout << "   " << lineString( VERT_START, VERT_MID, VERT_END, HORZ, 3, numPerLine ) << "\n";
    }

    //UNDERLINE
    cout << "   " << lineString( BL, BM, BR, HORZ, 3, numPerLine ) << "\n";
}

void displayBoardSimple(Stone arr[][MAX_SIZE], int size) {
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++) {
            cout << arr[i][j];
        }
    }
    cout << endl;
}

bool makeMove(Stone board[][MAX_SIZE], int size, string playerMove, bool isFirstPlayerTurn){
    //BEGIN TODO
    if (playerMove.length() < 2) return false;
    int row = stoi(playerMove.substr(0, playerMove.length() - 1));
    char colChar = playerMove[playerMove.length() - 1];
    int col = colChar - 'a';
    row = size - row;   
    if (row < 0 || row >= size || col < 0 || col >= size) return false;
    if (board[row][col] != NA) return false;
    board[row][col] = isFirstPlayerTurn ? X : O;
    return true;
     //END TODO
}
bool hasWon(Stone board[][MAX_SIZE], int size, bool isFirstPlayerTurn) {
    //BEGIN TODO
    Stone currentStone = isFirstPlayerTurn ? X : O;
    Stone opponentStone = isFirstPlayerTurn ? O : X;
    auto isBlockedAtEnds = [&](int startX, int startY, int dx, int dy) -> bool {
        int beforeX = startX - dx;
        int beforeY = startY - dy;
        int afterX = startX + 5 * dx;
        int afterY = startY + 5 * dy;
        
        bool blockedBefore = (beforeX >= 0 && beforeX < size && beforeY >= 0 && beforeY < size && board[beforeX][beforeY] == opponentStone);
        bool blockedAfter = (afterX >= 0 && afterX < size && afterY >= 0 && afterY < size && board[afterX][afterY] == opponentStone);
        
        return blockedBefore && blockedAfter;
    };
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j <= size - 5; ++j) {
            int count = 0;
            for (int k = 0; k < 5; ++k) {
                if (board[i][j + k] == currentStone) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == 5 && !isBlockedAtEnds(i, j, 0, 1)) {
                return true;
            }
        }
    }
    for (int i = 0; i <= size - 5; ++i) {
        for (int j = 0; j < size; ++j) {
            int count = 0;
            for (int k = 0; k < 5; ++k) {
                if (board[i + k][j] == currentStone) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == 5 && !isBlockedAtEnds(i, j, 1, 0)) {
                return true;
            }
        }
    }
    for (int i = 0; i <= size - 5; ++i) {
        for (int j = 0; j <= size - 5; ++j) {
            int count = 0;
            for (int k = 0; k < 5; ++k) {
                if (board[i + k][j + k] == currentStone) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == 5 && !isBlockedAtEnds(i, j, 1, 1)) {
                return true;
            }
        }
    }
    for (int i = 0; i <= size - 5; ++i) {
        for (int j = 4; j < size; ++j) {
            int count = 0;
            for (int k = 0; k < 5; ++k) {
                if (board[i + k][j - k] == currentStone) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == 5 && !isBlockedAtEnds(i, j, 1, -1)) {
                return true;
            }
        }
    }

    return false;
    //END TODO
}


void displayHistory(string history, int numOfMoves) {
    //BEGIN TODO
    Stone game[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            game[i][j] = NA;
        }
    }
    displayBoard(game, MAX_SIZE);
    int currentMoveIndex = -1;
    cout << inputCommand;

    char userInput;
    while (cin >> userInput) {
        if (userInput == 'n') {
            if (currentMoveIndex < numOfMoves - 1) {
                currentMoveIndex++;
                string move = history.substr(currentMoveIndex * 2, 2);
                makeMove(game, MAX_SIZE, move, currentMoveIndex % 2 == 0);
                displayBoard(game, MAX_SIZE);
            } else {
                cout << endOfHistory;
            }
        } else if (userInput == 'p') {
            if (currentMoveIndex >= 0) {
                string move = history.substr(currentMoveIndex * 2, 2);
                int row = stoi(move.substr(0, move.length() - 1));
                char colChar = move[move.length() - 1];
                int col = colChar - 'a';
                row = MAX_SIZE - row;
                game[row][col] = NA;
                currentMoveIndex--;
                displayBoard(game, MAX_SIZE);
            } else {
                cout << startOfGame;
            }
        } else if (userInput == 's') {
            break;
        } else {
            cout << invalidInput;
        }
        cout << inputCommand;
    }
    //END TODO
}


void startGame() {
    Stone game[15][15];
    for(int i = 0; i < MAX_SIZE; i++) {
        for(int j = 0; j < MAX_SIZE; j++) {
            game[i][j] = NA;
        }
    }
    displayBoard(game, MAX_SIZE);
    string playerMove;
    bool player1Turn = true;
    cout << "Player 1 turn: ";
    while(cin >> playerMove) {
        if(playerMove == "ff") {
            cout << ((player1Turn) ? "Player 2 won " : "Player 1 won ");
            break;
        }
        else if(makeMove(game, MAX_SIZE, playerMove, player1Turn)){
            if(hasWon(game, MAX_SIZE, player1Turn)) {
                displayBoard(game, MAX_SIZE);
                cout << ((player1Turn) ? "Player 1 won " : "Player 2 won ");
                return;
            }
            player1Turn = !player1Turn;
            displayBoard(game, MAX_SIZE);
            cout << ((player1Turn) ? "Player 1 turn: " : "Player 2 turn: ");
        } else {
            cout << "Illegal move, please try again: ";
        }
    }
}

int main()
{
    cout << "Welcome to Gomoku!" << endl;
    cout << "1. Play game" << endl;
    cout << "2. History" << endl;
    cout << "3. Exit" << endl;
    cout << "Please select mode [1/2/3]: ";
    int mode;
    while(cin >> mode){
        if(mode == 1) {
            startGame();
            return 0;
        } else if (mode == 2) {
            int numOfMoves;
            cout << "Please enter number of moves: ";
            cin >> numOfMoves;
            string history;
            cout << "Please enter history: ";
            cin >> history;
            displayHistory(history, numOfMoves);
            return 0;
        } else if (mode == 3) {
            return 0;
        } else {
            cout << "Invalid mode, please try again: ";
        }
    }

    return 0;
}
