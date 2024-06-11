#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <unordered_map>
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

void saveScore(const string &playerName, bool success, int attempts, const string &secretWord)
{
    ofstream file(SCORES_FILE, ios::binary | ios::app);
    if (!file.is_open())
    {
        cout << "Nije moguće otvoriti datoteku s rezultatima: " << SCORES_FILE << endl;
        return;
    }

    int nameLength = playerName.length();
    file.write(reinterpret_cast<const char *>(&nameLength), sizeof(nameLength));
    file.write(playerName.c_str(), nameLength);

    file.write(reinterpret_cast<const char *>(&success), sizeof(success));
    file.write(reinterpret_cast<const char *>(&attempts), sizeof(attempts));

    int wordLength = secretWord.length();
    file.write(reinterpret_cast<const char *>(&wordLength), sizeof(wordLength));
    file.write(secretWord.c_str(), wordLength);

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
    cout << "Najbolji rezultati:" << endl;
    while (file.peek() != EOF)
    {
        int nameLength;
        file.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
        string playerName(nameLength, ' ');
        file.read(&playerName[0], nameLength);

        bool success;
        file.read(reinterpret_cast<char *>(&success), sizeof(success));

        int attempts;
        file.read(reinterpret_cast<char *>(&attempts), sizeof(attempts));

        int wordLength;
        file.read(reinterpret_cast<char *>(&wordLength), sizeof(wordLength));
        string secretWord(wordLength, ' ');
        file.read(&secretWord[0], wordLength);

        cout << playerName << ": " << (success ? "✔️" : "❌") << ", "
             << (success ? to_string(7 - attempts) : "-") << ", " << secretWord << endl;
    }
    file.close();
}

void displayBoard(const vector<string> &guesses, const string &secret, int wordLength)
{
    cout << "---------------------------------------------" << endl;

    for (const string &guess : guesses)
    {
        unordered_map<char, int> letterCount;
        for (char c : secret)
        {
            letterCount[c]++;
        }

        cout << "|";
        for (size_t i = 0; i < guess.size(); ++i)
        {
            if (guess[i] == secret[i])
            {
                cout << "\033[32m  " << guess[i] << "  \033[0m|";
                letterCount[guess[i]]--;
            }
            else if (letterCount[guess[i]] > 0 && secret.find(guess[i]) == secret.find(guess[i])) //Provjerimo je li trenutni znak prvo pojavljivanje istog znaka u tajnoj riječi
            {
                cout << "\033[33m  " << guess[i] << "  \033[0m|"; //Slovo je žutom bojom
                letterCount[guess[i]]--;
            }
            else
            {
                cout << "\033[30m  " << guess[i] << "  \033[0m|"; //Slovo je crnom bojom
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

void playGame(const vector<string> &words, int wordLength, const string &playerName)
{
    string secret = getRandomWord(words);
    int attempts = 6;
    vector<string> guesses;

    while (attempts > 0)
    {
        (void)system("clear");
        cout << "Broj pokušaja: " << attempts << endl;
        displayBoard(guesses, secret, wordLength);
        string guess;
        cout << "Unesite svoju pretpostavku: ";
        cin >> guess;
        if (guess.size() != secret.size())
        {
            cout << "Nezadovoljavajuća duljina pretpostavke." << endl;
            cout << "Pokušajte ponovno pritiskom na Enter" << endl;
            cin.ignore();
            cin.get();
            continue;
        }
        guesses.push_back(guess);
        if (guess == secret)
        {
            (void)system("clear");
            displayBoard(guesses, secret, wordLength);
            cout << "Bravo! Pogodio si riječ." << endl;
            saveScore(playerName, true, attempts, secret);
            cout << endl;
            cout << "Svoj uspjeh možete vidjeti na pregledu najboljih rezultata" << endl
                 << endl
                 << endl;
            cout << "Pritisnite Enter za povratak u izbornik...";
            cin.ignore();
            cin.get();
            return;
        }
        attempts--;
    }
    (void)system("clear");
    displayBoard(guesses, secret, wordLength);
    cout << "Kraj igre! Riječ je bila: " << secret << endl;
    saveScore(playerName, false, 0, secret);
    cout << endl;
    cout << "Svoj uspjeh možete vidjeti na pregledu najboljih rezultata" << endl
         << endl
         << endl;
    cout << "Pritisnite Enter za povratak u izbornik...";
    cin.ignore();
    cin.get();
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
    cout << "2. Modovi" << endl;
    cout << "3. Pravila igre" << endl;
    cout << "4. Pregled najboljih rezultata" << endl;
    cout << "5. Izlaz" << endl;
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

void displayRules()
{
    cout << "Pravila igre Wordle:" << endl;
    cout << "1. Igra započinje odabirom nasumične riječi." << endl;
    cout << "2. Igrač ima 6 pokušaja da pogodi riječ." << endl;
    cout << "3. Svaka pretpostavka mora biti riječ sa istim brojem slova kao tajna riječ." << endl;
    cout << "4. Nakon svake pretpostavke, slova će biti obojena:" << endl;
    cout << "   - Zeleno: slovo je na ispravnom mjestu." << endl;
    cout << "   - Žuto: slovo se nalazi u riječi, ali na pogrešnom mjestu." << endl;
    cout << "   - Crno: slovo se ne nalazi u riječi." << endl;
    cout << "5. Cilj igre je pogoditi riječ u što manje pokušaja." << endl;
    cout << endl;
}

void waitForEnter()
{
    cout << "\n\nPritisnite Enter za povratak u izbornik...";
    cin.ignore();
    cin.get();
}

int main()
{
    int choice;
    int language = 2;
    int mode = 2;
    vector<string> words;

    do
    {
        (void)system("clear");
        displayTitle();
        displayMenu();
        cout << "Unesi svoj izbor: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            string playerName;
            cout << "Unesite svoje ime: ";
            cin >> playerName;
            (void)system("clear");
            cout << "UPOZORENJE: \n"
                 << endl;
            cout << "1. Unesene riječi moraju uvijek sadržavati točno " << mode + 3 << " slova." << endl;
            cout << "2. Unesene riječi moraju biti unesene sa malim slovima." << endl;
            cout << "\nSretno i zabavi se" << endl;
            cout << "\n\nPritisnite Enter za početak igre...";
            cin.ignore();
            cin.get();
            setWordsAndScoresFile(language, mode);
            words.clear();
            loadWords(words);
            if (words.empty())
            {
                cout << "Nema učitanih riječi. Izlaženje." << endl;
                return 1;
            }
            playGame(words, mode + 3, playerName);
            break;
        }
        case 2:
            (void)system("clear");
            displayOptions();
            cout << "Unesi svoj izbor: ";
            cin >> language;
            displayGameModeOptions();
            cout << "Unesi svoj izbor: ";
            cin >> mode;
            break;
        case 3:
            (void)system("clear");
            displayRules();
            waitForEnter();
            break;
        case 4:
            (void)system("clear");
            setWordsAndScoresFile(language, mode);
            displayScores();
            waitForEnter();
            break;
        case 5:
            cout << "Izlaženje..." << endl;
            break;
        default:
            cout << "Nevažeći izbor. Pokušaj ponovo." << endl;
        }
    } while (choice != 5);

    return 0;
}