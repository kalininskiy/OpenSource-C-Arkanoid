//
// 2015 (c) Copyright Ivan "VDM" Kalininskiy
//
// Класс объекта шарик
//
class Ball : public RectangleElement {
private:
    // Размер шарика
    int ballXsize;
    int ballYsize;

    // Спрайт
    CImg<unsigned char> gameBall;

    // Нормальная скорость шарика
    float SpeedVertical = 2.0f;
    float SpeedHorisontal = 2.0f;

    // Текущие координаты шарика
    float X = 0,
          Y = 0;

    // Предыдущие координаты шарика
    float Xo = 0,
          Yo = 0;

    // Вектор движения шарика
    float xVector = 0,
          yVector = 0;

    static const int RGB = 3;

public:
    // Конструктор класса
    Ball() : RectangleElement() {
        // Инициализируем шарик
        ballXsize = sizeY / 2;
        ballYsize = ballXsize;

        unsigned char white[RGB] = { 255, 255, 255 }; // Белый

        gameBall = CImg<unsigned char> (ballXsize, ballYsize, 1, RGB, 0);
        gameBall.draw_circle(ballXsize / 2, ballYsize / 2, ballXsize / 2 - 1, white);
    }

    // Двигаем шарик по вектору движения
    //
    void move() {
        // Запоминаем начальные координаты шарика
        Xo = X;
        Yo = Y;

        // Двигаем шарик по вектору направления
        X += xVector;
        Y += yVector;
    }

    // Инициализация шарика для начала игры
    //
    template <typename T1, typename T2>
    void initialize(T1 xO, T2 yO) {
        // Шарик изначально появится в центре игрового поля
        Xo = xO;
        Yo = yO;
        X = Xo;
        Y = Yo;

        // Начальная скорость шарика
        xVector = SpeedVertical;
        yVector = SpeedHorisontal;
    }

    // Проверка уронили шарик или нет
    //
    template <class T>
    bool fallDown(T &gamePad) { return (Y > gamePad.height()); }

    // Функция проверки на столкновение шарика с краями игрового поля, с ракеткой-доской и с кирпичами
    //
    template <class T1, class T2, class T3, typename T4>
    unsigned int testCollisions(T1 &gamePad, T2 &brick, T3 board, T4 playerScore)
    {
        // Проверяем столкновение шарика с правым и левым краем игрового поля
        if ((X >= gamePad.width() - widthHalf()) ||
            (X < widthHalf()))
        {
            X -= xVector;
            Y -= yVector;
            xVector = -xVector;
        }

        // Проверяем столкновение шарика с верхним краем игрового поля
        if (Y < heightHalf()) {
            Y = (float) heightHalf();
            yVector = -yVector;
        }

        // Проверяем столкновение шарика с ракеткой-доской
        if (Y >= gamePad.height() - board.height() - ballXsize - heightHalf() &&
            Y < gamePad.height() - ballYsize - heightHalf() &&
            board.X() - board.widthHalf() <= X &&
            board.X() + board.widthHalf() >= X)
        {
            X = Xo;
            Y = gamePad.height() - board.height() - (float) ballXsize - heightHalf();

            // Ускоряем или замедляем шарик в зависимости от движения ракетки-доски
            yVector = -yVector;
            xVector += (board.X() - board.Xo()) / heightHalf();
            if (cimg::abs(xVector) > ballXsize)
                xVector *= ballXsize / cimg::abs(xVector);
        }

        // Если шарик на верхней половине игрового поля
        if (Y < gamePad.dimY() * sizeY) {
            int X = (int) getX() / sizeX,
                Y = (int) getY() / sizeY;

            // Проверяем столкновение шарика с кирпичом
            if (gamePad.getGamePadValue(X, Y)) {
                // Сохраняем +1 выбитый кирпич
                playerScore++;
                gamePad.killBrick(X, Y, brick);
                // Меняем направление движения шарика
                if (Xo < (X << 5) || Xo >= ((X + 1) << 5))
                    // По оси X
                    xVector = -xVector;
                else if (Yo < (Y << 4) || Yo >= ((Y + 1) << 4))
                    // По оси Y
                    yVector = -yVector;
            }
        }
        return playerScore;
    }

    CImg<unsigned char> getImage() { return gameBall; }

    float getX() { return X; }
    float getY() { return Y; }

    int width() { return gameBall.dimx(); }
    int height() { return gameBall.dimy(); }

    int widthHalf() { return width() / 2; }
    int heightHalf() { return height() / 2; }
};