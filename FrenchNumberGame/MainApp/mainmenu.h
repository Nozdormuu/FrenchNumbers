#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <gamescreen.h>
#include <endscreen.h>
#include <multiplayerscreen.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE
/*
   * @brief класс главного меню
   */

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    /*
   * @brief конструктор класс
   */
    MainMenu(QWidget* parent = nullptr);

    /*
   * @brief деструктор
   */
    ~MainMenu();

private slots:

    /*
   * @brief слот нажатия кнопки старта
   */
    void StartButtonClicked();

    /*
   * @brief слот установки финальной сцены
   */
    void SetEndScreenWidget(int);

    /*
   * @brief слот установки финальной сцены множества игроков
   */
    void SetMultiPlayerEndScreenWidget(int, bool, QString);

    /*
   * @brief слот возврата в лобби
   */
    void HandleReturnToLobby(int);

    /*
   * @brief слот измениния и проверки
   */
    void CheckAndChangeWidget();

signals:

    /*
   * @brief сигнал начала игры
   */
    void GameStarted();

    /*
   * @brief сигнал множественного изменения
   */
    void MultiChangeWidgetIndex(int);

private:
    Ui::MainMenu* ui;
    GameScreen* GamePage = new GameScreen;
    EndScreen* EndPage = new EndScreen;
    MultiPlayerScreen* MultiPlayerPage = new MultiPlayerScreen;
};
#endif // MAINMENU_H
