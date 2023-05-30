#include "mainmenu.h"
#include "ui_mainmenu.h"
#include <QIntValidator>
#include <QDebug>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    QValidator* RangeValidator = new QIntValidator(1, INT_MAX, this);
    QValidator* AmountValidator = new QIntValidator(1, 100, this);
    QWidget* MainScreenPage = ui->stackedWidget->widget(1);
    QWidget* GameSettingsPage = ui->stackedWidget->widget(0);
    ui->stackedWidget->setCurrentWidget(MainScreenPage);
    ui->LowestInput->setValidator(RangeValidator);
    ui->HighestInput->setValidator(RangeValidator);
    ui->AmountInput->setValidator(AmountValidator);
    ui->stackedWidget->addWidget(GamePage);
    ui->stackedWidget->addWidget(EndPage);
    ui->stackedWidget->addWidget(MultiPlayerPage);
    connect(this, &MainMenu::MultiChangeWidgetIndex, MultiPlayerPage, &MultiPlayerScreen::ChangeWidgetIndex);
    connect(ui->StartButton, &QPushButton::clicked, this, &MainMenu::StartButtonClicked);
    connect(GamePage, &GameScreen::GameOverSignal, this, &MainMenu::SetEndScreenWidget);
    connect(EndPage, &EndScreen::AgainButtonClicked, this, &MainMenu::StartButtonClicked);
    connect(EndPage, &EndScreen::MenuButtonClicked, this, [=](){this->ui->stackedWidget->setCurrentWidget(MainScreenPage);});
    connect(EndPage, &EndScreen::MultiMenuButtonClicked, MultiPlayerPage, &MultiPlayerScreen::DisconnectSocket);
    connect(EndPage, &EndScreen::MultiLobbyButtonClicked, this, &MainMenu::HandleReturnToLobby);
    connect(EndPage, &EndScreen::MultiPlayAgainClicked, this, [=](){HandleReturnToLobby(3);});
    connect(EndPage, &EndScreen::ClientStateChanged, MultiPlayerPage, &MultiPlayerScreen::PlayAgainButtonClicked);
    connect(EndPage, &EndScreen::ServerCheckStatuses, MultiPlayerPage, &MultiPlayerScreen::CheckAllUsersReady);
    connect(MultiPlayerPage, &MultiPlayerScreen::MenuButtonClickedSignal, this, [=](){this->ui->stackedWidget->setCurrentWidget(MainScreenPage);});
    connect(MultiPlayerPage, &MultiPlayerScreen::GameEnded, this, &MainMenu::SetMultiPlayerEndScreenWidget);
    connect(MultiPlayerPage, &MultiPlayerScreen::GameScoreUpdated, EndPage, &EndScreen::UpdateScoreboard);
    connect(MultiPlayerPage, &MultiPlayerScreen::UpdateScoreboardState, EndPage, &EndScreen::UpdateScoreboardState);
    connect(MultiPlayerPage, &MultiPlayerScreen::CanPlayAgain, this, [=](){HandleReturnToLobby(3);});
    connect(MultiPlayerPage, &MultiPlayerScreen::CanReturnToLobby, this, [=](int Index){HandleReturnToLobby(Index);});
    connect(MultiPlayerPage, &MultiPlayerScreen::NotAllPlayersReady, EndPage, &EndScreen::NotAllPlayersReady);
    connect(MultiPlayerPage, &MultiPlayerScreen::CheckAndChangeWidget, this, &MainMenu::CheckAndChangeWidget);
    connect(ui->SinglePlayerButton, &QPushButton::clicked, this, [=](){this->ui->stackedWidget->setCurrentWidget(GameSettingsPage);});
    connect(ui->MultiPlayerButton, &QPushButton::clicked, this, [=](){
        this->ui->stackedWidget->setCurrentWidget(MultiPlayerPage);
        emit MultiChangeWidgetIndex(0);});
    connect(ui->MainMenuButton, &QPushButton::clicked, this, [=](){this->ui->stackedWidget->setCurrentWidget(MainScreenPage);});
    connect(ui->SingleMenuButton, &QPushButton::clicked, this, [=](){this->ui->stackedWidget->setCurrentWidget(MainScreenPage);});
}

void MainMenu::CheckAndChangeWidget()
{
    if(ui->stackedWidget->currentWidget() != MultiPlayerPage)
    {
        ui->stackedWidget->setCurrentWidget(MultiPlayerPage);

    }
}

void MainMenu::StartButtonClicked()
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
    ui->stackedWidget->setCurrentWidget(GamePage);
    GamePage->StartGame(Lowest, Highest, Amount);
}

void MainMenu::SetEndScreenWidget(int Score)
{
    EndPage->SetScore(Score);
    ui->stackedWidget->setCurrentWidget(EndPage);
}

void MainMenu::SetMultiPlayerEndScreenWidget(int Score, bool BClient, QString Username)
{
    ui->stackedWidget->setCurrentWidget(EndPage);
    EndPage->MultiPlayerSetUp(Score, BClient, Username);
}


void MainMenu::HandleReturnToLobby(int Index)
{
    ui->stackedWidget->setCurrentWidget(MultiPlayerPage);
    emit MultiChangeWidgetIndex(Index);
}

MainMenu::~MainMenu()
{
    delete ui;
}



