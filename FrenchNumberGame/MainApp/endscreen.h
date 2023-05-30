#ifndef ENDSCREEN_H
#define ENDSCREEN_H

#include <QWidget>
#include <tcpclient.h>

namespace Ui {
class EndScreen;
}
/*
   * @brief класс финальная сцена
   */
class EndScreen : public QWidget
{
    Q_OBJECT

public:

    /*
   * @brief конструктор для класса
   */
    explicit EndScreen(QWidget* parent = nullptr);

    /*
   * @brief функция для установки режима мультиплеер
   */
    void MultiPlayerSetUp(int, bool, QString);

    /*
   * @brief функция изменения счета
   */
    void SetScore(int);

    /*
   * @brief деструктор
   */
    ~EndScreen();

public slots:

    /*
   * @brief слот обновления доски со счетом
   */
    void UpdateScoreboard(int, QString, bool);

    /*
   * @brief слот обновления состояния доски
   */
    void UpdateScoreboardState(QString);

    /*
   * @brief слот проверки готовности игроков
   */
    void NotAllPlayersReady();

signals:

    /*
   * @brief сигнал нажатия кнопки меню
   */
    void MenuButtonClicked();

    /*
   * @brief сигнал нажатия кнопки мультименю
   */
    void MultiMenuButtonClicked();

    /*
   * @brief сигнал нажатия кнопки назад
   */
    void AgainButtonClicked();

    /*
   * @brief сигнал нажатия кнопки лобби
   */
    void MultiLobbyButtonClicked(int);

    /*
   * @brief сигнал нажатия кнопки назад
   */
    void MultiPlayAgainClicked();

    /*
   * @brief сигнал изменения состояния клиента
   */
    void ClientStateChanged(QString);

    /*
   * @brief сигнал проверки от сервера
   */
    void ServerCheckStatuses(QString);


private:
    Ui::EndScreen* ui;
    TcpClient* tcpClient = nullptr;
    bool IsClient;
    QString ClientUsername;

    /*
   * @brief функция нажатия кнопки
   */
    void HandleMultiplayerButtonClicked(QString);
    /*
   * @brief функция изменения счета
   */
    void UpdateUserState();
};

#endif // ENDSCREEN_H
