#ifndef MULTIPLAYERSCREEN_H
#define MULTIPLAYERSCREEN_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <tcpserver.h>
#include <tcpclient.h>
#include <game.h>
#include <endscreen.h>
#include <QPointer>


namespace Ui {
class MultiPlayerScreen;
}

/*
 * @brief класс окна для многопользовательского режима
 */
class MultiPlayerScreen : public QWidget
{
    Q_OBJECT

public:
    /*
   * @brief конструктор
   */
    explicit MultiPlayerScreen(QWidget* parent = nullptr);

    /*
   * @brief деструктор
   */
    ~MultiPlayerScreen();

public slots:

    /*
   * @brief слот дисконекта
   */
    void DisconnectSocket();

    /*
   * @brief слот изменения индекса
   */
    void ChangeWidgetIndex(int);

    /*
   * @brief слот начала игры заново
   */
    void PlayAgainButtonClicked();

    /*
   * @brief слот проверки готовности игроков
   */
    void CheckAllUsersReady(QString);

signals:

    /*
   * @brief сигнал нажатия меню
   */
    void MenuButtonClickedSignal();

    /*
   * @brief сигнал начала игры
   */
    void GameStarted(QString);

    /*
   * @brief сигнал конца игры
   */
    void GameEnded(int, bool, QString);

    /*
   * @brief сигнал отправки сообщения
   */
    void ClientSendAnswer(QString);

    /*
   * @brief сигнал обновления счета
   */
    void GameScoreUpdated(int, QString, bool);

    /*
   * @brief сигнал выключения сервера
   */
    void CloseServer();

    /*
   * @brief сигнал отключеня клиента
   */
    void CloseClient();

    /*
   * @brief сигнал обновления партии
   */
    void GamePlayAgainReset();

    /*
   * @brief сигнал посылки имени
   */
    void ClientSendUsername(QString);

    /*
   * @brief сигнал готовности пользователя
   */
    void UserReady(QString);

    /*
   * @brief сигнал обновления состояния доски
   */
    void UpdateScoreboardState(QString);

    /*
   * @brief сигнал переигрыша
   */
    void CanPlayAgain();

    /*
   * @brief сигнал неготовности всех игровков
   */
    void NotAllPlayersReady();

    /*
   * @brief сигнал изменения и проверки виджета
   */
    void CheckAndChangeWidget();

    /*
   * @brief сигнал возмодности вернуться в лобби
   */
    void CanReturnToLobby(int);

private slots:

    /*
   * @brief слот вступления на сервер
   */
    void JoinServerButtonClicked();

    /*
   * @brief слот нажатия меню
   */
    void MenuButtonClicked();

    /*
   * @brief слот подключения клиента
   */
    void ClientConnected();

    /*
   * @brief слот нажатия начала игры
   */
    void StartGameButtonClicked();

    /*
   * @brief слот ошибки окна
   */
    void DisplayError(QString);

    /*
   * @brief слот установки экрана
   */
    void SetUpGameScreen(QString);

    /*
   * @brief слот  нажатия пропуска
   */
    void SkipButtonClicked();

    /*
   * @brief слот нажатия входа
   */
    void EnterButtonClicked();

    /*
   * @brief слот ввода имени
   */
    void UsernameEnterButtonClicked();

    /*
   * @brief слот дисконекта
   */
    void HandleClientDisconnect();

    /*
   * @brief слот принятия имени
   */
    void ClientUsernameAccepted();

    /*
   * @brief слот игры в прогрессе
   */
    void HandleGameInProgress();

    /*
   * @brief слот включения сервера
   */
    void StartServerClicked();

private:

    /*
   * @brief функция добавления пользователя в таблицу
   */
    void AddUserToTable(QString, QString, bool);

    /*
   * @brief функция обновления статуса пользователя
   */
    void UpdateUserStatus(QString, QString);

    /*
   * @brief функция переропределения таблицы статусов
   */
    void ResetTableStatus(QString);

    /*
   * @brief функция удаления пользователя
   */
    void RemoveUserFromTable(QString, QString);
    QHostAddress ServerAddress;
    quint16 ServerPort;
    Ui::MultiPlayerScreen* ui;
    QPointer<TcpClient> tcpClient;
    QPointer<TcpServer> tcpServer;
    Game* game = nullptr;
    QString Username;
    QString ServerHostUsername;
};

#endif // MULTIPLAYERSCREEN_H
