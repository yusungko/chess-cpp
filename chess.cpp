#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdio>

using namespace std;

// 棋子基類 (Piece)，所有棋子都會繼承這個類別
class Piece {
public:
    string color;  // 棋子的顏色
    Piece(string color) : color(color) {}
    virtual bool isValidMove(pair<int, int> start, pair<int, int> end, vector<vector<Piece*> >& board) = 0;  // 判斷移動是否合法 (虛擬函數)
    virtual char getSymbol() = 0;  // 獲取棋子的符號 (虛擬函數)
    virtual ~Piece() {}  // 虛擬析構函數
};

// 卒子 (Pawn) 類別，繼承自 Piece
class Pawn : public Piece {
public:
    Pawn(string color) : Piece(color) {}
    bool isValidMove(pair<int, int> start, pair<int, int> end, vector<vector<Piece*> >& board) {
        int direction = (color == "white") ? -1 : 1;  // 根據顏色決定方向，白棋往上移，黑棋往下移
        int startRow = start.first;
        int startCol = start.second;
        int endRow = end.first;
        int endCol = end.second;

        // 向前移動
        if (startCol == endCol) {
            if (endRow == startRow + direction && board[endRow][endCol] == NULL) {
                return true;
            }
            // 初次移動可以雙步前進
            if ((startRow == 1 && color == "black") || (startRow == 6 && color == "white")) {
                if (endRow == startRow + 2 * direction && board[endRow][endCol] == NULL && board[startRow + direction][endCol] == NULL) {
                    return true;
                }
            }
        } else if (abs(startCol - endCol) == 1 && endRow == startRow + direction) { // 斜向吃子
            if (board[endRow][endCol] != NULL && board[endRow][endCol]->color != color) {
                return true;
            }
        }

        return false;
    }
    char getSymbol()  { return 'P'; }  // 卒子的符號
};

// 車 (Rook) 類別，繼承自 Piece
class Rook : public Piece {
public:
    Rook(string color) : Piece(color) {}
    bool isValidMove(pair<int, int> start, pair<int, int> end, vector<vector<Piece*> >& board) {
        int startRow = start.first;
        int startCol = start.second;
        int endRow = end.first;
        int endCol = end.second;

        // 車只能直線移動
        if (startRow != endRow && startCol != endCol) {
            return false;
        }

        // 檢查路徑是否通暢
        int rowDirection = (endRow - startRow != 0) ? (endRow - startRow) / abs(endRow - startRow) : 0;
        int colDirection = (endCol - startCol != 0) ? (endCol - startCol) / abs(endCol - startCol) : 0;

        int currentRow = startRow + rowDirection;
        int currentCol = startCol + colDirection;
        while (currentRow != endRow || currentCol != endCol) {
            if (board[currentRow][currentCol] != NULL) {
                return false;
            }
            currentRow += rowDirection;
            currentCol += colDirection;
        }

        if (board[endRow][endCol] == NULL || board[endRow][endCol]->color != color) {
            return true;
        }

        return false;
    }
    char getSymbol()  { return 'R'; }  // 車的符號
};

// 馬 (Knight) 類別，繼承自 Piece
class Knight : public Piece {
public:
    Knight(string color) : Piece(color) {}
    bool isValidMove(pair<int, int> start, pair<int, int> end, vector<vector<Piece*> >& board) {
        int rowDiff = abs(start.first - end.first);
        int colDiff = abs(start.second - end.second);

        // 馬走 "日" 字形
        if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)) {
            if (board[end.first][end.second] == NULL || board[end.first][end.second]->color != color) {
                return true;
            }
        }
        return false;
    }
    char getSymbol()  { return 'N'; }  // 馬的符號
};

// 象 (Bishop) 類別，繼承自 Piece
class Bishop : public Piece {
public:
    Bishop(string color) : Piece(color) {}
    bool isValidMove(pair<int, int> start, pair<int, int> end, vector<vector<Piece*> >& board) {
        int rowDiff = abs(start.first - end.first);
        int colDiff = abs(start.second - end.second);

        // 象只能斜線移動
        if (rowDiff != colDiff) {
            return false;
        }

        // 檢查路徑是否通暢
        int rowDirection = (end.first - start.first) / rowDiff;
        int colDirection = (end.second - start.second) / colDiff;

        int currentRow = start.first + rowDirection;
        int currentCol = start.second + colDirection;
        while (currentRow != end.first || currentCol != end.second) {
            if (board[currentRow][currentCol] != NULL) {
                return false;
            }
            currentRow += rowDirection;
            currentCol += colDirection;
        }

        if (board[end.first][end.second] == NULL || board[end.first][end.second]->color != color) {
            return true;
        }

        return false;
    }
    char getSymbol()  { return 'B'; }  // 象的符號
};

// 皇后 (Queen) 類別，繼承自 Piece
class Queen : public Piece {
public:
    Queen(string color) : Piece(color) {}
    bool isValidMove(pair<int, int> start, pair<int, int> end, vector<vector<Piece*> >& board) {
        Rook rook(color);
        Bishop bishop(color);
        return rook.isValidMove(start, end, board) || bishop.isValidMove(start, end, board);  // 皇后的移動方式為車和象的結合
    }
    char getSymbol()  { return 'Q'; }  // 皇后的符號
};

// 國王 (King) 類別，繼承自 Piece
class King : public Piece {
public:
    King(string color) : Piece(color) {}
    bool isValidMove(pair<int, int> start, pair<int, int> end, vector<vector<Piece*> >& board) {
        int rowDiff = abs(start.first - end.first);
        int colDiff = abs(start.second - end.second);

        // 國王可以朝任意方向走一格
        if (rowDiff <= 1 && colDiff <= 1) {
            if (board[end.first][end.second] == NULL || board[end.first][end.second]->color != color) {
                return true;
            }
        }
        return false;
    }
    char getSymbol()  { return 'K'; }  // 國王的符號
};

