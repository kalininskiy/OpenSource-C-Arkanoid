//
// 2015 (c) Copyright Ivan "VDM" Kalininskiy
//
// Класс объекта кирпич
//
class Brick : public RectangleElement {
private:
    // Размер элемента кирпича
    int brickXsize;
    int brickYsize;

    // Задаем кирпич
    CImg<unsigned char> gameBrick;

public:
    // Конструктор класса
    Brick() : RectangleElement() {
        brickXsize = sizeX / 2;
        brickYsize = sizeY;

        // Задаем кирпич
        gameBrick = CImg<unsigned char> (brickXsize, brickYsize, 1, 1, 200);
        gameBrick.resize(sizeX - 1, sizeY - 1, 1, 1, 1)
                 .resize(sizeX, sizeY, 1, 1, 0);
    }

    CImg<unsigned char> getBrick() { return gameBrick; }

    int getX() const { return gameBrick.dimx(); }
    int getY() const { return gameBrick.dimy(); }
};