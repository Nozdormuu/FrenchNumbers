#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>

/*
    * @brief класс клиента сервера
    */
class TcpClient : public QObject
{
    Q_OBJECT
public:

    /*
   * @brief конструктор класса
   */
    TcpClient(QObject* parent = nullptr);

    /*
   * @brief функция подключения к серверу
   */
    void ConnectToServer(const QHostAddress HostIP, quint16 Port);

signals:

    /*
   * @brief сигнал ошибки
   */
    void ClientErrorSignal(QString);

    /*
   * @brief сигнал подключения клиента
   */
    void ClientConnectedSignal();

    /*
   * @brief сигнал начала игры
   */
    void GameStarted(QString);

    /*
   * @brief сигнал обновления игры
   */
    void GameUpdated(QString);

    /*
   * @brief сигнал конца игры
   */
    void GameEnded(int, bool);

    /*
   * @brief сигнал обновления счета
   */
    void GameScoreUpdate(int, QString, bool);

    /*
   * @brief сигнал дисконекта
   */
    void ClientDisconnected();

    /*
   * @brief сигнал вступления нового пользователя
   */
    void NewUserJoined(QString, bool, bool);

    /*
   * @brief сигнал изменения состояния
   */
    void UserStateChanged(QString);

    /*
   * @brief сигнал отключения другого клиента
   */
    void OtherClientDisconnected(QString);

    /*
   * @brief сигнал изменения доски
   */
    void ClientScoreboardStateChanged(QString);

    /*
   * @brief сигнал возврата в лобби
   */
    void ClientReturnToLobby();

    /*
   * @brief сигнал занятого имени
   */
    void UsernameAlreadyTaken();

    /*
   * @brief сигнал принятия имени
   */
    void UsernameAccepted();

    /*
   * @brief сигнал процесса игры
   */
    void GameInProgress();

    /*
   * @brief сигнал принятия подключения
   */
    void ConnectionAccepted();

    /*
   * @brief сигнал подключения к серверу
   */
    void ServerHostJoined(QString, bool, bool);

public slots:

    /*
   * @brief слот нового запроса
   */
    void RequestNewAnswer(QString);

    /*
   * @brief слот посылки имени
   */
    void SendUsername(QString);

    /*
   * @brief слот посылки статуса
   */
    void SendUserStatus(QString);

private slots:
    /*
   * @brief слот подключения клиента
   */
    void ClientConnected();

    /*
   * @brief слот получения сообщения
   */
    void MessageRecieved();

    /*
   * @brief слот дисконекта
   */
    void HandleDisconnect();


private:
    QTcpSocket* tcpSocket = nullptr;
    const QHostAddress HostIp;
    int Port;

    /*
   * @brief  функция ошибки
   */
    void HandleError(QAbstractSocket::SocketError);
    QDataStream in;

};

#endif // TCPCLIENT_H
