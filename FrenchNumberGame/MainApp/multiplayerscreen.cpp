#include "multiplayerscreen.h"
#include "ui_multiplayerscreen.h"

MultiPlayerScreen::MultiPlayerScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultiPlayerScreen)
{
    ui->setupUi(this);


    connect(ui->StartServerButton, &QPushButton::clicked, this, &MultiPlayerScreen::StartServerClicked);
    connect(ui->JoinServerButton, &QPushButton::clicked, this, &MultiPlayerScreen::JoinServerButtonClicked);
    connect(ui->MainMenuButton, &QPushButton::clicked, this, &MultiPlayerScreen::MenuButtonClicked);
    connect(ui->StartGameButton, &QPushButton::clicked, this, &MultiPlayerScreen::StartGameButtonClicked);

    connect(ui->EnterButton, &QPushButton::clicked, this, &MultiPlayerScreen::EnterButtonClicked);
    connect(ui->SkipButton, &QPushButton::clicked, this, &MultiPlayerScreen::SkipButtonClicked);
    connect(ui->ClearButton, &QPushButton::clicked, this, [this](){ui->UserInput->setText("");});

    connect(ui->UsernameEnterButton, &QPushButton::clicked, this, &MultiPlayerScreen::UsernameEnterButtonClicked);
    connect(ui->ReadyButton, &QPushButton::clicked, this, [this](){emit UserReady("UserTable");});
    connect(ui->ReadyButton, &QPushButton::clicked, this, [this]{UpdateUserStatus("Client", Username);});
    connect(ui->UsernameMenuButton, &QPushButton::clicked, this, &MultiPlayerScreen::MenuButtonClicked);
}


void MultiPlayerScreen::PlayAgainButtonClicked()
{
    emit UserReady("Scoreboard");
}

void MultiPlayerScreen::StartServerClicked()
{
    // Maybe add validators for this.
    ServerAddress = QHostAddress(ui->HostInput->text());
    ServerPort = ui->PortInput->text().toUInt();
    ui->stackedWidget->setCurrentIndex(4);
}

void MultiPlayerScreen::ClientUsernameAccepted()
{
    AddUserToTable("Client", Username, false);
    ui->stackedWidget->setCurrentIndex(2);
    ui->UsernameLabel->setText(tr("Please enter a username"));
}

void MultiPlayerScreen::UsernameEnterButtonClicked()
{
    // Add some checks for this.
    Username = ui->UsernameInputBox->text();
    QString UsernameInput = ui->UsernameInputBox->text();
    if(!tcpClient.isNull())
    {
        emit ClientSendUsername(UsernameInput);
        return;
    }
    ServerHostUsername = Username;
    tcpServer = new TcpServer(ServerAddress, ServerPort, UsernameInput, this);
    connect(this, &MultiPlayerScreen::CloseServer, tcpServer, &TcpServer::CloseServer);
    connect(tcpServer, &TcpServer::ClientUsernameRecieved, this, [this](QString Username){AddUserToTable("Server", Username, false);});
    connect(tcpServer, &TcpServer::ClientStateChanged, this, [this](QString Username){UpdateUserStatus("Server", Username);});
    connect(tcpServer, &TcpServer::ClientScoreboardStateChanged, this, [this](QString Username){emit UpdateScoreboardState(Username);});
    connect(tcpServer, &TcpServer::RemoveClientFromTable, this, [this](QString Username){RemoveUserFromTable("Server", Username);});
    connect(tcpServer, &TcpServer::SendUsernameAndScore, this, [this](QString Username, int Score, bool State){emit GameScoreUpdated(Score, Username, State);});
    AddUserToTable("Server", Username, true);
    ui->stackedWidget->setCurrentIndex(1);
    ui->ServerChatBox->appendPlainText(tcpServer->GetIpAndPort());
}

void MultiPlayerScreen::AddUserToTable(QString TableName, QString Username, bool BServer)
{
    QTableWidget* Table = TableName == "Server" ? ui->ServerUserTable : ui->ClientUserTable;
    int NextRow = Table->rowCount();
    Table->insertRow(NextRow);
    QTableWidgetItem* IdItem = new QTableWidgetItem(Username);
    QTableWidgetItem* ReadyItem = new QTableWidgetItem(tr(""));
    Table->setItem(NextRow, 0, IdItem);
    Table->setItem(NextRow, 1, ReadyItem);
    Table->item(NextRow, 1)->setBackground(QColor(255,255,255));
    if(BServer)
        Table->item(NextRow, 1)->setBackground(QColor(0,255,0));
}

void MultiPlayerScreen::RemoveUserFromTable(QString TableName, QString Username)
{
    QTableWidget* Table = TableName == "Server" ? ui->ServerUserTable : ui->ClientUserTable;
    QList<QTableWidgetItem*> Matches = Table->findItems(Username, Qt::MatchExactly);
    int UsernameRow = Table->row(Matches[0]);
    Table->removeRow(UsernameRow);
}

