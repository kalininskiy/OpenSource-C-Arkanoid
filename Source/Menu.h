//
// 2015 (c) Copyright Ivan "VDM" Kalininskiy
//
// Класс игрового меню
//
class Menu {
private:
    CImg<unsigned char> gameOver;
    CImg<unsigned char> gameWin;
    CImg<unsigned char> gameMenu;
    CImg<unsigned char> gameMenuStart;
    CImg<unsigned char> gameMenuExit;

    bool start_signal = false;
    bool exit_signal = false;

public:
    Menu() {
        gameOver = CImg<unsigned char> ("resources//GameOver.res");
        gameWin = CImg<unsigned char> ("resources//YouWin.res");
        gameMenu = CImg<unsigned char> ("resources//MainMenu.res");
        gameMenuStart = CImg<unsigned char> ("resources//MainStart.res");
        gameMenuExit = CImg<unsigned char> ("resources//MainExit.res");
    }

    // Обработка результирующих экранов и игрового меню
    template <class T1, class T2, class T3, typename T4, typename T5>
    void showMenu(T1 &imgDisplay, T2 gamePad, T3 ball, T4 playerScore, T5 bricksScore) {
        start_signal = exit_signal = false;

        // Если это начало игры, то просто вызываем игровое меню
        if ((playerScore == bricksScore) && (playerScore == 0)) {
            startMenu(imgDisplay);
        }

        // Если уронил шарик, то ГеймОвер
        if (ball.fallDown(gamePad)) {
            imgDisplay.display(gameOver);
            waitUser(imgDisplay);
            startMenu(imgDisplay);
        }

        // Если выиграл, то Экран Победы
        if ((playerScore == bricksScore) && (playerScore != 0)) {
            imgDisplay.display(gameWin);
            waitUser(imgDisplay);
            startMenu(imgDisplay);
        }
    }

    // Ожидание реакции пользователя
    template <class T>
    void waitUser(T &imgDisplay) const {
        while (!imgDisplay.key && !imgDisplay.is_closed && !imgDisplay.button)
            {
                imgDisplay.wait();
                if (imgDisplay.is_resized) imgDisplay.resize(imgDisplay);
            }
    }

    // Рисуем и обрабатываем игровое меню
    template <class T>
    void startMenu(T &imgDisplay) {
        imgDisplay.display(gameMenu);
        while (!start_signal && !exit_signal) {
            imgDisplay.wait();
            if (imgDisplay.is_resized) imgDisplay.resize(imgDisplay);

            if ((imgDisplay.mouse_x >= 0) && (imgDisplay.mouse_x <= 256) &&
                (imgDisplay.mouse_y >= 195) && (imgDisplay.mouse_y <= 240)) {
                imgDisplay.display(gameMenuStart);
                if (imgDisplay.button) start_signal = true;
            } else if ((imgDisplay.mouse_x >= 0) && (imgDisplay.mouse_x <= 256) &&
                       (imgDisplay.mouse_y >= 245) && (imgDisplay.mouse_y <= 275)) {
                imgDisplay.display(gameMenuExit);
                if (imgDisplay.button) exit_signal = true;
            } else {
                imgDisplay.display(gameMenu);
            }
        }
    }

    bool isExit() { return exit_signal; }
};