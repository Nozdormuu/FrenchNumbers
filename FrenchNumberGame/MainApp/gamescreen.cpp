#include "gamescreen.h"
#include "ui_gamescreen.h"

GameScreen::GameScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameScreen)
{
    ui->setupUi(this);
    connect(ui->EnterButton, &QPushButton::clicked, this, &GameScreen::EnterButtonClicked);
    connect(ui->SkipButton, &QPushButton::clicked, this, [this](){CheckIfGameOver();});
    connect(ui->ClearButton, &QPushButton::clicked, this, [this](){ui->UserInput->setText("");});
}

void GameScreen::StartGame(uint Lowest, uint Highest, int Amount)
{
    game = new Game(this);
    game->StartGame(Lowest, Highest, Amount);
    ui->WordLabel->setText(game->GetCurrentNumber());
}

void GameScreen::EnterButtonClicked()
{
    QString UserAnswer = ui->UserInput->text().trimmed();
    game->CheckAnswer(UserAnswer);
    CheckIfGameOver();
}

void GameScreen::CheckIfGameOver()
{
    CurrentNumber = game->Increment();
    if(CurrentNumber.isEmpty())
    {
        emit GameOverSignal(game->Score);
        return;
    }
    ui->WordLabel->setText(CurrentNumber);
    ui->UserInput->setText("");
}

GameScreen::~GameScreen()
{
    delete ui;
}
