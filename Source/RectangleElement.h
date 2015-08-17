//
// 2015 (c) Copyright Ivan "VDM" Kalininskiy
//
// Базовый класс примитивного элемента игрового поля (прямоугольника)
//
class RectangleElement {
protected:
    // Размер элемента игрового поля (прямоугольника)
    int sizeX;
    int sizeY;

public:
    // Конструктор класса
    RectangleElement() {
        // Инициализируем размер элемента (прямоугольника)
        sizeX = 32;
        sizeY = 16;
    }

    int getSizeX() const { return sizeX; }
    int getSizeY() const { return sizeY; }
};