// 棋盤 (ChessBoard) 類別
class ChessBoard {
public:
    vector<vector<Piece*> > board;

    ChessBoard() {
        board = vector<vector<Piece*> >(8, vector<Piece*>(8, NULL));  // 初始化 8x8 的棋盤
        initializePieces();  // 初始化棋子
    }

    ~ChessBoard() {
        // 釋放棋子記憶體
        for (size_t i = 0; i < board.size(); ++i) {
            for (size_t j = 0; j < board[i].size(); ++j) {
                delete board[i][j];
            }
        }
    }

    void initializePieces() {
        // 初始化卒子
        for (int i = 0; i < 8; ++i) {
            board[1][i] = new Pawn("black");
            board[6][i] = new Pawn("white");
        }

        // 初始化其他棋子
        board[0][0] = new Rook("black");
        board[0][1] = new Knight("black");
        board[0][2] = new Bishop("black");
        board[0][3] = new Queen("black");
        board[0][4] = new King("black");
        board[0][5] = new Bishop("black");
        board[0][6] = new Knight("black");
        board[0][7] = new Rook("black");

        board[7][0] = new Rook("white");
        board[7][1] = new Knight("white");
        board[7][2] = new Bishop("white");
        board[7][3] = new Queen("white");
        board[7][4] = new King("white");
        board[7][5] = new Bishop("white");
        board[7][6] = new Knight("white");
        board[7][7] = new Rook("white");
    }

    void printBoard() {
        cout << "  A B C D E F G H" << endl;  // 印出棋盤的列標識
        for (size_t i = 0; i < board.size(); ++i) {
            cout << i << " ";  // 印出棋盤的行標識
            for (size_t j = 0; j < board[i].size(); ++j) {
                if (board[i][j])
                    cout << board[i][j]->getSymbol() << " ";  // 印出棋子符號
                else
                    cout << ". ";  // 空格顯示為 '.'
            }
            cout << endl;
        }
    }

    bool movePiece(pair<int, int> start, pair<int, int> end) {
        int x1 = start.first, y1 = start.second;
        int x2 = end.first, y2 = end.second;

        Piece* piece = board[x1][y1];
        if (!piece) {
            cout << "起始位置沒有棋子" << endl;
            return false;
        }

        if (!piece->isValidMove(start, end, board)) {
            cout << "棋子移動無效" << endl;
            return false;
        }

        if (board[x2][y2] && board[x2][y2]->getSymbol() == 'K') {
            cout << piece->color << " 獲勝！" << endl;
            exit(0);
        }

        if (board[x2][y2]) {
            cout << endl << piece->color << " 的 " << piece->getSymbol() << " 吃掉了 " << board[x2][y2]->color << " 的 " << board[x2][y2]->getSymbol() << "。" << endl << endl;
            delete board[x2][y2];
        }
        board[x2][y2] = piece;
        board[x1][y1] = NULL;
        return true;
    }
};

// 將用戶輸入解析為坐標
pair<pair<int, int>, pair<int, int> > parseInput(string userInput) {
    char y1_char, y2_char;
    int x1, x2;
    if (sscanf(userInput.c_str(), "%c %d %c %d", &y1_char, &x1, &y2_char, &x2) == 4) {
        // 將小寫轉成大寫或直接處理小寫
        if (y1_char >= 'a' && y1_char <= 'h') y1_char = y1_char - 'a' + 'A';
        if (y2_char >= 'a' && y2_char <= 'h') y2_char = y2_char - 'a' + 'A';

        int y1 = y1_char - 'A';
        int y2 = y2_char - 'A';

        // 檢查座標是否在合法範圍內
        if (x1 >= 0 && x1 <= 7 && x2 >= 0 && x2 <= 7 && y1 >= 0 && y1 <= 7 && y2 >= 0 && y2 <= 7) {
            return make_pair(make_pair(x1, y1), make_pair(x2, y2));
        }
    }
    cout << endl << "輸入格式錯誤. 使用格式: y1 x1 y2 x2，例如 A 1 A 2 或 d 6 d 4" << endl << endl;
    return make_pair(make_pair(-1, -1), make_pair(-1, -1));
}


int main() {
    system("chcp 65001"); // 設定編碼為 UTF-8
    ChessBoard chessBoard;
    chessBoard.printBoard();

    string userInput;
    string currentTurn = "white";
    while (true) {
    	cout << "--------------------" << endl;
        cout << "目前是 " << currentTurn << " 的回合。" << endl;  // 顯示目前是誰的回合
        cout << endl << "輸入要移動的旗子與座標 (y1 x1 y2 x2，例如 A 1 A 2) 或輸入 'Q' 來退出: " ;
        getline(cin, userInput);
        if (userInput == "Q") {
            break;
        }
        cout << endl; 

        pair<pair<int, int>, pair<int, int> > positions = parseInput(userInput);
        pair<int, int> start = positions.first;
        pair<int, int> end = positions.second;
        
        if (start.first != -1 && end.first != -1) {
            Piece* piece = chessBoard.board[start.first][start.second];
            if (piece && piece->color != currentTurn) {
                cout << "換邊！目前是 " << currentTurn << " 的回合。" << endl << endl;
                continue;
            }
            if (chessBoard.movePiece(start, end)) {
                chessBoard.printBoard();
                currentTurn = (currentTurn == "white") ? "black" : "white";  // 換回合
            }
        }
    }
    return 0;
}

