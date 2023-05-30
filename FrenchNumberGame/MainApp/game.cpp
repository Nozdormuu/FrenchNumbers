#include "game.h"
#include <QRandomGenerator64>
#include <QFile>
#include <QDebug>
#include <algorithm>

Game::Game(QObject *parent)
    : QObject{parent}, numberConverter{NumberConverter()}, GameFinished{false}
{
}

Game::Game(TcpServer* TCPSERVER, uint Lowest, uint Highest, int Amount, QObject *parent)
    : QObject(parent), numberConverter{NumberConverter()}, tcpServer(TCPSERVER),
      AmountOfNumbers(Amount), LowestNumber(Lowest), HighestNumber(Highest), GameFinished(false)
{
    connect(tcpServer, &TcpServer::ClientUsernameRecieved, this, &Game::AddClient);
    connect(tcpServer, &TcpServer::ClientAnswerRecieved, this, &Game::CheckClientAnswer);
    connect(tcpServer, &TcpServer::HandleClientDisconnect, this, &Game::HandleClientDisconnect);
    connect(this, &Game::ClientGameUpdate, tcpServer, &TcpServer::MessageClient);
    connect(this, &Game::GameScoreUpdate, tcpServer, &TcpServer::GetClientUsername);
}

void Game::AddClient(QString ClientUsername, int ClientId)
{
    QHash<QString, int> DefaultValues;
    DefaultValues["Score"] = 0;
    DefaultValues["Counter"] = 0;
    ClientScores.insert(ClientId, DefaultValues);
}

void Game::ResetGame()
{
    // Resets the client scores/counts to 0
    for(auto it = ClientScores.begin(); it != ClientScores.end(); it++)
    {
        auto key = it.key();
        ClientScores[key]["Counter"] = 0;
        ClientScores[key]["Score"] = 0;
    }
    GenerateNumbers();
    Score = 0;
    GameFinished = false;
}

void Game::HandleClientDisconnect(int ClientId)
{
    // If a client has left then remove them from the scores hashMap
    ClientScores.remove(ClientId);
}

QString Game::CheckServerAnswer(QString answer)
{
    if(answer == *CurrentAnswer)
        Score++;

    if(CurrentNumber + 1 == EnglishNumbers.end())
    {
        emit GameEnded(Score, false);
        QString ServerScore = QString::number(Score);
        for(auto it = ClientScores.begin(); it != ClientScores.end(); it++)
        {
            auto key = it.key();
            if(ClientScores[key]["Counter"] == AmountOfNumbers - 1)
            {
                emit ClientGameUpdate(ServerMessageTypes::GameScoreUpdate, key, ServerScore);
                emit GameScoreUpdate(ClientScores[key]["Score"], key, false);
            }
        }
        GameFinished = true;
        return "";
    }
    CurrentNumber++;
    CurrentAnswer++;
    return *CurrentNumber;
}

void Game::CheckClientAnswer(QString answer, int ClientId)
{
    qDebug() << "Check score is being called for client " << ClientId;
    // 1) Check the answer
    // 2) Increment score
    int CurrentPosition = ClientScores[ClientId]["Counter"];
    if(!answer.isEmpty() && answer == FrenchNumbers[CurrentPosition])
    {
        qDebug() << ClientScores[ClientId]["Score"];
        ClientScores[ClientId]["Score"]++;
        qDebug() << ClientScores[ClientId]["Score"];
    }

    // 3) Check if there is another element

    if(CurrentPosition == AmountOfNumbers - 1)
    {
        qDebug() << "This happened innside of checm answer!";
        // Emit some sort of game over signal
        // Then exit game loop
        QString ClientScore = QString::number(ClientScores[ClientId]["Score"]);
        emit ClientGameUpdate(ServerMessageTypes::GameEnd, ClientId, ClientScore);

        // Send all of the other scores where the client has finished their game.
        // && send your score to the people who have finished.
        for(auto it = ClientScores.begin(); it != ClientScores.end(); it++)
        {
            auto key = it.key();
            if(key != ClientId && ClientScores[key]["Counter"] == AmountOfNumbers - 1)
            {
                qDebug() << "This happened!";
                QString OtherClientScore = QString::number(ClientScores[key]["Score"]);
                emit ClientGameUpdate(ServerMessageTypes::GameScoreUpdate, ClientId, OtherClientScore, key);
                emit ClientGameUpdate(ServerMessageTypes::GameScoreUpdate, key, ClientScore, ClientId);
            }

            // If the game is finished then send the client score as well
        }

        if(GameFinished)
        {
            QString ServerScore = QString::number(Score);
            emit ClientGameUpdate(ServerMessageTypes::GameScoreUpdate, ClientId, ServerScore, 0);
            emit GameScoreUpdate(ClientScores[ClientId]["Score"], ClientId, false);
        }

        return;
    }

    // 4) Update ClientScores counter

    ClientScores[ClientId]["Counter"]++;

    // 5) Message client with new word!

    QString NewQuestion = EnglishNumbers[ClientScores[ClientId]["Counter"]];
    emit ClientGameUpdate(ServerMessageTypes::GameUpdate, ClientId, NewQuestion);
}

void Game::ClientUpdate(QString Data)
{
    emit ClientUpdated(Data);
}

void Game::GenerateNumbers()
{
    FrenchNumbers.clear();
    EnglishNumbers.clear();
    for(int i = 0; i < AmountOfNumbers; i++)
    {
        quint32 RandomNumber= QRandomGenerator64::global()->bounded(LowestNumber, HighestNumber);
        QString FrenchWord = numberConverter.NumberToFrench(RandomNumber);
        QString EnglishWord = numberConverter.NumberToEnglish(RandomNumber);
        FrenchNumbers.push_back(FrenchWord);
        EnglishNumbers.push_back(EnglishWord);
    }
    CurrentNumber = EnglishNumbers.begin();
    CurrentAnswer = FrenchNumbers.begin();
}

void Game::StartGame(uint Lowest, uint Highest, int Amount)
{
    AmountOfNumbers = Amount;
    HighestNumber = Highest;
    LowestNumber= Lowest;
    GenerateNumbers();
}

void  Game::StartMultiplayerGame()
{
    // Look at the reset game function.
    GenerateNumbers();
    for(auto it = ClientScores.begin(); it != ClientScores.end(); it++)
    {
        auto key = it.key();
        ClientScores[key]["Score"] = 0;
        ClientScores[key]["Counter"] = 0;
    }
    Score = 0;
    GameFinished = false;
    // Change this to a signal!
//  emit GameStarted(*CurrentNumber);
    qDebug() << "The first number is " << *CurrentNumber;
    tcpServer->MessageAll(ServerMessageTypes::GameStart, *CurrentNumber);
}

void Game::SetQuestionAndAnswer(QString answer, QString question)
{
    SingleAnswer = answer;
    SingleQuestion =question;
}

QString Game::GetCurrentNumber()
{
    return *CurrentNumber;
}

void Game::ClientCheckAnswer(QString UserAnswer)
{
    if(UserAnswer == SingleAnswer)
        Score++;
    qDebug() << "Client score" << Score;
}

void Game::CheckAnswer(QString UserAnswer)
{
    if(UserAnswer == *CurrentAnswer)
        Score++;
}

QString Game::Increment()
{
    if(CurrentNumber + 1 == EnglishNumbers.end())
    {
        return "";
    }
    CurrentNumber++;
    CurrentAnswer++;
    return *CurrentNumber;
}

