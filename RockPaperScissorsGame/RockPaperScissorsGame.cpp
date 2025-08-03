#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>


using namespace std;

enum enChoices { stone = 1, paper = 2, scissors = 3 };

enum enWin { player = 1, computer = 2, draw = 3 };

struct strRoundInfo
{
    short RoundNumber = 0;
    enChoices Player1Choice;
    enChoices ComputerChoice;
    enWin Winner;
    string WinnerName;
};

struct GameResult
{
    short GameRounds = 0;
    short Player1WinTimes = 0;
    short ComputerWinTimes = 0;
    short DrawTimes = 0;
    enWin GameWinner;
    string WinnerName = "";
};

int randomNumber(int from, int to) {
    int random = rand() % (to - from + 1) + from;
    return random;
}

bool didPlayer1lose(enChoices player1Choice, enChoices computerChoice) {
    return((player1Choice == enChoices::stone && computerChoice == enChoices::paper) ||
        (player1Choice == enChoices::paper && computerChoice == enChoices::scissors) ||
        (player1Choice == enChoices::scissors && computerChoice == enChoices::stone)
        );
}

enWin whoWins(enChoices player1Choice, enChoices computerChoice)
{

    if (player1Choice == computerChoice)
        return enWin::draw;

    return (didPlayer1lose(player1Choice, computerChoice)) ? enWin::computer : enWin::player;

}

short readHowManyRounds() {
    short rounds = 0;
    do {
        cout << "How many Rounds 1 to 10 ?" << endl;
        cin >> rounds;
    } while (rounds < 1 || rounds>10);
    return rounds;
}

enChoices readPlayerChoice() {
    short choice = 0;
    do {
        cout << "Your Choice : [1]:Stone, [2]:Paper, [3]:Scissors ?";
        cin >> choice;
    } while (choice > 3 || choice < 1);
    return (enChoices)choice;
}

string getChoicesString(enChoices choices) {
    switch (choices) {
    case stone: return "Stone";
    case paper: return "Paper";
    case scissors: return "Scissors";
    default: return "Unknown";
    }
}

string getWinnerName(enWin whoWin) {
    switch (whoWin) {
    case computer:return "Computer";
    case player:return "Player";
    default: return "No Winner";
    }
}

void applyEffect(enWin win) {
    if (win == enWin::computer) {
        system("color 4F");
        cout << "\a";
    }

    else if (win == enWin::player)
        system("color 20");
    else
        system("color E0");
}

enWin finalWinner(int player, int computer) {
    if (player == computer)
        return enWin::draw;
    if (player > computer)
        return enWin::player;

    return enWin::computer;
}

void addWinTime(strRoundInfo resultRound, GameResult& result) {

    if (resultRound.Winner == enWin::computer)
        result.ComputerWinTimes++;
    else if (resultRound.Winner == enWin::player)
        result.Player1WinTimes++;
    else
        result.DrawTimes++;
}

string tabs(int number) {
    string tap = "";
    for (int i = 1; i <= number; i++) {
        tap += "\t";
    }
    return tap;
}

void printFinalResult(GameResult result) {
    cout << "\n";
    cout << tabs(4) << "====================================================\n";
    cout << tabs(4) << "                    +++Game Over+++\n";
    cout << tabs(4) << "====================================================\n\n";
    cout << tabs(4) << "================= [Game Results] ==================\n";
    cout << tabs(4) << "Game Rounds        : " << result.GameRounds << ".\n";
    cout << tabs(4) << "Player1  won times : " << result.Player1WinTimes << ".\n";
    cout << tabs(4) << "Computer won times : " << result.ComputerWinTimes << ".\n";
    cout << tabs(4) << "Draw times         : " << result.DrawTimes << ".\n";
    cout << tabs(4) << "Final Winner       : " << result.WinnerName << ".\n";
    cout << tabs(4) << "====================================================" << endl;
    applyEffect(result.GameWinner);
}

void printResultRound(strRoundInfo resultRound) {
    cout << "\n\n=================== Round [" << resultRound.RoundNumber << "] ===================\n";
    cout << tabs(1) << "Player1 Choice : " << getChoicesString(resultRound.Player1Choice) << ".\n";
    cout << tabs(1) << "Computer Choice : " << getChoicesString(resultRound.ComputerChoice) << ".\n";
    cout << tabs(1) << "Round Winner : [" << resultRound.WinnerName << "].\n";
    cout << "=================================================\n" << endl;
}

enChoices getComputerChoice() {
    return (enChoices)randomNumber(1, 3);
}

strRoundInfo createRoundInfo(enChoices player1Choice, enChoices player2Choice, short roundNumber) {
    strRoundInfo round;
    round.RoundNumber = roundNumber;
    round.Player1Choice = player1Choice;
    round.ComputerChoice = player2Choice;
    round.Winner = whoWins(round.Player1Choice, round.ComputerChoice);
    round.WinnerName = getWinnerName(round.Winner);
    return round;
}

strRoundInfo playRound(short round) {
    cout << tabs(2) << "Round [" << round << "] begins :\n";
    enChoices player1Choice = readPlayerChoice();
    enChoices computerChoices = getComputerChoice();
    return createRoundInfo(player1Choice, computerChoices, round);
}

GameResult RunGameLoop(short howManyRound) {
    GameResult result;
    for (int round = 1; round <= howManyRound; round++) {
        strRoundInfo Info = playRound(round);
        printResultRound(Info);
        applyEffect(Info.Winner);
        addWinTime(Info, result);
    }
    result.GameWinner = finalWinner(result.Player1WinTimes, result.ComputerWinTimes);
    result.WinnerName = getWinnerName(result.GameWinner);
    return result;
}

bool playAgain() {
    char again;
    while (true)
    {
        cout << "\n\nDo you want to play again (Y/N) ? \n\n\n";
        cin >> again;
        if (again == 'Y' || again == 'y')
            return true;
        else if (again == 'N' || again == 'n')
            return false;
    }
}

void resetUI() {
    system("color 07");
    system("cls");
}

void startGame() {
    do
    {
        resetUI();
        short rounds = readHowManyRounds();
        GameResult resultFinal = RunGameLoop(rounds);
        printFinalResult(resultFinal);
    } while (playAgain());
}

int main()
{
    srand((unsigned)time(NULL));
    startGame();

    return 0;
}
