//
// 2015 (c) Copyright Ivan "VDM" Kalininskiy
//
// Класс объекта ракетки-доски
//
class Board : public RectangleElement {
private:
    // Размеры ракетки-доски, зависит от элемента игрового поля
    int boardXsize;
    int boardYsize;

    int mouseMove;

    CImg<unsigned char> gameBoard;

    int boardWidth,
        boardHeight,
        boardWidthHalf;

    float boardX,
          boardXo;

    static const int RGB = 3;

public:
    // Конструктор класса
    Board(int width) : RectangleElement() {

        boardXsize = sizeX * 2;
        boardYsize = sizeY / 2;

        mouseMove = sizeX * 2 + sizeY;

        // Задаем ракетку-доску
        gameBoard = CImg<unsigned char> (boardXsize, boardYsize, 1, RGB, 235);

        boardWidth = gameBoard.dimx();
        boardHeight = gameBoard.dimy();
        boardWidthHalf = boardWidth / 2;

        boardX = (float) (width - boardWidthHalf);
        boardXo = (float) boardX;
    }

    // Двигаем ракетку-доску
    //
    void move(int x) {
        boardXo = boardX;
        boardX = (float) x;
    }

    // Функция возвращает местонахождения ракетки-доски зависимости от координаты мыши
    //
    template <class T1, class T2>
    int getMouseX(T1 &imgDisplay, T2 gamePad)
    {
        int X = (int) boardX;

        // Соотносим координаты
        if (imgDisplay.mouse_x >= 0)
            X = (int) (gamePad.widthHalf() + ((imgDisplay.mouse_x < 0 ? gamePad.widthHalf() : imgDisplay.mouse_x) - gamePad.widthHalf()) * 2);
        else
            imgDisplay.set_mouse(boardX > gamePad.widthHalf() ? gamePad.width() - mouseMove + 1 : mouseMove, gamePad.heightHalf());

        // Проверяем правый край
        if (X < boardWidthHalf) {
            X = boardWidthHalf;
            imgDisplay.set_mouse(mouseMove, gamePad.heightHalf());
        }

        // Проверяем левый край
        if (X >= gamePad.width() - boardWidthHalf) {
            X = gamePad.width() - boardWidthHalf - 1;
            imgDisplay.set_mouse(gamePad.width() - mouseMove + 1, gamePad.heightHalf());
        }
        return X;
    }

    CImg<unsigned char> getGameBoard() { return gameBoard; };

    int height() { return boardHeight; }
    int widthHalf() { return boardWidthHalf; }

    float X() { return boardX; }
    float Xo() { return boardXo; }
};