void MultiPlayerScreen::UpdateUserStatus(QString TableName, QString Username)
{
    QTableWidget* Table = TableName == "Server" ? ui->ServerUserTable : ui->ClientUserTable;
    QList<QTableWidgetItem*> Matches = Table->findItems(Username, Qt::MatchExactly);
    int UsernameRow = Table->row(Matches[0]);
    QColor CurrentColour = Table->item(UsernameRow, 1)->background().color();
    QColor newColour = CurrentColour == QColor("lime") ? QColor(255,255,255) : QColor("lime");
    Table->item(UsernameRow, 1)->setBackground(newColour);
}

void MultiPlayerScreen::ResetTableStatus(QString TableName)
{
    QTableWidget* Table = TableName == "Server" ? ui->ServerUserTable : ui->ClientUserTable;
    for(int row = 0; row < Table->rowCount(); row++)
    {
        if(Table->item(row, 0)->text() == ServerHostUsername)
            continue;
        Table->item(row, 1)->setBackground(QColor(255,255,255));
    }
}

void MultiPlayerScreen::ChangeWidgetIndex(int Index)
{
    // Changes the widget depending on if the socket is open
    // End screen -> lobby only if the server is still open.
    if(!tcpClient.isNull())
    {
        ResetTableStatus("User");
        ui->stackedWidget->setCurrentIndex(Index);
    }
    else if(!tcpServer.isNull())
    {
        // If going to lobby then we need to delete the old game.
        if(Index == 1)
        {
            ResetTableStatus("Server");
            game->deleteLater();
        }
        // If we are playing again then a signal needs to be outputted to remove the old scores.
        else if(Index == 4)
        {
            emit GamePlayAgainReset();
        }
        ui->stackedWidget->setCurrentIndex(Index);
    }
    // If no socket then there must be no game open.
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void MultiPlayerScreen::DisconnectSocket()
{
    // Called when you exit the game but don't actually close the application.
    if(!tcpServer.isNull())
    {
        tcpServer->deleteLater();
        game->deleteLater();
        ui->ServerUserTable->setRowCount(0);
    }
    else if(!tcpClient.isNull())
    {
        qDebug() << "This is called!";
        tcpClient->deleteLater();
        ui->ClientUserTable->setRowCount(0);
    }
}

void MultiPlayerScreen::JoinServerButtonClicked()
{
    QHostAddress ServerIP = QHostAddress(ui->HostInput->text());
    quint16 ServerPort = ui->PortInput->text().toInt();
    // Add validators for this!
    tcpClient = new TcpClient;
    connect(tcpClient, &TcpClient::ClientErrorSignal, this, &MultiPlayerScreen::DisplayError);
    connect(tcpClient, &TcpClient::ConnectionAccepted, this, &MultiPlayerScreen::ClientConnected);
    connect(tcpClient, &TcpClient::GameStarted, this, &MultiPlayerScreen::SetUpGameScreen);
    connect(tcpClient, &TcpClient::GameUpdated, this, [this](QString Question){ui->WordLabel->setText(Question);});
    connect(tcpClient, &TcpClient::GameEnded, this, [this](int Score, bool BClient){emit GameEnded(Score, BClient, Username);});
    connect(tcpClient, &TcpClient::GameScoreUpdate, [this](int Score, QString Username, bool State){emit GameScoreUpdated(Score, Username, State);});
    connect(tcpClient, &TcpClient::ClientDisconnected, this, &MultiPlayerScreen::HandleClientDisconnect);
    connect(tcpClient, &TcpClient::NewUserJoined, this, [this](QString Username){AddUserToTable("Client",Username, false);});
    connect(tcpClient, &TcpClient::ServerHostJoined, this, [this](QString Username){ServerHostUsername = Username, AddUserToTable("Client", Username, true);});
    connect(tcpClient, &TcpClient::UserStateChanged, this, [this](QString Username){UpdateUserStatus("Client", Username);});
    connect(tcpClient, &TcpClient::OtherClientDisconnected, this, [this](QString Username){RemoveUserFromTable("Client", Username);});
    connect(tcpClient, &TcpClient::ClientScoreboardStateChanged, this, [this](QString Username){emit UpdateScoreboardState(Username);});
    connect(tcpClient, &TcpClient::ClientReturnToLobby, this, [this](){emit CanReturnToLobby(2);});
    connect(tcpClient, &TcpClient::UsernameAlreadyTaken, this, [this](){ui->UsernameLabel->setText(tr("Username Already Taken, Please Enter Another!"));});
    connect(tcpClient, &TcpClient::UsernameAccepted, this, &MultiPlayerScreen::ClientUsernameAccepted);
    connect(tcpClient, &TcpClient::GameInProgress, this, &MultiPlayerScreen::HandleGameInProgress);
    connect(this, &MultiPlayerScreen::ClientSendAnswer, tcpClient, &TcpClient::RequestNewAnswer);
    connect(this, &MultiPlayerScreen::ClientSendUsername, tcpClient, &TcpClient::SendUsername);
    connect(this, &MultiPlayerScreen::UserReady, tcpClient, &TcpClient::SendUserStatus);
    tcpClient->ConnectToServer(ServerIP, ServerPort);
}

void MultiPlayerScreen::HandleGameInProgress()
{
    // Display an error message here.
    ui->TitleLabel->setText(tr("That game is already in progress!"));
    tcpClient->deleteLater();
    ui->stackedWidget->setCurrentIndex(0);
}

void MultiPlayerScreen::HandleClientDisconnect()
{
    ui->stackedWidget->setCurrentIndex(0);
    tcpClient->deleteLater();
    ui->ClientUserTable->setRowCount(0);
}

void MultiPlayerScreen::SetUpGameScreen(QString question)
{
    emit CheckAndChangeWidget();
    ui->stackedWidget->setCurrentIndex(3);
    ui->WordLabel->setText(question);
}

void MultiPlayerScreen::EnterButtonClicked()
{
    // If server then just update normally
    QString UserAnswer = ui->UserInput->text().trimmed();
    if(tcpServer)
    {
        QString CurrentNumber = game->CheckServerAnswer(UserAnswer);
        if(CurrentNumber.isEmpty())
        {
            ui->UserInput->setText("");
            return;
        }
        ui->WordLabel->setText(CurrentNumber);
    }
    // If client then send message and ask for new word.
    else if(tcpClient)
    {
        emit ClientSendAnswer(UserAnswer);
    }
    ui->UserInput->setText("");
}

void MultiPlayerScreen::SkipButtonClicked()
{
    if(tcpServer)
    {
        QString CurrentNumber = game->CheckServerAnswer("");
        if(CurrentNumber.isEmpty())
        {
            return;
        }
        ui->WordLabel->setText(CurrentNumber);
    }
    else if(tcpClient)
    {
        emit ClientSendAnswer("");
    }
    ui->UserInput->setText("");
}

void MultiPlayerScreen::ClientConnected()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->ClientChatBox->appendPlainText(tr("Welcome to the server!"));
}


