#include "endscreen.h"
#include "ui_endscreen.h"

EndScreen::EndScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndScreen)
{
    ui->setupUi(this);
    ui->Scoreboard->sortByColumn(1, Qt::SortOrder::DescendingOrder);

    connect(ui->MultiMenuButton, &QPushButton::clicked, this, [=](){
        emit MultiMenuButtonClicked();
        emit MenuButtonClicked();});

    connect(ui->MultiLobbyButton, &QPushButton::clicked, this, [this](){HandleMultiplayerButtonClicked("Lobby");});
    connect(ui->MultiAgainButton, &QPushButton::clicked, this, [this](){HandleMultiplayerButtonClicked("Again");});
    connect(ui->AgainButton, &QPushButton::clicked, this, [this](){emit AgainButtonClicked();});
    connect(ui->MenuButton, &QPushButton::clicked, this, [this](){emit MenuButtonClicked();});
}

void EndScreen::SetScore(int Score)
{
    QString ScoreText = QString("You scored %1!").arg(Score);
    ui->ScoreLabel->setText(ScoreText);
}

//this is largely the same as the method in multiplayerscreen
void EndScreen::UpdateScoreboardState(QString Username)
{
    QList<QTableWidgetItem*> Matches = ui->Scoreboard->findItems(Username, Qt::MatchExactly);
    int UsernameRow = ui->Scoreboard->row(Matches[0]);
    QColor CurrentColour = ui->Scoreboard->item(UsernameRow, 2)->background().color();
    QColor newColour = CurrentColour == QColor("lime") ? QColor(255,255,255) : QColor("lime");
    ui->Scoreboard->item(UsernameRow, 2)->setBackground(newColour);
}

void EndScreen::NotAllPlayersReady()
{
    ui->MultiAgainButton->setText(tr("Not all players are ready!"));
}

void EndScreen::HandleMultiplayerButtonClicked(QString Button)
{
    // If Client then update your state and send it to the server
    if(IsClient)
    {

        emit ClientStateChanged("Scoreboard");
        UpdateScoreboardState(ClientUsername);
        return;
    }
    emit ServerCheckStatuses(Button);
    // Else your the server and you want to try and play again
    // We have to check all the states are ready.
}

void EndScreen::UpdateScoreboard(int Score, QString Username, bool BServer)
{
    ui->Scoreboard->setSortingEnabled(false);
    int NextRow = ui->Scoreboard->rowCount();
    ui->Scoreboard->insertRow(NextRow);
    QTableWidgetItem* IdItem = new QTableWidgetItem(Username);
    QTableWidgetItem* ScoreItem = new QTableWidgetItem(QString::number(Score));
    QTableWidgetItem* ReadyItem = new QTableWidgetItem(tr(""));
    ui->Scoreboard->setItem(NextRow,0, IdItem);
    ui->Scoreboard->setItem(NextRow,1, ScoreItem);
    ui->Scoreboard->setItem(NextRow, 2, ReadyItem);
    if(BServer)
        ui->Scoreboard->item(NextRow,2)->setBackground(QColor(0,255,0));
    ui->Scoreboard->setSortingEnabled(true);
}

void EndScreen::MultiPlayerSetUp(int Score, bool BClient, QString Username)
{
    ui->Scoreboard->setRowCount(0);
    IsClient = BClient;
    ClientUsername = Username;
    this->ui->stackedWidget->setCurrentIndex(1);
    UpdateScoreboard(Score, Username, !BClient);
}

EndScreen::~EndScreen()
{
    delete ui;
}



