#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <tcpserver.h>
#include <tcpclient.h>
#include <numberconverter.h>
/*
    * @brief класс игры
    */

class Game : public QObject
{
    Q_OBJECT
public:

    /*
    * @brief конструктор по умолчанию
    */
    Game(QObject* parent = nullptr);

    /*
    * @brief конструктор онлайн
    */
    Game(TcpServer* TCPSERVER, uint Lowest, uint Highest, int Amount, QObject* parent = nullptr);

    /*
    * @brief функция начала игры
    */
    void StartGame(uint, uint, int);

    /*
    * @brief функция начала игры в онлайн
    */
    void StartMultiplayerGame();

    /*
    * @brief проверка ответа
    */
    void CheckAnswer(QString);

    /*
    * @brief проверка ответа клиента
    */
    void ClientCheckAnswer(QString);

    /*
    * @brief функия инкремента
    */
    QString Increment();

    /*
    * @brief функция получения текущего счета
    */
    QString GetCurrentNumber();

    /*
    * @brief функция установка вопроса и получения ответа
    */
    void SetQuestionAndAnswer(QString, QString);

    int Score = 0;                              // счет

    /*
    * @brief функция проверки ответа сервера
    */
    QString CheckServerAnswer(QString);

    /*
    * @brief функция пропуска ответа клиента
    */
    void SkipClientAnswer(int);

    /*
    * @brief функция обновления игры
    */
    void ResetGame();

    /*
    * @brief функция установки имени пользователя
    */
    void SetGameUsername(QString);

    /*
    * @brief функция перевода на французский
    */
    QString NumberToFrench(uint);

    /*
    * @brief функция перевода на английский
    */
    QString NumberToEnglish(uint);

private slots:

    /*
    * @brief слот добавления клиента
    */
    void AddClient(QString, int);

    /*
    * @brief слот обновления клиента
    */
    void ClientUpdate(QString);

    /*
    * @brief слот проверки ответа клиента
    */
    void CheckClientAnswer(QString, int);

    /*
    * @brief слот проверки дисконекта клиента
    */
    void HandleClientDisconnect(int);

signals:
    /*
    * @brief сигнал обновления клиента
    */
    void ClientUpdated(QString);

    /*
    * @brief сигнал обновление онлайн клиента
    */
    void ClientGameUpdate(ServerMessageTypes, int, QString, int = 0);

    /*
    * @brief сигнал окончания игры
    */
    void GameEnded(int, bool);

    /*
    * @brief сигнал обновления счета
    */
    void GameScoreUpdate(int, int, bool);

private:
    NumberConverter numberConverter;
    TcpServer* tcpServer = nullptr;
    TcpClient* tcpClient = nullptr;
    // ClientId : {"Score : ""}, {"Counter : ""}}
    QHash<int, QHash<QString, int>> ClientScores;
    void GenerateNumbers();
    QVector<QString> FrenchNumbers;
    QVector<QString> EnglishNumbers;
    QVector<QString>::iterator CurrentNumber;
    QVector<QString>::Iterator CurrentAnswer;
    QString SingleAnswer;
    QString SingleQuestion;
    int AmountOfNumbers;
    uint LowestNumber;
    uint HighestNumber;
    bool GameFinished;
};

#endif // GAME_H
