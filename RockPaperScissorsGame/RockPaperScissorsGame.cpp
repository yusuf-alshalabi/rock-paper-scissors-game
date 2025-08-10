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

// Generates a random number between two values (inclusive)
int randomNumber(int from, int to) {
    int random = rand() % (to - from + 1) + from;
    return random;
}

// Checks if the player lost based on their and the computer's choices
bool didPlayer1lose(enChoices player1Choice, enChoices computerChoice) {
    return ((player1Choice == enChoices::stone && computerChoice == enChoices::paper) ||
        (player1Choice == enChoices::paper && computerChoice == enChoices::scissors) ||
        (player1Choice == enChoices::scissors && computerChoice == enChoices::stone));
}

// Determines the winner of the round (player, computer, or draw)
enWin whoWins(enChoices player1Choice, enChoices computerChoice) {
    if (player1Choice == computerChoice)
        return enWin::draw;
    return (didPlayer1lose(player1Choice, computerChoice)) ? enWin::computer : enWin::player;
}

// Reads how many rounds the player wants to play (1–10)
short readHowManyRounds() {
    short rounds = 0;
    cout << "How many Rounds [1 to 10]? ";
    cin >> rounds;
    while (cin.fail() || rounds < 1 || rounds > 10) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 10.\n";
            cin >> rounds;
    }
    return rounds;
}

// Reads and validates the player's choice (stone, paper, or scissors)
enChoices readPlayerChoice() {
    short choice;
    cout << "Your Choice : [1]:Stone, [2]:Paper, [3]:Scissors ? ";
    cin >> choice;
    while (cin.fail() || choice < 1 || choice > 3) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 1, 2, or 3.\n";
            cin >> choice;
    }
    return (enChoices)choice;
}

// Converts choice enum to readable string
string getChoicesString(enChoices choices) {
    switch (choices) {
    case stone: return "Stone";
    case paper: return "Paper";
    case scissors: return "Scissors";
    default: return "Unknown";
    }
}

// Converts winner enum to readable name
string getWinnerName(enWin whoWin) {
    switch (whoWin) {
    case computer: return "Computer";
    case player: return "Player";
    default: return "No Winner";
    }
}

// Applies a visual and sound effect based on the winner
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

// Determines final winner based on win counts
enWin finalWinner(int player, int computer) {
    if (player == computer)
        return enWin::draw;
    if (player > computer)
        return enWin::player;
    return enWin::computer;
}

// Increases the corresponding win/draw counter
void addWinTime(strRoundInfo resultRound, GameResult& result) {
    if (resultRound.Winner == enWin::computer)
        result.ComputerWinTimes++;
    else if (resultRound.Winner == enWin::player)
        result.Player1WinTimes++;
    else
        result.DrawTimes++;
}

// Returns a string of tabs for formatting
string tabs(int number) {
    string tap = "";
    for (int i = 1; i <= number; i++) {
        tap += "\t";
    }
    return tap;
}

// Displays final game results summary
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

// Prints the details of one round
void printResultRound(strRoundInfo resultRound) {
    cout << "\n\n=================== Round [" << resultRound.RoundNumber << "] ===================\n";
    cout << tabs(1) << "Player1 Choice : " << getChoicesString(resultRound.Player1Choice) << ".\n";
    cout << tabs(1) << "Computer Choice : " << getChoicesString(resultRound.ComputerChoice) << ".\n";
    cout << tabs(1) << "Round Winner : [" << resultRound.WinnerName << "].\n";
    cout << "=================================================\n" << endl;
}

// Generates a random computer choice
enChoices getComputerChoice() {
    return (enChoices)randomNumber(1, 3);
}

// Creates and returns round info from choices and round number
strRoundInfo createRoundInfo(enChoices player1Choice, enChoices player2Choice, short roundNumber) {
    strRoundInfo round;
    round.RoundNumber = roundNumber;
    round.Player1Choice = player1Choice;
    round.ComputerChoice = player2Choice;
    round.Winner = whoWins(round.Player1Choice, round.ComputerChoice);
    round.WinnerName = getWinnerName(round.Winner);
    return round;
}

// Plays a single round and returns the result
strRoundInfo playRound(short round) {
    cout << tabs(2) << "Round [" << round << "] begins :\n";
    enChoices player1Choice = readPlayerChoice();
    enChoices computerChoices = getComputerChoice();
    return createRoundInfo(player1Choice, computerChoices, round);
}

// Runs all rounds of the game and returns the final result
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
    result.GameRounds = howManyRound;
    return result;
}

// Asks user if they want to play again
bool playAgain() {
    char again;
    while (true) {
        cout << "\n\nDo you want to play again (Y/N) ? \n\n\n";
        cin >> again;
        if (again == 'Y' || again == 'y')
            return true;
        else if (again == 'N' || again == 'n')
            return false;
    }
}

// Resets the console UI color and clears the screen
void resetUI() {
    system("color 07");
    system("cls");
}

// Starts the full game flow
void startGame() {
    do {
        resetUI();
        short rounds = readHowManyRounds();
        GameResult resultFinal = RunGameLoop(rounds);
        printFinalResult(resultFinal);
    } while (playAgain());
}

int main() {
    srand((unsigned)time(NULL));
    startGame();
    return 0;
}
