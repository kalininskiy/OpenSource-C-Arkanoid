// TestTinyArkanoid.cpp
//
// 2015 (c) Copyright Ivan "VDM" Kalininskiy


// Используем однофайловую (.h) графическую библиотеку CImg - http://cimg.sourceforge.net/
#include "CImg.h"
using namespace cimg_library;

// Классы вынесены в отдельные файлы исходников (.h)
#include "RectangleElement.h" // Базовый класс примитивного элемента игрового поля (прямоугольника)
#include "GamePad.h" // Класс игрового поля
#include "Brick.h" // Класс объекта кирпич
#include "Ball.h" // Класс объекта шарик
#include "Board.h" // Класс объекта ракетки-доски
#include "Menu.h" // Класс игрового меню

// Начало Main()
//
int main()
{
    RectangleElement rectangle; // Объявляем базовый объект элемент игрового поля (прямоугольник)
    GamePad gamePad; // Объявляем объект игровое поле
    Brick brick; // Объявляем объект кирпич
    Ball ball; // Объявляем объект шарик
    Board board(gamePad.width()); // Объявляем объект ракетку-доску
    Menu menu; // Объявляем объект игровое меню
    CImgDisplay imgDisplay(gamePad.getFrame(), "Tiny Arkanoid", 0); // Объявляем объект окно

    // Перемещаем окно в центр экрана
    imgDisplay.move((CImgDisplay::screen_dimx() - gamePad.width()) / 2, (CImgDisplay::screen_dimy() - gamePad.height()) / 2);

    // Основной цикл приложения, проверяем закрытие окна и нажатие ESC
    //
    for (unsigned int playerScore = 0, bricksScore = 0;
                     !imgDisplay.is_closed && imgDisplay.key != cimg::keyESC && !menu.isExit();)
    {
        // Если шарик упал вниз (проигрыш) или если это начало (до старта) или конец игры (победа)
        if (ball.fallDown(gamePad) || playerScore == bricksScore)
        {
            imgDisplay.show_mouse(); // Показываем курсор мыши

            // Выводим меню или графику в зависимости от ситуации
            menu.showMenu(imgDisplay, gamePad, ball, playerScore, bricksScore);

            gamePad.clear(); // Очищаем игровое поле
            gamePad.fillRandomBricks(brick); // Заполняем игровое поле случайными кирпичами

            // Инициализируем шарик в центре игрового поля
            ball.initialize((float) gamePad.widthHalf(), gamePad.dimY() * rectangle.getSizeY() + ball.height());

            // Считываем и сохраняем начальное количество кирпичей и начальные очки пользователя
            bricksScore = (int) gamePad.Sum();
            playerScore = 0;

            imgDisplay.hide_mouse(); // Прячем курсор мыши
        }

        gamePad.display(imgDisplay, board, ball); // Отображаем кадр игрового поля
        int X = board.getMouseX(imgDisplay, gamePad); // Берем координату доски в зависимости от мышки
        board.move(X); // Двигаем ракетку-доску
        ball.move(); // Двигаем шарик

        // Проверяем шарик на всевозможные столкновения
        playerScore = ball.testCollisions(gamePad, brick, board, playerScore);

        // Отображаем оставшееся количество кирпичей
        imgDisplay.set_title("Left : %u", bricksScore - playerScore);
    }
    return 0; // Конец цикла и выход
}
// Здесь конец Main()