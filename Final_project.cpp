#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <Windows.h>

using namespace std;

//made by Mohammad Gholami (4041226110)  &  Mohammad Erfan Barani (4041226128)

const int WIDTH = 68; 
const int HEIGHT = 18;
const int MAX_SHOTS = 10;
const double G = 0.5;

struct Bird {
    double x, y;
    double vx, vy;
    bool active;
};

struct Level {
    char map[HEIGHT][WIDTH];
    int playerX, playerY;
    int enemies;
};

int scores[3] = {0, 0, 0};

void loadScores() {
    ifstream fin("scores.txt");
    if (!fin) {
        scores[0] = scores[1] = scores[2] = 0;
        return;
    }
    fin >> scores[0] >> scores[1] >> scores[2];
    fin.close();
}

void saveScores() {
    ofstream fout("scores.txt");
    fout << scores[0] << " " << scores[1] << " " << scores[2];
    fout.close();
}

void resetScores() {
    scores[0] = scores[1] = scores[2] = 0;
    saveScores();
}

void loadEasy(Level &lvl) {
    const char* raw[HEIGHT] = {
        "###################################################################",
        "#                                                                 #",
        "#                                                                 #",
        "#                                                    E            #",
        "#                                                    #            #",
        "#                                                  E #            #",
        "#                                                  ###            #",
        "#                                                    #            #",
        "#   Y                                             E  #            #",
        "#####                                            ##################",
        "#                                                                 #",
        "#                                                                 #",
        "#                                                                 #",
        "#                                                                 #",
        "#                                                                 #",
        "#                                                                 #",
        "#                                                                 #",
        "###################################################################"
    };

    lvl.enemies = 0;

    for (int i = 0; i < HEIGHT; i++) {
        strcpy(lvl.map[i], raw[i]);
        for (int j = 0; j < WIDTH - 1; j++) {
            if (lvl.map[i][j] == 'Y') {
                lvl.playerX = j;
                lvl.playerY = i;
            }
            if (lvl.map[i][j] == 'E') lvl.enemies++;
        }
    }
}

void loadMedium(Level &lvl) {
    const char* raw[HEIGHT] = {
        "###################################################################",
        "#                                                                 #",
        "#                                                                 #",
        "#                                               E                 #",
        "#                                               ###################",
        "#                                               #                 #",
        "#                                               #                 #",
        "#                                                                 #",
        "#                                                        E        #",
        "#                                                       ###       #",
        "#                                                        #        #",
        "#                                               #        #        #",
        "#   Y                                           #        #        #",
        "#####                                           ###################",
        "#                                                                 #",
        "#                                                                 #",
        "#                                                 E               #",
        "###################################################################"
    };

    lvl.enemies = 0;

    for (int i = 0; i < HEIGHT; i++) {
        strcpy(lvl.map[i], raw[i]);
        for (int j = 0; j < WIDTH - 1; j++) {
            if (lvl.map[i][j] == 'Y') {
                lvl.playerX = j;
                lvl.playerY = i;
            }
            if (lvl.map[i][j] == 'E') lvl.enemies++;
        }
    }
}

void loadHard(Level &lvl) {
    const char* raw[HEIGHT] = {
        "###################################################################",
        "#                                                                 #",
        "#                                                                 #",
        "#                                                                 #",
        "#   Y                                                             #",
        "#####                                                             #",
        "#                                                                 #",
        "#                                                                 #",
        "#                                                                 #",
        "#                                                    ##############",
        "#                                                            ######",
        "#                                                              ####",
        "#                                                                ##",
        "#                         E                                       #",
        "#                         #                     #                 #",
        "#                         #######         #######                 #",
        "#                         #     #    E    #     #         E       #",
        "###################################################################"
    };

    lvl.enemies = 0;

    for (int i = 0; i < HEIGHT; i++) {
        strcpy(lvl.map[i], raw[i]);
        for (int j = 0; j < WIDTH - 1; j++) {
            if (lvl.map[i][j] == 'Y') {
                lvl.playerX = j;
                lvl.playerY = i;
            }
            if (lvl.map[i][j] == 'E') lvl.enemies++;
        }
    }
}

void printMap(Level lvl, Bird p) {
    char temp[HEIGHT][WIDTH];

    for (int i = 0; i < HEIGHT; i++)
        strcpy(temp[i], lvl.map[i]);

    if (p.active) {
        int px = (int)(p.x + 0.5);
        int py = (int)(p.y + 0.5);

        if (py >= 0 && py < HEIGHT && px >= 0 && px < WIDTH - 1) {
            if (temp[py][px] == ' ') temp[py][px] = '*';
        }
    }

    for (int i = 0; i < HEIGHT; i++)
        cout << temp[i] << endl;
}

void pressEnterTick() {
    cout << "Press ENTER for next tick... ";
    cin.ignore();
}

