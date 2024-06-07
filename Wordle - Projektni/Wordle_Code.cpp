#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;
const string WORDS_FILE = "words.txt";
const string SCORES_FILE = "scores.dat";
void loadWords(vector<string> &words)
{
    ifstream file(WORDS_FILE);
    if (!file.is_open())
    {
        cout << "Nije moguće otvoriti datoteku riječi." << endl;
        return;
    }
    string word;
    while (file >> word)
    {
        words.push_back(word);
    }
    file.close();
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
        cout << "Nije moguće otvoriti datoteku s rezultatima." << endl;
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
        cout << "Nije moguće otvoriti datoteku s rezultatima." << endl;
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
void playGame(const vector<string> &words)
{
    string secret = getRandomWord(words);
    int attempts = 6;
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
        if (guess == secret)
        {
            cout << "Bravo! Pogodio si riječ." << endl;
            saveScore(attempts);
            return;
        }
        for (size_t i = 0; i < guess.size(); ++i)
        {
            if (guess[i] == secret[i])
            {
                cout << "🟩";
            }
            else if (secret.find(guess[i]) != string::npos)
            {
                cout << "🟨";
            }
            else
            {
                cout << "⬛";
            }
        }
        cout << endl;
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
    cout << "Riječi moraju počinjati s malim slovom." << endl;
    cout << endl;
}
void displayMenu()
{
    cout << "1. Započni igru" << endl;
    cout << "2. Pregled najboljih rezultata" << endl;
    cout << "3. Izlaz" << endl;
}
int main()
{
    vector<string> words;
    loadWords(words);
    if (words.empty())
    {
        cout << "Nema učitanih riječi. Izlaženje." << endl;
        return 1;
    }
    int choice;
    do
    {
        displayTitle();
        displayMenu();
        cout << "Unesi svoj izbor: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            playGame(words);
            break;
        case 2:
            displayScores();
            break;
        case 3:
            cout << "Izlaženje..." << endl;
            break;
        default:
            cout << "Nevažeći izbor. Pokušaj ponovo." << endl;
        }
    } while (choice != 3);
    return 0;
}