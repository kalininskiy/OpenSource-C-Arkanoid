//
// 2015 (c) Copyright Ivan "VDM" Kalininskiy
//
// Класс игрового поля
//
class GamePad : public RectangleElement {
private:
    // Размеры игрового поля в "элементах" игрового поля (прямоугольниках)
    static const int gamePadXsize = 8;
    static const int gamePadYsize = 8;

    // Задаем игровую доску и количество элементов игрового поля (будет хранить кирпичи)
    CImg<unsigned char> gamePad;

    // Игровой задник
    CImg<unsigned char> background;

    // Игровой кадр и буфер
    CImg<unsigned char> frameBuffer,
                        frame;

    // Размеры игрового поля в пикселях
    int widthFull,
        heightFull,

        WidthHalf,
        HeightHalf;

public:
    // Конструктор класса
    GamePad() : RectangleElement() {
        // Задаем игровую доску и количество элементов игрового поля (будет хранить кирпичи)
        gamePad = CImg<unsigned char> (gamePadXsize, gamePadYsize, 1, 1, 0);

        // Игровой задник
        background = CImg<unsigned char> ("resources//BackGround.res");

        // В буфер записываем задник
        frameBuffer = CImg<unsigned char> (background / 2);

        // Игровой кадр, пишем туда буфер
        frame = CImg<unsigned char> (frameBuffer);

        widthFull = frame.dimx();
        heightFull = frame.dimy();

        WidthHalf = widthFull / 2,
        HeightHalf = heightFull / 2;
    }

    // Чистим игровое поле
    //
    void clear() {
        // Обнуляем наличие кирпичей на игровом поле
        gamePad.fill(0);
        // Копируем в буфер игровой задник
        frameBuffer = background;
    }

    // Здесь случайным образом заполняем игровое поле кирпичами с случайными цветами
    //
    template <class T>
    void fillRandomBricks(T brick) {
        // Для этого проходимся циклом по каждому месту для кирпича на игровом поле
        cimg_forXY(gamePad, x, y)
            // Случайным образом решаем быть тут кирпичу или нет
            if (0.2f + cimg::crand() >= 0) {
                // Создаем заполнение кирпича случайным цветом
                CImg<> cbrick = CImg<double>::vector(100 + cimg::rand() * 155,
                                                     100 + cimg::rand() * 155,
                                                     100 + cimg::rand() * 155).unroll('v')
                                                    .resize(brick.getX(), brick.getY());
                // Рисуем для красоты блики на краях кирпича
                cimg_forV(cbrick, k) (cbrick.get_shared_channel(k).mul(brick.getBrick())) /= 255;
                // Рисуем в буфер получившийся кирпич случайного цвета
                frameBuffer.draw_image(cbrick, x * sizeX, y * sizeY);
                // Сохраняем наличие кирпича в этом месте игрового поля
                gamePad(x, y) = 1;
            }
    }

    // Удаляем кирпич по "адресу" X, Y
    //
    template <typename T1, typename T2, class T3>
    void killBrick(T1 X, T2 Y, T3 brick) {
        // Обнуляем "выбиваем" кирпич
        gamePad(X, Y) = 0;
        // Высчитываем по каким координатам стирать кирпич
        int x0 = X * brick.getX(),
            y0 = Y * brick.getY(),
            x1 = (X + 1) * brick.getX() - 1,
            y1 = (Y + 1) * brick.getY() - 1;
        // Затем стираем его копируя в область буфера из задника
        frameBuffer.draw_image(background.get_crop(x0, y0, x1, y1), x0, y0);
    }

    // Отображаем кадр игрового поля
    //
    template <class T1, class T2, class T3>
    void display(T1 &imgDisplay, T2 board, T3 ball) {
        // Рисуем кадр из буфера
        imgDisplay.display((frame = frameBuffer)
                  // Рисуем ракетку-доску
                  .draw_image(board.getGameBoard(), (int) (board.X() - board.widthHalf()), height() - board.height() - ball.height())
                  // Рисуем шарик
                  .draw_image(ball.getImage(), (int) (ball.getX() - ball.widthHalf()), (int) (ball.getY() - ball.heightHalf())));

        // Задержка в милисекундах чтобы не было слишком уж быстро
        if (imgDisplay.wait(20).is_resized) imgDisplay.resize(imgDisplay);
    }

    int width() const { return widthFull; }
    int height() const { return heightFull; }
    int widthHalf() const { return WidthHalf; }
    int heightHalf() const { return HeightHalf; }

    int dimY() { return gamePad.dimy(); }
    double Sum() { return gamePad.sum(); }

    bool getGamePadValue(int x, int y) { return gamePad(x, y); }

    CImg<unsigned char> getFrame() { return frame; }
};