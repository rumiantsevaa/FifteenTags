#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <queue>
#include <set>
#include <ctime> 
using namespace std;

struct Node {
    vector<vector<int>> board;
    int emptyRow;
    int emptyCol;
    int g;
    int h;
};

bool operator<(const Node& a, const Node& b) {
    return a.g + a.h < b.g + b.h;
}

int calculateManhattanDistance(int value, int targetRow, int targetCol, int boardSize) {
    int row = value / boardSize;
    int col = value % boardSize;
    return abs(row - targetRow) + abs(col - targetCol);
}

void shuffleBoard(vector<vector<int>>& board, int& emptyRow, int& emptyCol) {
    int boardSize = board.size();
    vector<int> numbers(boardSize * boardSize - 1);
    for (int i = 0; i < boardSize * boardSize - 1; ++i) {
        numbers[i] = i + 1;
    }
    // Начальное размешивание пятнашек
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(numbers.begin(), numbers.end(), default_random_engine(seed));

    int index = 0;
    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            if (row != emptyRow || col != emptyCol) {
                board[row][col] = numbers[index++];
            }
        }
    }
}

void printBoard(const vector<vector<int>>& board) {
    for (const auto& row : board) {
        for (int number : row) {
            cout << number << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void makeComputerMove(vector<vector<int>>& board, int& emptyRow, int& emptyCol) {
    //Выбор режима игры 3*3 4*4
    int boardSize = board.size();

    vector<vector<int>> targetBoard(boardSize, vector<int>(boardSize));
    int targetRow = 0;
    int targetCol = 0;

    for (int value = 1; value < boardSize * boardSize; ++value) {
        targetBoard[targetRow][targetCol] = value;
        ++targetCol;
        if (targetCol == boardSize) {
            ++targetRow;
            targetCol = 0;
        }
    }

    targetBoard[targetRow][targetCol] = 0;

    priority_queue<Node> openList;
    set<vector<vector<int>>> visited;

    Node startNode;
    startNode.board = board;
    startNode.emptyRow = emptyRow;
    startNode.emptyCol = emptyCol;
    startNode.g = 0;
    startNode.h = 0;

    openList.push(startNode);

    while (!openList.empty()) {
        Node currentNode = openList.top();
        openList.pop();

        if (currentNode.board == targetBoard) {
            board = currentNode.board;
            emptyRow = currentNode.emptyRow;
            emptyCol = currentNode.emptyCol;
            return;
        }

        visited.insert(currentNode.board);

        int row = currentNode.emptyRow;
        int col = currentNode.emptyCol;

        // Generate possible moves
        vector<pair<int, int>> possibleMoves;
        if (row - 1 >= 0) {
            possibleMoves.push_back({ row - 1, col });
        }
        if (row + 1 < boardSize) {
            possibleMoves.push_back({ row + 1, col });
        }
        if (col - 1 >= 0) {
            possibleMoves.push_back({ row, col - 1 });
        }
        if (col + 1 < boardSize) {
            possibleMoves.push_back({ row, col + 1 });
        }

        for (const auto& move : possibleMoves) {
            int newRow = move.first;
            int newCol = move.second;

            Node newNode;
            newNode.board = currentNode.board;
            newNode.emptyRow = newRow;
            newNode.emptyCol = newCol;
            newNode.g = currentNode.g + 1;
            newNode.h = calculateManhattanDistance(currentNode.board[newRow][newCol], newRow, newCol, boardSize);

            swap(newNode.board[row][col], newNode.board[newRow][newCol]);

            if (visited.count(newNode.board) == 0) {
                openList.push(newNode);
            }
        }
    }
}

int main() {
    int boardSize;
    cout << "Choose board size (Enter size 3 or 4): ";
    cin >> boardSize;

    vector<vector<int>> board(boardSize, vector<int>(boardSize));
    int emptyRow = 0;
    int emptyCol = 0;

    // Начальное размешивание пятнашек (Компьютерное размешивание)
    int shuffleOption;
    cout << "Would you like to shuffle the order of tags randomly? (Enter 1 - Yes, 0 - No): ";
    cin >> shuffleOption;
    if (shuffleOption == 1) {
        shuffleBoard(board, emptyRow, emptyCol);
    }
    else {
        // Заполнение последовательно 
        int index = 1;
        for (int row = 0; row < boardSize; ++row) {
            for (int col = 0; col < boardSize; ++col) {
                board[row][col] = index++;
            }
        }
        emptyRow = boardSize - 1;
        emptyCol = boardSize - 1;
        board[emptyRow][emptyCol] = 0;
    }

    printBoard(board);

    //Режимы игры
    int mode;
    cout << "Select game mode:\n";
    cout << "1. Human\n";
    cout << "2. Computer\n";
    cout << "Enter mode number ";
    cin >> mode;

    if (mode == 1) {
        // Человек собирает
        cout << "The game has begun. To move a tag, enter its value: ";
        int value;
        int moves = 0;
        auto startTime = chrono::steady_clock::now();
        while (cin >> value) {
            if (value < 0 || value >= boardSize * boardSize) {
                cout << "Incorrect value. Try again: ";
                continue;
            }

            int row = emptyRow;
            int col = emptyCol;

            // Поиск пятнашки
            for (int i = 0; i < boardSize; ++i) {
                for (int j = 0; j < boardSize; ++j) {
                    if (board[i][j] == value) {
                        row = i;
                        col = j;
                        break;
                    }
                }
            }

            // Проверка возможности перемещения
            if (abs(row - emptyRow) + abs(col - emptyCol) != 1) {
                cout << "Moving is not possible. Try again: ";
                continue;
            }

            // Перемещение пятнашки
            swap(board[row][col], board[emptyRow][emptyCol]);
            emptyRow = row;
            emptyCol = col;

            ++moves;
            printBoard(board);

            // Проверка завершения игры
            if (board == vector<vector<int>>(boardSize, vector<int>(boardSize))) {
                auto endTime = chrono::steady_clock::now();
                auto duration = chrono::duration_cast<chrono::seconds>(endTime - startTime).count();
                cout << "Congratulations! You collected tags for " << moves << " moves.\n";
                cout << "Game time: " << duration << " secs.\n";
                break;
            }

            cout << "Enter the value of the following tag: ";
        }
    }
    else if (mode == 2) {
        // Компьютер собирает
        cout << "Computer moves:\n";
        int moves = 0;
        auto startTime = chrono::steady_clock::now();
        while (true) {
            makeComputerMove(board, emptyRow, emptyCol);
            ++moves;
            printBoard(board);

            // Проверка завершения игры
            if (board == vector<vector<int>>(boardSize, vector<int>(boardSize))) {
                auto endTime = chrono::steady_clock::now();
                auto duration = chrono::duration_cast<chrono::seconds>(endTime - startTime).count();
                cout << "Computer collected tags for " << moves << " moves.\n";
                cout << "Game time: " << duration << " secs.\n";
                break;
            }
        }
    }

    return 0;
}

