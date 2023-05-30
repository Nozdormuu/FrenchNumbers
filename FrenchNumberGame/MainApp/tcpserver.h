#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

/*
* @brief объединение типов сообщений сервера
*/
    enum class ServerMessageTypes
{
    GameStart,
    GameEnd,
    GameUpdate,
    GameScoreUpdate,
    GameSendUsername,
    GameSendServerUsername,
    GameUpdateUserStatus,
    GameClientDisconnect,
    GameReturnToLobby,
    GameUsernameTaken,
    GameUsernameAccepted,
    GameInProgress,
    GameConnectionAccepted,
};

/*
 * @brief класс сервера
 */
class TcpServer : public QObject
{
    Q_OBJECT

public:
    /*
    * @brief конструктор
    */
    TcpServer(const QHostAddress HostIP, quint16 Port, QString Username, QObject* parent = nullptr);

    /*
    * @brief функция оповещения пользователя
    */
    void MessageAll(ServerMessageTypes, QString);

    /*
    * @brief функция проверки пользователей
    */
    bool AllPlayersReady();

    QHash<QTcpSocket*, int> ClientIds;
    QHash<int, QTcpSocket*> ClientIndex;
    QHash<int, bool> ClientStates;
    bool GameStarted;

    /*
    * @brief функция получения IP
    */
    QString GetIpAndPort();

signals:

    /*
    * @brief сигнал получения оповещения
    */
    void ClientAnswerRecieved(QString, int);

    /*
    * @brief сигнал пропуска
    */
    void ClientSkipRecieved(int);

    /*
    * @brief сигнал дисконекта
    */
    void HandleClientDisconnect(int);

    /*
    * @brief сигнал получения имени
    */
    void ClientUsernameRecieved(QString, int);

    /*
    * @brief сигнал посылания имени и счета
    */
    void SendUsernameAndScore(QString, int, bool);

    /*
    * @brief сигнал изменения состояния клиента
    */
    void ClientStateChanged(QString);

    /*
    * @brief сигнал удаления клиента из таблицы
    */
    void RemoveClientFromTable(QString);

    /*
    * @brief сигнал изменения таблицы
    */
    void ClientScoreboardStateChanged(QString);

public slots:

    /*
    * @brief слот закрытия сервера
    */
    void CloseServer();

    /*
    * @brief слот оповещения клиента
    */
    void MessageClient(ServerMessageTypes, int, QString, int UserId = 0);

    /*
    * @brief слот получения имени
    */
    void GetClientUsername(int, int, bool);

private slots:

    /*
    * @brief слот подключения клиента
    */
    void ClientConnected();

    /*
    * @brief слот прочтения сообщения
    */
    void ReadMessage();

    /*
    * @brief слот дисконекта
    */
    void HandleDisconnect();

private:
    /*
    * @brief функция создания jsona
    */
    QJsonObject CreateUsernameScoreJsonObject(QString, int, int);

    /*
    * @brief функция отправки
    */
    void MessageOtherClients(ServerMessageTypes, QString, int);

    /*
    * @brief функция передачи данных другим
    */
    void MessageOtherClientsData(ServerMessageTypes, QJsonObject, int);

    /*
    * @brief функция  отправки данных
    */
    void MessageClientData(ServerMessageTypes, QJsonObject, int);

    /*
    * @brief создание байт массива
    */
    QByteArray CreateQByteArray(QJsonObject object);
    QTcpServer* tcpServer = nullptr;
    QVector<QTcpSocket*> Connections;
    const QHostAddress HostIp;
    int Port;
    QDataStream in;
    int UserCount;
    QHash<int, QString> ClientUsernames;
    QString ServerUsername;
};

#endif // TCPSERVER_H
