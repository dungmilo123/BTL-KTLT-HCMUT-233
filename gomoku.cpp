#include <iostream>
#include <iomanip>
#include<string>
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

bool makeMove(Stone board[][MAX_SIZE], int size, string playerMove, bool isFirstPlayerTurn) {
    // Check for leading or trailing whitespace
    if (playerMove.find_first_not_of(" \t\n\r\f\v") != 0 ||
        playerMove.find_last_not_of(" \t\n\r\f\v") != playerMove.length() - 1) {
        return false;
        }

    // Check if the move is empty
    if (playerMove.empty()) return false;

    // Check if the last character is lowercase alphabetic between 'a' and 'o'
    if (playerMove.back() < 'a' || playerMove.back() > 'o') return false;

    // Check for any uppercase letters in the entire string
    for (char c : playerMove) {
        if (c >= 'A' && c <= 'Z') return false;
    }

    // Extract the column (last character)
    char colChar = playerMove.back();
    int col = colChar - 'a';

    // Remove the column character from the string
    playerMove.pop_back();

    // Check if the remaining string is empty
    if (playerMove.empty()) return false;

    // Check for invalid characters (only digits allowed)
    for (char c : playerMove) {
        if (c < '0' || c > '9') return false;
    }

    // Check for leading zeros
    if (playerMove[0] == '0' && playerMove.length() > 1) return false;

    // Try to convert the remaining string to an integer
    int row;
    try {
        row = stoi(playerMove);
    } catch (const std::exception&) {
        return false;  // Conversion failed or out of range
    }

    // Check if row is between 1 and 15
    if (row < 1 || row > size) return false;

    // Adjust row number
    row = size - row;

    // Check if the cell is already occupied
    if (board[row][col] != NA) return false;

    // Make the move
    board[row][col] = isFirstPlayerTurn ? X : O;
    return true;
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

        return blockedBefore && blockedAfter;
    };

    // Helper function to check if there's exactly 5 in a row and not part of a longer sequence
    auto checkLine = [&](int startX, int startY, int dx, int dy) -> bool {
        for (int i = 0; i < 5; ++i) {
            int x = startX + i * dx;
            int y = startY + i * dy;
            if (x < 0 || x >= size || y < 0 || y >= size || board[x][y] != currentStone) {
                return false;
            }
        }

        // Check if it's part of a longer sequence
        int beforeX = startX - dx;
        int beforeY = startY - dy;
        int afterX = startX + 5 * dx;
        int afterY = startY + 5 * dy;

        bool extendsBefore = (beforeX >= 0 && beforeX < size && beforeY >= 0 && beforeY < size && board[beforeX][beforeY] == currentStone);
        bool extendsAfter = (afterX >= 0 && afterX < size && afterY >= 0 && afterY < size && board[afterX][afterY] == currentStone);

        return !extendsBefore && !extendsAfter && !isBlockedAtBothEnds(startX, startY, dx, dy);
    };

    // Check horizontal lines
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j <= size - 5; ++j) {
            if (checkLine(i, j, 0, 1)) return true;
        }
    }

    // Check vertical lines
    for (int i = 0; i <= size - 5; ++i) {
        for (int j = 0; j < size; ++j) {
            if (checkLine(i, j, 1, 0)) return true;
        }
    }

    // Check diagonal from top-left to bottom-right
    for (int i = 0; i <= size - 5; ++i) {
        for (int j = 0; j <= size - 5; ++j) {
            if (checkLine(i, j, 1, 1)) return true;
        }
    }

    // Check diagonal from bottom-left to top-right
    for (int i = 4; i < size; ++i) {
        for (int j = 0; j <= size - 5; ++j) {
            if (checkLine(i, j, -1, 1)) return true;
        }
    }

    return false;
}




void displayHistory(string history, int numOfMoves) {
    bool firstMoveMade = false;
    // Step 1: Initialize the game board with all cells set to NA
    Stone game[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            game[i][j] = NA;
        }
    }

    // Step 2: Display the initial empty board
    displayBoardSimple(game, MAX_SIZE);

    int currentMoveIndex = -1; // Start with no moves made
    string moves[225]; // Max possible moves in a 15x15 board
    int moveCount = 0;
    size_t pos = 0;

    // Parse the history string into individual moves
    while (pos < history.length() && moveCount < 225) {
        size_t len = 1;
        while (pos + len < history.length() && history[pos + len] >= '0' && history[pos + len] <= '9') {
            len++;
        }
        len++; // Include the move character (a-z)
        moves[moveCount++] = history.substr(pos, len);
        pos += len;
    }

    // Print the initial command prompt
    cout << inputCommand;

    // Step 3: Process user input
    char userInput;
    while (cin >> userInput) {
        if (userInput == 'n') { // Next move
            if(currentMoveIndex == numOfMoves - 1) {
                cout << endOfHistory;
                continue;
            }
            if (currentMoveIndex < numOfMoves - 1) {
                currentMoveIndex++;
                string move = moves[currentMoveIndex];
                makeMove(game, MAX_SIZE, move, currentMoveIndex % 2 == 0);
                displayBoardSimple(game, MAX_SIZE);
                cout << inputCommand;
            }
        } else if (userInput == 'p') { // Previous move
            if(currentMoveIndex == -1) {
                cout << startOfGame;
                continue;
            }
            if (currentMoveIndex >= 0) {
                string move = moves[currentMoveIndex];
                int row = MAX_SIZE - stoi(move.substr(0, move.length() - 1));
                char colChar = move.back();
                int col = colChar - 'a';
                game[row][col] = NA; // Remove the move
                currentMoveIndex--;
                displayBoardSimple(game, MAX_SIZE);
                cout << inputCommand;
            }
        } else if (userInput == 's') { // Stop
            break; // Exit the loop
        } else {
            cout << invalidInput;
        }
    }
}







void startGame() {
    Stone game[15][15];
    for(int i = 0; i < MAX_SIZE; i++) {
        for(int j = 0; j < MAX_SIZE; j++) {
            game[i][j] = NA;
        }
    }
    displayBoardSimple(game, MAX_SIZE);
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
                displayBoardSimple(game, MAX_SIZE);
                cout << ((player1Turn) ? "Player 1 won " : "Player 2 won ");
                return;
            }
            player1Turn = !player1Turn;
            displayBoardSimple(game, MAX_SIZE);
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