//Art
//To Do
//1. Create Readme. add game decription, gif examples, howto install...
//2. REFACTOR IT. New Architect. Need Classes, constructors, get, set...
//3. Optimize algo.
//4. test bugs, check break conditions
//5. Create Environment. (rewrite without dependencies)
//6. think about new interface. color it, beautify it. 
//7. Mb to Rewrite to Text User Interface or GUI
//#include<bits/stdc++.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <ctime>
#include <algorithm>
#include <Windows.h>

// #include <chrono>
// #include <thread>
using namespace std;
 
// const int INF = 1e9;
const int coordi[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
const int coordj[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
 
const char 
    EMPTY = '.', 
    LIVE  = '*';
 
const int CELL_H = 2;
const int CELL_W = 2;
const int MAX_HEIGHT = 20;
const int MAX_WIDTH = 50;
 
enum ConsoleColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
};
 
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 
inline void mySleep(clock_t sec) 
// clock_t is a like typedef unsigned int clock_t. Use clock_t instead of integer in this context
{
    // clock_t start_time = clock();
    // clock_t end_time = sec * 1000 + start_time;
    // while(clock() != end_time);
    // this_thread::sleep_for(chrono::milliseconds(sec*1000));
    Sleep(sec * 1000);
}
 
class Live {
private:
    string name = "Human";
    vector <vector <char> > beauty_map; 
    bool isBeautyModeON = false;
    bool isAssigned = false;
    
    //From file
    int saved_map_id = 1;
    int height, width;
    int min_k=0, max_t=8;
    vector <vector <char> > live_map, curr_map_state;

public:
    void init()
    {
        if (!isAssigned)
        {
            live_map.assign(height, vector <char> (width));
            curr_map_state.assign(height, vector <char> (width));
            beauty_map.assign(height*CELL_H, vector <char> (width*CELL_W));
            isAssigned = true;
        }
        else
        {
            live_map.clear();
            curr_map_state.clear();
            beauty_map.clear();
            live_map.resize(height, vector <char> (width));
            curr_map_state.resize(height, vector <char> (width));
            beauty_map.resize(height*CELL_H, vector <char> (width*CELL_W));
        }
    }
 
    void settings()
    {
        ifstream fin("settings.txt");
        fin >> saved_map_id;
        fin.close();
    }

    void update_settings()
    {
        ofstream fout("settings.txt");
        fout << saved_map_id;
        fout.close();
    }
 
    void create_random_map()
    {
        height = rand() % (MAX_HEIGHT-2)+2;
        width = rand() % (MAX_WIDTH-2)+2;
        max_t = rand() % 6 + 2;
        min_k = rand() % max_t;
        init();
        string s = ".*";
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                live_map[i][j] = s[rand() % 2];
                curr_map_state[i][j] = live_map[i][j];
            }
    }
 
    void shuffle()
    {
        int n = height;
        int m = width;
        for (int it = 0; it < n*m; it++)
        {
            int i = rand() % n;
            int j = rand() % m;
            int i1 = rand() % n;
            int j1 = rand() % m;
            swap(live_map[i][j], live_map[i1][j1]);
            swap(curr_map_state[i][j], curr_map_state[i1][j1]);
        }
    }

    int choose_number() {
        bool gotIt = false;
        int id;
        for (int i = 0; i < 7; i++) {
            printf("Choose map number from 1 to %d\n", saved_map_id);
            string sid;
            cin >> sid;
            stringstream ss(sid);
            if ((ss >> id).fail()) {
                puts("Can not understand your number :(");
            } else if (id < 1) {
                puts("your number is too small. Minimal number - 1");
            } else if (id > saved_map_id) {
                printf("your number is too big. Maximal number - %d\n", saved_map_id);
            } else {
                gotIt = true;
                break;
            }
            puts("Try again");
        }

        if (!gotIt) {
            puts("OK. I'll choose file randomly");
            id = rand() % saved_map_id+1; 
        }

        return id;
    }
 
    void read_map_from_file(bool isReaden)
    {  
        int id = choose_number();
        stringstream ss;
        ss << id;
        string file_name = "world" + ss.str() + ".txt";
        string file_path = "maps/";
        string file = file_path+file_name;
        ifstream fin(file);
        fin >> height >> width;
        init();
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                fin >> live_map[i][j];
                curr_map_state[i][j] = live_map[i][j];
            }
 
        fin >> min_k >> max_t;
        fin.close();
    }
 
    void save_map()
    {
        string file_path = "maps/";
        saved_map_id++;
        stringstream ss; 
        ss << saved_map_id;
        string file_name = "world" + ss.str() + ".txt";
        string file = file_path+file_name;
        ofstream fout(file);
        fout << height << " " << width << '\n';
        for (int i = 0; i < height; i++, fout << '\n')
            for (int j = 0; j < width; j++)
                fout << live_map[i][j];
 
        fout << min_k << " " << max_t;
        fout.close();
        update_settings();
    }
 
    int edit_map()
    {   
        int id;
        puts("Edit last seen map? yes/no");
        string ans;
        cin >> ans;
        if (ans == "yes" || ans == "YES" || ans == "Yes") {
            save_map();
            id = saved_map_id;
        } else {
            puts("You can choose from old maps one");
            id = choose_number();
        }

        stringstream ss; 
        ss << id;
        string file_path = "maps\\";
        string file_name = "world" + ss.str() + ".txt";
        string file = file_path + file_name;
        system(file.c_str());
        //edit with sublime
        //string cmd = "C:\\Program Files\\Sublime Text 3\\sublime_text.exe " + file;
        //system(cmd.c_str()); 
        system("pause");
        return id;
    }
 
    void start_view()
    {
        srand(int(time(NULL)));
        setlocale(0, "");
        //setlocale(LC_CTYPE, "rus");
        //system("Color XY");https://www.geeksforgeeks.org/how-to-print-colored-text-in-c/
        //system("color 67"); 
        //system("mode con cols=1024 lines=720");
        //system("mode con cols=100 lines=30");
        puts("Welcome to...\n");
        mySleep(1);
        puts("   #                                       ");
        puts("   #           #  #         #  ##########  ");
        puts("   #           #   #       #   ##          ");
        puts("   #           #    #     #    #######     ");
        puts("   #           #     #   #     ##          ");
        puts("   #           #      # #      ##          ");
        puts("   ##########  #       #       ##########  \n");
        mySleep(1);
        puts("Please type your name: ");
        cin >> name;
        mySleep(1);
        
        settings();
        string _switch = "on ";
        bool isReaden = false;
        string action_string;
        char action = '#';
        while (true)
        {
            if (action == '1')
            {
                create_random_map();
                printf("%d %d\n\n", height, width);
                // isAssigned = true;
                if (isBeautyModeON)
                    print_beauty_map();
                else
                    print_map();
            }
            else if (action == '2')
            {
                read_map_from_file(isReaden);
                printf("%d %d\n\n", height, width);
                isReaden = true;
                // isAssigned = true;
                if (isBeautyModeON)
                    print_beauty_map();
                else
                    print_map();
            }
            else if (action == '3')
            {
                cout << "Beauty mode is turned " << _switch << "\n\n";
                mySleep(2);
                if (!isBeautyModeON)
                {
                    isBeautyModeON = true;
                    _switch = "off";
                    if (isAssigned)
                        print_beauty_map();
                } else {
                    isBeautyModeON = false;
                    _switch = "on ";
                    if (isAssigned)
                        print_map();
                }
            } 
            else if (action == '4')
            {
                puts("Presentation mode is turned on\n");
                mySleep(2);
                offline_mode();
            }
            else if (action == '5')
            {
                save_map();
                cout << "The last map was saved in file: 'world" << saved_map_id << ".txt'\n";
                mySleep(2);
            } 
            else if (action == '6')
            {
                int id = edit_map();
                cout << "The map was edited and saved in file: 'world" << id << ".txt'\n";
                mySleep(2);
            }
            else if (action == '7')
            {
                cout << "OK " << name << " let's play!\n\n";
                //system("pause");
                curr_map_state = live_map;
                mySleep(2);
                start();
            }
            else if (action == 'q')
            {
                finish_view();
                mySleep(2);
                break;
            } else if (action != '#') {
                puts("Unknown command");
            }
 
            puts("\nGenerate new map       - press 1");
            puts("Read map from file     - press 2");
            cout << "Turn " << _switch << " 'Beauty Mode' - press 3" << "\n";
            puts("Presentation mode      - press 4");
            if (isAssigned) 
            {
                cout << "Save this map to file  - press 5  (world" << saved_map_id+1 << ".txt)\n";
                puts("Edit map               - press 6");
                puts("Start                  - press 7");
            }
            puts("Exit                   - press q");
            puts("Force Exit - Ctrl + C");
            
            cin >> action_string;
            action = action_string[0];
            puts("");
            mySleep(1);         
        }
        //cout.flush();
        //mySleep(2);
        //sleep(2);
        //delay(2);
        //char ch = getchar();
    }
 
    void start()
    { 
        // system("cls");//
        // print_map();//-1, -1);
        bool is_alive = true;
        while (is_alive)
        {
            is_alive = false;
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    //printf("%d %d\n", i, j); //logs
                    int neighbors = 0;
                    for (int t = 0; t < 8; t++)
                    {
                        int new_i = i + coordi[t];
                        int new_j = j + coordj[t];
                        if (0 <= new_i && new_i < height &&
                            0 <= new_j && new_j < width)
                            if (curr_map_state[new_i][new_j] == LIVE)
                                neighbors++;
                    }

                    if (curr_map_state[i][j] == EMPTY)
                    {
                        if (min_k < neighbors && neighbors < max_t)
                        {
                            curr_map_state[i][j] = LIVE;
                            is_alive = true;
                        }
                    }
                    else //if (curr_map_state[i][j] == LIVE) 
                    {
                        if (neighbors >= max_t)
                        {
                            curr_map_state[i][j] = EMPTY;
                            is_alive = true;
                        }
                    }
                }
            }

            if (isBeautyModeON)
                print_beauty_map();
            else {
                print_map();//i, j);
            }

            // system("cls");//
            puts("Press CTRL+C to stop");
            mySleep(3);
        }

        puts("THE END of this Live :)");
    }
 
    void print_map() //int i1, int j1
    {
        //mySleep(5);// was 5
        // printf("%d %d\n", height, width);
        for (int i = 0; i < height; i++, puts("")) 
        {
            SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
            // if (i == i1)
            //  cout << "--> ";
            // else
            //  cout << "    ";
            //mySleep(0.3);
 
            for (int j = 0; j < width; j++) 
            {
                if (curr_map_state[i][j] == LIVE)
                    SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Green));
                else
                    SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Red));
                putchar(curr_map_state[i][j]);
            }
            //mySleep(0.3);
        }
        
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
        printf("\nmin neighbors (k): %d\n", min_k);
        printf("max neighbors (d): %d\n\n", max_t);
    }
 
    void create_live(int i1, int j1)
    {
        vector <string> live(CELL_H);
        live[0] = "/\\";
        live[1] = "\\/";
 
        for (int i = 0; i < CELL_H; i++)
        {
            for (int j = 0; j < CELL_W; j++)
            {
                beauty_map[i1 * CELL_H + i][j1 * CELL_W + j] = live[i][j];
            }
        }
    }
 
    void create_empty(int i1, int j1)
    {
        vector <string> empty(CELL_H);
        empty[0] = "..";
        empty[1] = "..";
        for (int i = 0; i < CELL_H; i++)
        {
            for (int j = 0; j < CELL_W; j++)
            {
                beauty_map[i1 * CELL_H + i][j1 * CELL_W + j] = empty[i][j];
            }
        }
    }
 
    void create_beauty_map()
    {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                if (curr_map_state[i][j] == LIVE)
                    create_live(i, j);
                else //if (curr_map_state[i][j] == EMPTY)
                    create_empty(i, j);
    }
 
    void print_beauty_map()
    {
        create_beauty_map();
        for (int i = 0; i < height * CELL_H; i++, puts(""))
            for (int j = 0; j < width * CELL_W; j++) 
            {
                if (curr_map_state[i / CELL_H][j / CELL_W] == LIVE)
                    SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Green));
                else if (curr_map_state[i / CELL_H][j / CELL_W] == EMPTY)
                    SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Red));
                    
                putchar(beauty_map[i][j]);
            }
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
        
        printf("\nmin neighbors (k): %d\n", min_k);
        printf("max neighbors (d): %d\n\n", max_t);
    }
 
    void offline_mode()
    {
        while (true)
        {
            create_random_map();
            // printf("%d %d\n\n", height, width);
            // isAssigned = true;
            if (isBeautyModeON)
                print_beauty_map();
            else
                print_map();
            mySleep(3);
 
            curr_map_state = live_map;
            start();
            puts("\nSave current map       - press s");
            puts("Exit                   - press q or CTRL+C\n");
            mySleep(2);
            if (GetKeyState('Q') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
            {
                break;
            } 
            else if (GetKeyState('S') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
            {
                save_map();
            }
        }
    }
 
    void finish_view()
    {
        puts("It's done!");
        cout << "Good bye " << name << "!\n";
    }
};
 
int main()
{
    Live new_live;
    new_live.start_view();
    return 0;
} 