bool simulateShot(Level &lvl, int degree, int speed, int &score) {
    Bird p;
    p.active = true;
    p.x = lvl.playerX + 1;
    p.y = lvl.playerY;

    double angleRad = degree * (3.14159265 / 180.0);
    p.vx = speed * cos(angleRad);
    p.vy = -speed * sin(angleRad);

    while (p.active){

        system("cls");

        cout << "*** Level Game ***" << endl;
        cout << "Enemies Left: " << lvl.enemies << " | Score: " << score << endl;
        cout << "----------------------------------------" << endl;

        printMap(lvl, p);

        cout << endl;

        cin.ignore();

        int steps = (int)ceil(max(fabs(p.vx), fabs(p.vy)) * 2);
        if (steps < 1) steps = 1;

            double dx = p.vx / steps;
            double dy = p.vy / steps;

        for (int i = 0; i < steps; i++) {
            p.x += dx;
            p.y += dy;

            int px = (int)(p.x + 0.5);
            int py = (int)(p.y + 0.5);

            if (px < 0 || px >= WIDTH - 1 || py < 0 || py >= HEIGHT) {
                p.active = false;
                break;
            }

            if (lvl.map[py][px] == '#') {
                p.active = false;
                break;
            }

            if (lvl.map[py][px] == 'E') {
                lvl.map[py][px] = ' ';
                lvl.enemies--;
                score++;
                p.active = false;

                system("cls");

                cout << "*** Level Game ***" << endl;
                cout << "Enemies Left: " << lvl.enemies << " | Score: " << score << endl;
                cout << "----------------------------------------" << endl;
                printMap(lvl, p);
                cout << "----------------------------------------" << endl;
                cout <<"-- Enemy destroyed! Press ENTER... " << endl;
                cin.ignore();
                cin.get();
            
            break;
            }
        }
        p.vy += G;
    }
    
    return true;
}

void playLevel(int levelIndex) {
    Level lvl;
    int score = 0;
    int shots = MAX_SHOTS;

    if (levelIndex == 0) loadEasy(lvl);
    else if (levelIndex == 1) loadMedium(lvl);
    else loadHard(lvl);

    while (shots > 0 && lvl.enemies > 0) {
        system("cls");

        cout << "*** Level: ";
        if (levelIndex == 0) cout << "Easy";
        else if (levelIndex == 1) cout << "Medium";
        else cout << "Hard";
        cout << " ***" << endl;

        cout << "Remaining Shots: " << shots
                << " | Enemies Left: " << lvl.enemies
                << " | Score: " << score << endl;

        cout << "--------------------------------------------------" << endl;
        printMap(lvl, {0,0,0,0,false});
        cout << "--------------------------------------------------" << endl;

        int degree;
        while (true) {
            cout << "Degree? (0-90) ";
            cin >> degree;

            if (cin.fail() || degree < 0 || degree > 90) {
            cin.clear();
            while (cin.get() != '\n');
            cout << "Invalid input! Enter a number between 0 and 90" << endl;
            } 
            else {
                break;
            }
        }

        int speed;
        while (true) {
            cout << "Launcher Power(Speed)? (1-10) ";
            cin >> speed;

            if (cin.fail() || speed < 1 || speed > 10) {
                cin.clear();
                while (cin.get() != '\n');
                cout << "Invalid input! Enter a number between 1 and 10" << endl;
            } 
            else {
                break;
            }
        }

        cin.ignore();

        simulateShot(lvl, degree, speed, score);

        shots--;
    }
    
    system("cls");

    if (lvl.enemies == 0) {
        cout << "Level Finished! " << endl;
        cout << "You Win! " << endl;
    } else {
        cout << "Level Failed! " << endl;
        cout << "Out of shots! " << endl;
    }

    cout << "Final Score: " << score << endl;

    if (score > scores[levelIndex])
        scores[levelIndex] = score;

    saveScores();

    cout << "Press ENTER to go back to menu... " << endl;
    cin.ignore();
}

void showHowToPlay() {
    system("cls");

    cout << "How to Play? ez" << endl;
    cout << "(Destroy all enemies. You have 10 shots per level | enemy (E) | Player (Y) | Bird (*) )" << endl;
    cout << "Press ENTER to return... " << endl;
    cin.ignore();
}

void showScoreHistory() {
    system("cls");

    cout << "Scores History:" << endl;
    cout << "------------------------" << endl;
    cout << "Easy Level   | " << scores[0] << endl;
    cout << "Medium Level | " << scores[1] << endl;
    cout << "Hard Level   | " << scores[2] << endl;
    cout << endl;

    cout << "1) Reset Scores" << endl;
    cout << "2) Back" << endl;
    cout << "Choose the Number... " << endl;

    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        system("cls");
        
        resetScores();
        cout << "Scores reset successfully! " << endl;
        cout << "Press ENTER to return... " << endl;
        cin.ignore();
    }
}


int levelSelectMenu() {
    system("cls");

    cout << "Select Level:" << endl;
    cout << "1) Easy" << endl;
    cout << "2) Medium" << endl;
    cout << "3) Hard" << endl;
    cout << "4) Back" << endl;
    cout << "Choose the Number... " << endl;

    int c;
    cin >> c;
    cin.ignore();
    return c;
}

int main() {
    loadScores();

    while (true) {
        system("cls");

        cout << "_______________________________________________________________________" << endl;
        cout << "\n                        Angry Birds 2D CONSOLE" << endl;
        cout << "_______________________________________________________________________" << endl;
        cout << "1) Start" << endl;
        cout << "2) How to Play?" << endl;
        cout << "3) Scores History" << endl;
        cout << "4) Exit"<< endl;
        cout << "Alert! (If this is your first time playing this game be sure to read the guide)" << endl;
        cout << "Choose the Number... " << endl;

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            int lvlChoice = levelSelectMenu();
            if (lvlChoice >= 1 && lvlChoice <= 3)
                playLevel(lvlChoice - 1);
        }
        else if (choice == 2) {
            showHowToPlay();
        }
        else if (choice == 3) {
            showScoreHistory();
        }
        else if (choice == 4) {
            cout << "Goodbye! " << endl;
            break;
        }
    }

    return 0;
}
