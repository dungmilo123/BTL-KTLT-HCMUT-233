#include <iostream>
#include <cstdlib>
#include <iomanip>
#include<vector>
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
    Stone currentStone = isFirstPlayerTurn ? X : O;
    Stone opponentStone = isFirstPlayerTurn ? O : X;

    // Helper function to check if the line is blocked at both ends
    auto isBlockedAtBothEnds = [&](int startX, int startY, int dx, int dy) -> bool {
        int beforeX = startX - dx;
        int beforeY = startY - dy;
        int afterX = startX + 5 * dx;
        int afterY = startY + 5 * dy;

        bool blockedBefore = (beforeX >= 0 && beforeX < size && beforeY >= 0 && beforeY < size && board[beforeX][beforeY] == opponentStone);
        bool blockedAfter = (afterX >= 0 && afterX < size && afterY >= 0 && afterY < size && board[afterX][afterY] == opponentStone);

        return blockedBefore && blockedAfter; // Return true if blocked at both ends
    };

    // Helper function to check if there's exactly 5 in a row and not part of a longer sequence
    auto checkLine = [&](int startX, int startY, int dx, int dy) -> bool {
        for (int i = 0; i <= size - 5; ++i) {
            int x = startX + i * dx;
            int y = startY + i * dy;

            // Check if there's exactly 5 in a row
            int count = 0;
            while (count < 5 && x + count * dx < size && x + count * dx >= 0 &&
                   y + count * dy < size && y + count * dy >= 0 &&
                   board[x + count * dx][y + count * dy] == currentStone) {
                count++;
            }

            // If we have exactly 5 in a row
            if (count == 5) {
                // Check if it's part of a longer sequence
                bool isExactFive = true;
                if ((x + 5 * dx < size && x + 5 * dx >= 0 &&
                     y + 5 * dy < size && y + 5 * dy >= 0 &&
                     board[x + 5 * dx][y + 5 * dy] == currentStone) ||
                    (x - dx >= 0 && x - dx < size &&
                     y - dy >= 0 && y - dy < size &&
                     board[x - dx][y - dy] == currentStone)) {
                    isExactFive = false; // There are more than 5 stones in a row
                }

                // Check for exact 5 stones and if blocked at both ends
                if (isExactFive && !isBlockedAtBothEnds(x, y, dx, dy)) {
                    return true; // Exactly 5 stones and not blocked at both ends
                }
            }
        }
        return false;
    };

    // Check horizontal lines
    for (int i = 0; i < size; ++i) {
        if (checkLine(i, 0, 0, 1)) {
            return true;
        }
    }

    // Check vertical lines
    for (int j = 0; j < size; ++j) {
        if (checkLine(0, j, 1, 0)) {
            return true;
        }
    }

    // Check diagonal from top-left to bottom-right
    for (int i = 0; i < size; ++i) {
        if (checkLine(i, 0, 1, 1)) {
            return true;
        }
        if (checkLine(0, i, 1, 1)) {
            return true;
        }
    }

    // Check diagonal from bottom-left to top-right
    for (int i = 0; i < size; ++i) {
        if (checkLine(i, 0, 1, -1)) {
            return true;
        }
        if (checkLine(size - 1, i, 1, -1)) {
            return true;
        }
    }

    // Check diagonal from top-right to bottom-left
    for (int i = 0; i < size; ++i) {
        if (checkLine(i, size - 1, 1, -1)) {
            return true;
        }
        if (checkLine(0, size - 1 - i, 1, -1)) {
            return true;
        }
    }

    // Check diagonal from bottom-right to top-left
    for (int i = 0; i < size; ++i) {
        if (checkLine(size - 1, i, -1, -1)) {
            return true;
        }
        if (checkLine(i, size - 1, -1, -1)) {
            return true;
        }
    }

    return false;
}




void displayHistory(string history, int numOfMoves) {
    //BEGIN
    Stone game[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            game[i][j] = NA;
        }
    }
    displayBoard(game, MAX_SIZE);
    int currentMoveIndex = -1;
    string moves[225];
    int moveCount = 0;
    size_t pos = 0;
    while (pos < history.length() && moveCount < numOfMoves) {
        size_t len = 1;
        while (isdigit(history[pos + len])) {
            len++;
        }
        len++;
        moves[moveCount++] = history.substr(pos, len);
        pos += len;
    }
    cout << inputCommand;

    char userInput;
    while (cin >> userInput) {
        if (userInput == 'n') {
            if (currentMoveIndex < numOfMoves - 1) {
                currentMoveIndex++;
                string move = moves[currentMoveIndex];
                makeMove(game, MAX_SIZE, move, currentMoveIndex % 2 == 0);
                displayBoard(game, MAX_SIZE);
            } else {
                cout << endOfHistory;
            }
        } else if (userInput == 'p') {
            if (currentMoveIndex >= 0) {
                string move = moves[currentMoveIndex];
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
    //ENDTODO
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