#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <QMainWindow>
#include <game.h>

namespace Ui {
class GameScreen;
}
/*
   * @brief класс экрана игры
   */
class GameScreen : public QMainWindow
{
    Q_OBJECT

public:

    /*
   * @brief конструктор окна игры
   */
    explicit GameScreen(QWidget* parent = nullptr);

    /*
   * @brief функция начала игры
   */
    void StartGame(uint, uint, int);

    /*
   * @brief деструктор
   */
    ~GameScreen();

private slots:

    /*
   * @brief слот нажатия кнопки входа
   */
    void EnterButtonClicked();

signals:

    /*
   * @brief сигнал окончания игры
   */
    void GameOverSignal(int);

private:
    Ui::GameScreen* ui;

    /*
   * @brief проверка окончания игры
   */
    void CheckIfGameOver();

    QString CurrentNumber;
    Game* game;
};

#endif // GAMESCREEN_H