void MultiPlayerScreen::StartGameButtonClicked()
{
    long Lowest = ui->LowestInput->text().toUInt();
    long Highest = ui->HighestInput->text().toUInt();
    int Amount = ui->AmountInput->text().toUInt();
    if(Highest < Lowest)
    {
        ui->ErrorLabel->setText(tr("Highest must be greater or equal to lowest!"));
    }
    else if(Lowest == 0)
    {
        ui->ErrorLabel->setText(tr("Lowest must be greater than zero!"));
    }
    if(!tcpServer->AllPlayersReady())
    {
        ui->StartGameButton->setText("Not all players are ready!");
        return;
    }
    game = new Game(tcpServer, Lowest, Highest, Amount, this);
    game->StartMultiplayerGame();
    ui->stackedWidget->setCurrentIndex(3);
    ui->WordLabel->setText(game->GetCurrentNumber());
    connect(game, &Game::GameEnded, this, [this](int Score, bool BClient){emit GameEnded(Score, BClient, Username);});
    connect(this, &MultiPlayerScreen::GamePlayAgainReset, game, &Game::ResetGame);
    ui->StartGameButton->setText("Start Game");
}

void MultiPlayerScreen::CheckAllUsersReady(QString Button)
{

    if(!tcpServer->AllPlayersReady())
    {
        emit NotAllPlayersReady();
        return;
    }
    if(Button == "Again")
    {
        emit CanPlayAgain();
        game->StartMultiplayerGame();
        ui->WordLabel->setText(game->GetCurrentNumber());
    }
    // Else we are bring all the players back to the lobby
    else
    {
        emit CanReturnToLobby(1);
        // Turn this into a slot
        tcpServer->MessageAll(ServerMessageTypes::GameReturnToLobby, "");
    }
}

void MultiPlayerScreen::DisplayError(QString SocketError)
{
    ui->ErrorLabel->setText(SocketError);
    if(!tcpClient.isNull())
        tcpClient->deleteLater();
    else if(!tcpServer.isNull())
        tcpServer->deleteLater();
}

void MultiPlayerScreen::MenuButtonClicked()
{
    if(!tcpClient.isNull())
        tcpClient->deleteLater();
    else if(!tcpServer.isNull())
        tcpServer->deleteLater();
    emit MenuButtonClickedSignal();
}

MultiPlayerScreen::~MultiPlayerScreen()
{
    delete ui;
}
