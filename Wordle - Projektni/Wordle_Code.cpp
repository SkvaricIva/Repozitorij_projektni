#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;
string SCORES_FILE = "en_5_scores.dat";
string WORDS_FILE = "en_5.txt";
void loadWords(vector<string> &words)
{
    ifstream infile(WORDS_FILE);
    if (!infile.is_open())
    {
        cout << "Nije moguće otvoriti datoteku riječi: " << WORDS_FILE << endl;
        return;
    }
    string word;
    while (infile >> word)
    {
        words.push_back(word);
    }
    infile.close();
}
string getRandomWord(const vector<string> &words)
{
    srand(static_cast<unsigned int>(time(0)));
    int index = rand() % words.size();
    return words[index];
}
void saveScore(int score)
{
    ofstream file(SCORES_FILE, ios::binary | ios::app);
    if (!file.is_open())
    {
        cout << "Nije moguće otvoriti datoteku s rezultatima: " << SCORES_FILE << endl;
        return;
    }
    file.write(reinterpret_cast<const char *>(&score), sizeof(score));
    file.close();
}
void displayScores()
{
    ifstream file(SCORES_FILE, ios::binary);
    if (!file.is_open())
    {
        cout << "Nije moguće otvoriti datoteku s rezultatima: " << SCORES_FILE << endl;
        return;
    }
    int score;
    cout << "Najbolji rezultati:" << endl;
    while (file.read(reinterpret_cast<char *>(&score), sizeof(score)))
    {
        cout << score << endl;
    }
    file.close();
}
void displayBoard(const vector<string> &guesses, const string &secret, int wordLength)
{
    cout << "---------------------------------------------" << endl;
    for (const string &guess : guesses)
    {
        cout << "|";
        for (size_t i = 0; i < guess.size(); ++i)
        {
            if (guess[i] == secret[i])
            {
                cout << "\033[32m  " << guess[i] << "  \033[0m|";
            }
            else if (secret.find(guess[i]) != string::npos)
            {
                cout << "\033[33m  " << guess[i] << "  \033[0m|";
            }
            else
            {
                cout << "\033[30m  " << guess[i] << "  \033[0m|";
            }
        }
        cout << endl;
        cout << "---------------------------------------------" << endl;
    }
    for (int i = guesses.size(); i < 6; ++i)
    { 
        cout << "|";
        for (int j = 0; j < wordLength; ++j)
        {
            cout << "     |";
        }
        cout << endl;
        cout << "---------------------------------------------" << endl;
    }
}
void playGame(const vector<string> &words, int wordLength)
{
    string secret = getRandomWord(words);
    int attempts = 6;
    vector<string> guesses;
    while (attempts > 0)
    {
        string guess;
        cout << "Unesite svoju pretpostavku: ";
        cin >> guess;
        if (guess.size() != secret.size())
        {
            cout << "Nevažeća duljina pretpostavke. Pokušaj ponovo." << endl;
            continue;
        }
        guesses.push_back(guess);
        displayBoard(guesses, secret, wordLength);
        if (guess == secret)
        {
            cout << "Bravo! Pogodio si riječ." << endl;
            saveScore(attempts);
            return;
        }
        attempts--;
        cout << "Preostali pokušaji: " << attempts << endl;
    }
    cout << "Kraj igre! Riječ je bila: " << secret << endl;
}
void displayTitle()
{
    cout << " _    _               _  _      " << endl;
    cout << "| |  | |             | || |     " << endl;
    cout << "| |  | | ___  _ __ __| || | ___ " << endl;
    cout << "| |/\\| |/ _ \\| '__/ _` || |/ _ \\" << endl;
    cout << "\\  /\\  / (_) | | | (_| || |  __/" << endl;
    cout << " \\/  \\/ \\___/|_|  \\__,_||_|\\___|" << endl;
    cout << "====================================" << endl;
    cout << endl;
}
void displayMenu()
{
    cout << "1. Započni igru" << endl;
    cout << "2. Opcije" << endl;
    cout << "3. Pregled najboljih rezultata" << endl;
    cout << "4. Izlaz" << endl;
}
void displayOptions()
{
    cout << "Odaberi jezik:" << endl;
    cout << "1. Hrvatski" << endl;
    cout << "2. Engleski" << endl;
}
void displayGameModeOptions()
{
    cout << "Odaberi duljinu riječi:" << endl;
    cout << "1. 4-slova" << endl;
    cout << "2. 5-slova" << endl;
    cout << "3. 6-slova" << endl;
    cout << "4. 7-slova" << endl;
}
void setWordsAndScoresFile(int language, int mode)
{
    string langPrefix = (language == 1) ? "hr_" : "en_";
    string modeSuffix = to_string(mode + 3);
    WORDS_FILE = langPrefix + modeSuffix + ".txt";
    SCORES_FILE = langPrefix + modeSuffix + "_scores.dat";
}
int main()
{
    int choice;
    int language = 2;
    int mode = 2;
    vector<string> words;
    do
    {
        displayTitle();
        displayMenu();
        cout << "Unesi svoj izbor: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            setWordsAndScoresFile(language, mode);
            words.clear();
            loadWords(words);
            if (words.empty())
            {
                cout << "Nema učitanih riječi. Izlaženje." << endl;
                return 1;
            }
            playGame(words, mode + 3);
            break;
        case 2:
            displayOptions();
            cout << "Unesi svoj izbor: ";
            cin >> language;
            displayGameModeOptions();
            cout << "Unesi svoj izbor: ";
            cin >> mode;
            break;
        case 3:
            setWordsAndScoresFile(language, mode);
            displayScores();
            break;
        case 4:
            cout << "Izlaženje..." << endl;
            break;
        default:
            cout << "Nevažeći izbor. Pokušaj ponovo." << endl;
        }
    } while (choice != 4);
    return 0;
}