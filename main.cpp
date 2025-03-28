#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <string>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <map>

std::map<float, std::string> Figure_blocks;

const std::string Block = "[]";
//const std::string Block = std::string(1, char(219)) + std::string(1, char(219));
//const std::string Block = std::string(1, char(178)) + std::string(1, char(178));
const std::string Space = " .";

const char Score[] = "Score.txt";

const int width = 15;//10
const int length = 30;//20

const int Down = 5;//14
const int Across = 60;//70
const int Text_increment = 40;//40

int mili = 100;

inline void Print(std::vector<std::vector<std::string>> &Grid, int Full_lines,int Level, int Score, float Time_sec, int id);

void Directions(bool &up_key, bool &space_key, int &direct_x, int &direct_y);

int Get_score();

int Random_figure(int range);

int Place_figure(std::vector<std::vector<std::string>> &Grid, int &start_x, int &start_y, int Figures_id);

bool Move_figure(std::vector<std::vector<std::string>> &Grid, int id, int &start_x, int &start_y, int direct_x, int stage, bool agree);

void Rotate(std::vector<std::vector<std::string>> &Grid, int id, int stage, int &start_x, int &start_y);

bool Is_rotation_legal(std::vector<std::vector<std::string>> &Grid, int id, int stage, int start_x, int start_y, bool rotation);

bool Is_x_axys_movement_legal(std::vector<std::vector<std::string>> &Grid, int id, int stage, int start_x, int start_y, int direct_x);

void Remove_lines(std::vector<std::vector<std::string>> &Grid, int &Full_lines, int &Level,int &Score);

bool Game_over(std::vector<std::vector<std::string>> &Grid);

void Up_level(int &Level, int Full_lines);


int main()
{
    std::cout << "\033[?25l";
    std::srand(time(0));

    std::string Spaces ="  ";

    Figure_blocks[0.0] = /*"[][][][]";*/ (Block + Block + Block + Block);
    Figure_blocks[0.1] = /*"        ";*/ (Spaces + Spaces + Spaces + Spaces);

    Figure_blocks[1.0] = /*"[][]    ";*/ (Block + Block + Spaces + Spaces);
    Figure_blocks[1.1] = /*"[][]    ";*/ (Block + Block + Spaces + Spaces);

    Figure_blocks[2.0] = /*"    []  ";*/ (Spaces + Spaces + Block + Spaces);
    Figure_blocks[2.1] = /*"[][][]  ";*/ (Block + Block + Block + Spaces);

    Figure_blocks[3.0] = /*"[]      ";*/ (Block + Spaces + Spaces + Spaces);
    Figure_blocks[3.1] = /*"[][][]  ";*/ (Block + Block + Block + Spaces);

    Figure_blocks[4.0] = /*"  [][]  ";*/ (Spaces + Block + Block + Spaces);
    Figure_blocks[4.1] = /*"[][]    ";*/ (Block + Block + Spaces + Spaces);

    Figure_blocks[5.0] = /*"[][]    ";*/ (Block + Block + Spaces + Spaces);
    Figure_blocks[5.1] = /*"  [][]  ";*/ (Spaces + Block + Block + Spaces);

    Figure_blocks[6.0] = /*"  []    ";*/ (Spaces + Block + Spaces + Spaces);
    Figure_blocks[6.1] = /*"[][][]  ";*/ (Block + Block + Block + Spaces);

    /*for(auto values:Figure_blocks){
        std::cout<<values.second<<'|'<<std::endl;
    }*/

    std::vector<std::vector<std::string>> Grid(length, std::vector<std::string>(width, Space));
    Grid.insert(Grid.begin(), std::vector<std::string>(width, "  "));

    int Full_lines = 1;
    int Level = 1;
    int Score = 1;
    float Time_sec = 0;

    Sleep(1500);

    bool Is_placement_needed = true;

    int id;
    int start_x;
    int start_y;

    int stage = 0;
    bool temp;
    bool agree;
    bool moves;

    int id_after = Random_figure(7);
    int id_now = Random_figure(7);

    //for(int i = 0; i < 500; i++){
    while(Game_over(Grid) == true /*&& false == true*/){
        Time_sec+=(float(mili)/1000.0*2);

        int direct_x = 0;
        int direct_y = 0;

        bool up_key = false;
        bool space_key = false;

        bool rotation = false;

        if(Is_placement_needed){
            Remove_lines(Grid, Full_lines, Level, Score);
            id = Place_figure(Grid, start_x, start_y, id_now);
            Is_placement_needed = false;
        }

        Directions(up_key, space_key, direct_x, direct_y);

        if(up_key == true){

            temp = Is_rotation_legal(Grid, id, stage, start_x, start_y, rotation);
            if(temp == true){
                Rotate(Grid, id, stage, start_x, start_y);
                stage++;
            }
        }

        if(space_key == true){
            moves = Move_figure(Grid, id, start_x, start_y, direct_x, stage, true);

            while(moves == true){
                moves = Move_figure(Grid, id, start_x, start_y, direct_x, stage, true);
            }
            Is_placement_needed = true;
            stage = 0;
            id_now = id_after;
            id_after = Random_figure(7);
            continue;
        }

        if(id == 0 && stage == 2){stage = 0;}
        else if (id == 1 && stage == 1){stage = 0;}
        else if (id == 2 && stage == 4){stage = 0;}
        else if (id == 3 && stage == 4){stage = 0;}
        else if (id == 4 && stage == 2){stage = 0;}
        else if (id == 5 && stage == 2){stage = 0;}
        else if (id == 6 && stage == 4){stage = 0;}

        if(direct_y == 1){
            direct_y = 0;
            agree = Is_x_axys_movement_legal(Grid, id , stage, start_x, start_y, direct_x);
            moves = Move_figure(Grid, id, start_x, start_y, direct_x, stage, agree);

            if(moves == false){
                Is_placement_needed = true;
                stage = 0;
                continue;
            }
        }

        Sleep(mili/2);
        Print(Grid, Full_lines, Level, Score, Time_sec, id_after);

        agree = Is_x_axys_movement_legal(Grid, id , stage, start_x, start_y, direct_x);
        moves = Move_figure(Grid, id, start_x, start_y, direct_x, stage, agree);

        if(moves == false){
            Is_placement_needed = true;
            stage = 0;
            id_now = id_after;
            id_after = Random_figure(7);
            continue;
        }
    }

    return 0;
}

inline void Print(std::vector<std::vector<std::string>> &Grid, int Full_lines,int Level, int Score, float Time_sec, int id)
{
    std::cout << "\033[H";
    //std::system("cls");
    //std::cout << "\033[2J\033[H";

    for(int i = 0; i < Down; i++){
        std::cout<<'\n';
    }
    for (int i = 1; i < length+1; i++) {
        if(i == 0){
            int indent = log10(Full_lines);
            indent++;

            std::cout<<std::setw(Text_increment + 12)<<"FULL LINES: "<<Full_lines;;
            std::cout<<std::setw(Across - Text_increment - 12 - indent)<<"<!";
        }
        else if (i == 1){
            int indent = log10(Level);
            indent++;
            std::cout<<std::setw(Text_increment + 7)<<"LEVEL: "<<Level;
            std::cout<<std::setw(Across - Text_increment - 7 - indent)<<"<!";
        }
        else if (i == 2){
            int indent = log10(Score);
            indent++;
            std::cout<<std::setw(Text_increment + 7);
            std::cout<<"SCORE: "<<Score;
            std::cout<<std::setw(Across - Text_increment - 7 - indent)<<"<!";
        }
        else if (i == 3){
            int incrament = std::to_string(int(Time_sec) / 60).size();
            incrament += std::to_string(int(Time_sec) % 60).size();
            std::cout<<std::setw(Text_increment + 6);
            std::cout<<"TIME: "<<(int(Time_sec) / 60)<<":"<<(int(Time_sec)%60);
            std::cout<<std::setw(Across  - Text_increment - 7 - incrament)<<"<!";
        }
        else if (i == 10){
            std::cout<<std::setw(Text_increment+10)<<Figure_blocks[float(id)]<<std::setw(Across - Text_increment-10)<<"!<";
        }
        else if (i == 11){
            std::cout<<std::setw(Text_increment+10)<<Figure_blocks[float(id)+0.1]<<std::setw(Across - Text_increment-10)<<"!<";
        }
        else{
           std::cout<<std::setw(Across)<<"<!";
        }

        for (int j = 0; j < width; j++) {
            std::cout << Grid[i][j];
        }std::cout<<"!>"<< "\n";
    }

    std::cout<<std::setw(Across)<<"<!";
    for(int i = 0; i < width * 2; i++){
        std::cout<<"*";
    }std::cout<<"!>"<<'\n';

    std::cout<<std::setw(Across)<<"  ";

    for(int i = 0; i < width * 2; i++){
        if(i%2 == 0){
            std::cout<<char(92);
        }
        else {
            std::cout<<'/';}
    }
    //std::cout<<"\033[H";
    //std::cout<<'\n';
}

int Random_figure(int range){
    return rand() % range;
}

void Directions(bool &up_key, bool &space_key, int &direct_x, int &direct_y)
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'w':up_key = true;break;
        case 'd':direct_x = 1; break;
        case 's':direct_y = 1; break;
        case 'a':direct_x = -1; break;
        case ' ':space_key = true;break;
        }
    }
}

int Place_figure(std::vector<std::vector<std::string>> &Grid, int &start_x, int &start_y, int Figures_id)
{
    /*int seven = 7;
    int Figures_id = Random_figure(seven);
    //Figures_id+=6;*/

    int length_figure;
    int height;

    switch (Figures_id){
        case 0:length_figure = 4;break;
        case 1:length_figure = 3;break;
        case 2:length_figure = 3;break;
        case 3:length_figure = 2;break;
        case 4:length_figure = 3;break;
        case 5:length_figure = 3;break;
        case 6:length_figure = 3;break;
    }

    height = (length_figure > 3) ? 1 : 2;

    int starting_position = Random_figure(width - length_figure);
    start_x = starting_position;

    if(Figures_id == 0){
        start_y = 1;
        Grid[1][starting_position] = Block;
        Grid[1][starting_position + 1] = Block;
        Grid[1][starting_position + 2] = Block;
        Grid[1][starting_position + 3] = Block;
    }
    else if (Figures_id == 1){
        start_y = 0;
        Grid[1][starting_position] = Block;
        Grid[1][starting_position + 1] = Block;
        Grid[0][starting_position] = Block;
        Grid[0][starting_position + 1] = Block;
    }
    else if (Figures_id == 2){
        start_y = 1;
        Grid[1][starting_position] = Block;
        Grid[1][starting_position + 1] = Block;
        Grid[1][starting_position + 2] = Block;
        Grid[0][starting_position + 2] = Block;
    }
    else if (Figures_id == 3){
        start_y=1;
        Grid[1][starting_position] = Block;
        Grid[1][starting_position + 1] = Block;
        Grid[1][starting_position + 2] = Block;
        Grid[0][starting_position] = Block;
    }
    else if (Figures_id == 4){
        start_y = 0;
        Grid[0][starting_position+1] = Block;
        Grid[0][starting_position + 2] = Block;
        Grid[1][starting_position] = Block;
        Grid[1][starting_position + 1] = Block;
    }
    else if (Figures_id == 5){
        start_y = 0;
        Grid[0][starting_position] = Block;
        Grid[0][starting_position + 1] = Block;
        Grid[1][starting_position + 1] = Block;
        Grid[1][starting_position + 2] = Block;
    }
    else if (Figures_id == 6){
        start_y = 1;
        Grid[1][starting_position] = Block;
        Grid[1][starting_position + 1] = Block;
        Grid[1][starting_position + 2] = Block;
        Grid[0][starting_position + 1] = Block;
    }
    return Figures_id;
}

bool Move_figure(std::vector<std::vector<std::string>> &Grid, int id, int &start_x, int &start_y, int direct_x, int stage, bool agree)
{
    using namespace std;

    if(id == 0){
        if(stage == 0){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x + 4]);
                    start_x++;
                }else if (direct_x == -1){
                    std::swap(Grid[start_y][start_x+3], Grid[start_y][start_x - 1]);
                    start_x--;
                }
            }
            if(start_y == length || Grid[start_y + 1][start_x] == Block || Grid[start_y + 1][start_x + 1] == Block || Grid[start_y + 1][start_x + 2] == Block || Grid[start_y + 1][start_x + 3] == Block){
                    return false;
            }
            else {
                std::swap(Grid[start_y][start_x], Grid[start_y + 1][start_x]);
                std::swap(Grid[start_y][start_x + 1],Grid[start_y + 1][start_x + 1]);
                std::swap(Grid[start_y][start_x + 2], Grid[start_y + 1][start_x + 2]);
                std::swap(Grid[start_y][start_x + 3], Grid[start_y + 1][start_x + 3]);
            }
        }
        else if (stage == 1){
            if(agree == true){
                if(direct_x != 0){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x + direct_x]);
                    swap(Grid[start_y+1][start_x], Grid[start_y + 1][start_x + direct_x]);
                    swap(Grid[start_y+2][start_x], Grid[start_y + 2][start_x + direct_x]);
                    swap(Grid[start_y+3][start_x], Grid[start_y + 3][start_x + direct_x]);
                    start_x+=direct_x;
                }
            }
            if(start_y +3 == length || Grid[start_y + 4][start_x] == Block){
                return false;
            }
            else{
                swap(Grid[start_y][start_x], Grid[start_y + 4][start_x]);
            }
        }

    }
    else if (id == 1){
        if(agree == true){
            if(direct_x == 1){
                swap(Grid[start_y][start_x], Grid[start_y][start_x + 2]);
                swap(Grid[start_y + 1][start_x], Grid[start_y+1][start_x + 2]);

                start_x++;
            }
            else if (direct_x == -1){
                swap(Grid[start_y][start_x+1], Grid[start_y][start_x-1]);
                swap(Grid[start_y + 1][start_x+1], Grid[start_y+1][start_x-1]);

                start_x--;
            }
        }
        if(start_y + 1 == length || Grid[start_y + 2][start_x] == Block || Grid[start_y + 2][start_x + 1] == Block){
            return false;
        }
        else{
            swap(Grid[start_y][start_x], Grid[start_y + 2][start_x]);
            swap(Grid[start_y][start_x + 1], Grid[start_y + 2][start_x+1]);
        }
    }
    if (id == 2){
        if(stage == 0){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x+3]);
                    swap(Grid[start_y-1][start_x+2], Grid[start_y-1][start_x+3]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y-1][start_x+2], Grid[start_y-1][start_x+1]);
                    swap(Grid[start_y][start_x+2], Grid[start_y][start_x-1]);
                    start_x--;
                }
            }

            if(start_y  == length || Grid[start_y+1][start_x] == Block || Grid[start_y+1][start_x+1] == Block || Grid[start_y+1][start_x+2] == Block){
                return false;
            }
            else{
                swap(Grid[start_y][start_x], Grid[start_y+1][start_x]);
                swap(Grid[start_y][start_x+1],Grid[start_y+1][start_x+1]);
                swap(Grid[start_y-1][start_x+2], Grid[start_y+1][start_x+2]);
            }
        }
        if(stage == 1){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x + 2]);
                    swap(Grid[start_y+1][start_x+1], Grid[start_y+1][start_x+2]);
                    swap(Grid[start_y+2][start_x+1], Grid[start_y+2][start_x+2]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y][start_x+1], Grid[start_y][start_x-1]);
                    swap(Grid[start_y+1][start_x+1], Grid[start_y+1][start_x]);
                    swap(Grid[start_y+2][start_x+1], Grid[start_y+2][start_x]);
                    start_x--;
                }
            }

            if(start_y + 2 == length || Grid[start_y+1][start_x] == Block || Grid[start_y+3][start_x+1] == Block){
                return false;
            }
            else{
                swap(Grid[start_y][start_x], Grid[start_y+1][start_x]);
                swap(Grid[start_y][start_x+1], Grid[start_y+3][start_x+1]);
            }
        }
        else if (stage == 2){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x+3]);
                    swap(Grid[start_y+1][start_x], Grid[start_y+1][start_x+1]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y][start_x+2], Grid[start_y][start_x-1]);
                    swap(Grid[start_y+1][start_x], Grid[start_y+1][start_x-1]);
                    start_x--;
                }
            }

            if(start_y + 1 == length || Grid[start_y+2][start_x] == Block || Grid[start_y+1][start_x+1] == Block || Grid[start_y+1][start_x+2] == Block){
                return false;
            }
            else{
                swap(Grid[start_y][start_x], Grid[start_y+2][start_x]);
                swap(Grid[start_y][start_x+1], Grid[start_y+1][start_x+1]);
                swap(Grid[start_y][start_x+2], Grid[start_y+1][start_x+2]);
            }
        }
        else if (stage == 3){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x+1]);
                    swap(Grid[start_y+1][start_x], Grid[start_y+1][start_x+1]);
                    swap(Grid[start_y+2][start_x], Grid[start_y+2][start_x+2]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x-1]);
                    swap(Grid[start_y+1][start_x], Grid[start_y+1][start_x-1]);
                    swap(Grid[start_y+2][start_x+1], Grid[start_y+2][start_x-1]);

                    start_x--;
                }
            }

            if(start_y + 2 == length || Grid[start_y + 3][start_x] == Block || Grid[start_y+3][start_x+1] == Block){
                return false;
            }
            else{
                swap(Grid[start_y+2][start_x+1], Grid[start_y+3][start_x+1]);
                swap(Grid[start_y][start_x], Grid[start_y+3][start_x]);
            }
        }
    }
    if(id == 3){
        if(stage == 0){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y-1][start_x],Grid[start_y-1][start_x+1]);
                    swap(Grid[start_y][start_x],Grid[start_y][start_x+3]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y-1][start_x], Grid[start_y-1][start_x-1]);
                    swap(Grid[start_y][start_x+2], Grid[start_y][start_x-1]);
                    start_x--;
                }
            }
            if(start_y  == length || Grid[start_y+1][start_x] == Block || Grid[start_y+1][start_x+1] == Block || Grid[start_y+1][start_x+2] == Block){
                return false;
            }
            else{
                swap(Grid[start_y-1][start_x], Grid[start_y+1][start_x]);
                swap(Grid[start_y][start_x+1], Grid[start_y+1][start_x+1]);
                swap(Grid[start_y][start_x+2], Grid[start_y+1][start_x+2]);
            }
        }
        else if (stage == 1){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y-2][start_x+1], Grid[start_y-2][start_x+2]);
                    swap(Grid[start_y-1][start_x+1], Grid[start_y-1][start_x+2]);
                    swap(Grid[start_y][start_x], Grid[start_y][start_x+2]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y-2][start_x+1], Grid[start_y-2][start_x]);
                    swap(Grid[start_y-1][start_x+1], Grid[start_y-1][start_x]);
                    swap(Grid[start_y][start_x + 1], Grid[start_y][start_x-1]);
                    start_x--;
                }
            }
            if(start_y == length || Grid[start_y+1][start_x] == Block || Grid[start_y+1][start_x+1] == Block){
                return false;
            }
            else{
                swap(Grid[start_y][start_x], Grid[start_y+1][start_x]);
                swap(Grid[start_y-2][start_x+1], Grid[start_y+1][start_x + 1]);
            }
        }
        else if (stage == 2){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y+1][start_x+2], Grid[start_y+1][start_x+3]);
                    swap(Grid[start_y][start_x], Grid[start_y][start_x+3]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y+1][start_x+2], Grid[start_y+1][start_x+1]);
                    swap(Grid[start_y][start_x+2], Grid[start_y][start_x-1]);
                    start_x--;
                }
            }
            if(start_y + 1 == length || Grid[start_y+1][start_x] == Block || Grid[start_y+1][start_x+1] == Block || Grid[start_y+2][start_x+2] == Block){
                return false;
            }
            else {
                swap(Grid[start_y][start_x], Grid[start_y+1][start_x]);
                swap(Grid[start_y][start_x+1], Grid[start_y+1][start_x+1]);
                swap(Grid[start_y][start_x+2], Grid[start_y+2][start_x+2]);
            }
        }
        else if (stage == 3){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x + 2]);
                    swap(Grid[start_y+1][start_x], Grid[start_y+1][start_x+1]);
                    swap(Grid[start_y+2][start_x], Grid[start_y+2][start_x+1]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y][start_x+1], Grid[start_y][start_x-1]);
                    swap(Grid[start_y+1][start_x], Grid[start_y+1][start_x-1]);
                    swap(Grid[start_y+2][start_x], Grid[start_y+2][start_x-1]);
                    start_x--;
                }
            }
            if(start_y + 2 == length || Grid[start_y+3][start_x] == Block || Grid[start_y + 1][start_x + 1] == Block){
                return false;
            }
            else{
                swap(Grid[start_y][start_x+1], Grid[start_y+1][start_x+1]);
                swap(Grid[start_y][start_x], Grid[start_y+3][start_x]);
            }
        }

    }
    else if (id == 4){
        if(stage == 0){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y][start_x+1], Grid[start_y][start_x+3]);
                    swap(Grid[start_y+1][start_x], Grid[start_y+1][start_x+2]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y][start_x+2], Grid[start_y][start_x]);
                    swap(Grid[start_y+1][start_x+1], Grid[start_y+1][start_x-1]);
                    start_x--;
                }
            }
            if(start_y + 1 == length || Grid[start_y+2][start_x] == Block || Grid[start_y+2][start_x+1] == Block || Grid[start_y+1][start_x+2] == Block){
                return false;
            }
            else{
                swap(Grid[start_y+1][start_x], Grid[start_y+2][start_x]);
                swap(Grid[start_y][start_x+1], Grid[start_y+2][start_x+1]);
                swap(Grid[start_y][start_x+2], Grid[start_y+1][start_x+2]);
            }
        }
        if(stage == 1){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x+1]);
                    swap(Grid[start_y+1][start_x], Grid[start_y+1][start_x+2]);
                    swap(Grid[start_y+2][start_x+1], Grid[start_y+2][start_x+2]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x-1]);
                    swap(Grid[start_y+1][start_x+1], Grid[start_y+1][start_x-1]);
                    swap(Grid[start_y+2][start_x+1], Grid[start_y+2][start_x]);
                    start_x--;
                }
            }
            if(start_y + 2 >= length || Grid[start_y+2][start_x] == Block || Grid[start_y+3][start_x+1] == Block){
                return false;
            }
            else{
                swap(Grid[start_y][start_x], Grid[start_y+2][start_x]);
                swap(Grid[start_y+1][start_x+1], Grid[start_y+3][start_x+1]);
            }
        }
    }
    else if (id == 5){
        if(stage == 0){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x+2]);
                    swap(Grid[start_y+1][start_x+1], Grid[start_y+1][start_x+3]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y][start_x+1], Grid[start_y][start_x-1]);
                    swap(Grid[start_y+1][start_x+2], Grid[start_y+1][start_x]);
                    start_x--;
                }
            }
            if(start_y + 1 == length || Grid[start_y+1][start_x] == Block || Grid[start_y+2][start_x+1] == Block || Grid[start_y+2][start_x+2] == Block){
                return false;
            }
            else{
                swap(Grid[start_y][start_x], Grid[start_y+1][start_x]);
                swap(Grid[start_y][start_x + 1], Grid[start_y+2][start_x+1]);
                swap(Grid[start_y+1][start_x+2], Grid[start_y+2][start_x+2]);
            }
        }
        else if (stage == 1){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x+1]);
                    swap(Grid[start_y+1][start_x], Grid[start_y+1][start_x+2]);
                    swap(Grid[start_y+2][start_x+1], Grid[start_y+2][start_x+2]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x-1]);
                    swap(Grid[start_y+1][start_x+1], Grid[start_y+1][start_x-1]);
                    swap(Grid[start_y+2][start_x+1], Grid[start_y+2][start_x]);
                    start_x--;
                }
            }
            if(start_y + 2 == length || Grid[start_y + 2][start_x] == Block || Grid[start_y+3][start_x+1] == Block){
                return false;
            }
            else{
                swap(Grid[start_y][start_x], Grid[start_y+2][start_x]);
                swap(Grid[start_y+1][start_x+1], Grid[start_y+3][start_x+1]);
            }
        }

    }
    else if (id == 6){
        if(stage == 0){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x+3]);
                    swap(Grid[start_y-1][start_x+1], Grid[start_y-1][start_x+2]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y][start_x+2], Grid[start_y][start_x-1]);
                    swap(Grid[start_y-1][start_x+1], Grid[start_y-1][start_x]);
                    start_x--;
                }
            }
            if(start_y == length || Grid[start_y+1][start_x] == Block || Grid[start_y+1][start_x+1] == Block || Grid[start_y+1][start_x+2] == Block){
                return false;
            }
            else{
                swap(Grid[start_y][start_x], Grid[start_y+1][start_x]);
                swap(Grid[start_y-1][start_x+1], Grid[start_y+1][start_x+1]);
                swap(Grid[start_y][start_x+2], Grid[start_y+1][start_x+2]);
            }
        }
        else if (stage == 1){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y-1][start_x+1], Grid[start_y-1][start_x+2]);
                    swap(Grid[start_y][start_x], Grid[start_y][start_x+2]);
                    swap(Grid[start_y+1][start_x+1], Grid[start_y+1][start_x+2]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y-1][start_x+1], Grid[start_y-1][start_x]);
                    swap(Grid[start_y][start_x+1], Grid[start_y][start_x-1]);
                    swap(Grid[start_y+1][start_x+1], Grid[start_y+1][start_x]);
                    start_x--;
                }
            }
            if(start_y + 1 == length || Grid[start_y+1][start_x] == Block || Grid[start_y+2][start_x+1] == Block){
                return false;
            }
            else {
                swap(Grid[start_y][start_x], Grid[start_y+1][start_x]);
                swap(Grid[start_y-1][start_x+1], Grid[start_y+2][start_x+1]);
            }
        }
        else if(stage == 2){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x+3]);
                    swap(Grid[start_y+1][start_x+1], Grid[start_y+1][start_x+2]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y][start_x+2], Grid[start_y][start_x-1]);
                    swap(Grid[start_y+1][start_x+1], Grid[start_y+1][start_x]);
                    start_x--;
                }
            }
            if(start_y + 1 == length || Grid[start_y+1][start_x] == Block || Grid[start_y+2][start_x+1] == Block || Grid[start_y+1][start_x+2] == Block){
                return false;
            }
            else{
                swap(Grid[start_y][start_x], Grid[start_y+1][start_x]);
                swap(Grid[start_y][start_x+1], Grid[start_y+2][start_x+1]);
                swap(Grid[start_y][start_x+2], Grid[start_y+1][start_x+2]);
            }
        }
        else if (stage == 3){
            if(agree == true){
                if(direct_x == 1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x+1]);
                    swap(Grid[start_y+1][start_x], Grid[start_y+1][start_x+2]);
                    swap(Grid[start_y+2][start_x], Grid[start_y+2][start_x+1]);
                    start_x++;
                }
                else if (direct_x == -1){
                    swap(Grid[start_y][start_x], Grid[start_y][start_x-1]);
                    swap(Grid[start_y+1][start_x+1], Grid[start_y+1][start_x-1]);
                    swap(Grid[start_y+2][start_x], Grid[start_y+2][start_x-1]);
                    start_x--;
                }
            }
            if(start_y + 2 == length || Grid[start_y+3][start_x] == Block || Grid[start_y+2][start_x+1] == Block){
                return false;
            }
            else{
                swap(Grid[start_y][start_x], Grid[start_y+3][start_x]);
                swap(Grid[start_y+1][start_x+1], Grid[start_y+2][start_x+1]);
            }
        }
    }
    start_y++;
    return true;
}

void Rotate(std::vector<std::vector<std::string>> &Grid, int id, int stage, int &start_x, int &start_y)
{
    if(id == 0){
        if(stage == 0){
            std::swap(Grid[start_y][start_x+3], Grid[start_y - 1][start_x + 2]);
            std::swap(Grid[start_y][start_x+1], Grid[start_y - 2][start_x + 2]);
            std::swap(Grid[start_y][start_x], Grid[start_y - 3][start_x + 2]);

            start_x += 2;
            start_y -= 3;
        }
        else if(stage == 1){
            std::swap(Grid[start_y][start_x], Grid[start_y + 3][start_x - 2]);
            std::swap(Grid[start_y+1][start_x], Grid[start_y +3][start_x - 1]);
            std::swap(Grid[start_y+2][start_x], Grid[start_y + 3][start_x + 1]);

            start_x -= 2;
            start_y += 3;
        }
    }
    if(id == 2){
        if(stage == 0){
            swap(Grid[start_y][start_x], Grid[start_y-2][start_x]);
            swap(Grid[start_y-1][start_x+2], Grid[start_y-2][start_x+1]);
            swap(Grid[start_y][start_x+2], Grid[start_y-1][start_x+1]);

            start_y-=2;
        }
        else if (stage == 1){
            swap(Grid[start_y][start_x], Grid[start_y+2][start_x]);
            swap(Grid[start_y][start_x+1], Grid[start_y+1][start_x]);
            swap(Grid[start_y+2][start_x+1], Grid[start_y+1][start_x+2]);

            start_y++;
        }
        else if (stage == 2){
            swap(Grid[start_y][start_x+1], Grid[start_y-1][start_x]);
            swap(Grid[start_y][start_x+2], Grid[start_y+1][start_x+1]);

            start_y--;
        }
        else if (stage == 3){
            swap(Grid[start_y][start_x], Grid[start_y+1][start_x+2]);
            swap(Grid[start_y+1][start_x], Grid[start_y+2][start_x+2]);

            start_y+=2;
        }
    }
    else if (id == 3){
        if(stage == 0){
            swap(Grid[start_y][start_x], Grid[start_y-2][start_x+2]);
            swap(Grid[start_y-1][start_x], Grid[start_y-1][start_x+2]);
            start_x++;
        }
        else if(stage == 1){
            swap(Grid[start_y][start_x], Grid[start_y-1][start_x]);
            swap(Grid[start_y][start_x+1], Grid[start_y][start_x+2]);
            swap(Grid[start_y-2][start_x+1], Grid[start_y-1][start_x+2]);

            start_y--;
        }
        else if (stage == 2){
            swap(Grid[start_y][start_x+2], Grid[start_y+1][start_x]);
            swap(Grid[start_y+1][start_x+2], Grid[start_y+2][start_x]);
        }
        else if (stage == 3){
            swap(Grid[start_y][start_x], Grid[start_y+2][start_x+1]);
            swap(Grid[start_y][start_x+1], Grid[start_y+2][start_x+2]);
            start_y+=2;
        }
    }
    else if (id == 4){
        if(stage == 0){
            swap(Grid[start_y+1][start_x], Grid[start_y][start_x]);
            swap(Grid[start_y][start_x+2], Grid[start_y-1][start_x]);
            start_y--;
        }
        else if (stage == 1){
            swap(Grid[start_y][start_x], Grid[start_y+2][start_x]);
            swap(Grid[start_y+1][start_x], Grid[start_y+1][start_x+2]);
            start_y++;
        }
    }
    else if (id == 5){
        if(stage == 0){
            swap(Grid[start_y+1][start_x+2], Grid[start_y-1][start_x]);
            start_y--;
        }
        else if (stage == 1){
            swap(Grid[start_y][start_x], Grid[start_y+2][start_x+2]);
            start_y++;
        }
    }
    else if (id == 6){
        if(stage == 0){
            swap(Grid[start_y][start_x], Grid[start_y-2][start_x+2]);
            swap(Grid[start_y][start_x+1], Grid[start_y-1][start_x+2]);

            start_x++;
            start_y--;
        }
        else if (stage == 1){
            swap(Grid[start_y][start_x+1], Grid[start_y-1][start_x-1]);
            swap(Grid[start_y+1][start_x+1], Grid[start_y-1][start_x]);
            start_x--;
            start_y--;
        }
        else if (stage == 2){
            swap(Grid[start_y][start_x+1], Grid[start_y+1][start_x]);
            swap(Grid[start_y][start_x+2], Grid[start_y+2][start_x]);
        }
        else if(stage == 3){
            swap(Grid[start_y][start_x], Grid[start_y+2][start_x+1]);
            swap(Grid[start_y+1][start_x], Grid[start_y+2][start_x+2]);
            start_y+=2;
        }
    }
}

bool Is_rotation_legal(std::vector<std::vector<std::string>> &Grid, int id, int stage, int start_x, int start_y, bool rotation)
{
    if(id == 0){
        if(stage == 0){
            if(start_y - 3 < 1 || Grid[start_y - 3][start_x + 2] == Block || Grid[start_y - 2][start_x + 2] == Block && Grid[start_y - 3][start_x + 2] == Block){
                return false;
            }
        }
        else if (stage == 1){
            if(start_x - 2 < 0 || start_x + 1 >= width || Grid[start_y - 3][start_x - 2] == Block || Grid[start_y - 3][start_x - 1] == Block || Grid[start_y - 3][start_x + 1] == Block){
                return false;
            }
        }
    }
    else if (id == 1){
        return false;
    }
    if (id == 2){

        if(stage == 0){
            if(start_y -1 < 0 || Grid[start_y-2][start_x] == Block || Grid[start_y-2][start_x+1] == Block || Grid[start_y-1][start_x+1] == Block){
                return false;
            }
        }
        else if (stage == 1){
            if(start_x+2 == width || Grid[start_y+1][start_x] == Block || Grid[start_y+2][start_x] == Block || Grid[start_y+1][start_x+2] == Block){
                return false;
            }
        }
        else if (stage == 2){
            if(start_y-1 < 0 && Grid[start_y-1][start_x] == Block || Grid[start_y+1][start_x+1] == Block){
                return false;
            }
        }
        else if (stage == 3){
            if(start_x+2 == length || Grid[start_y+1][start_x+2] == Block || Grid[start_y+2][start_x+2] == Block){
                return false;
            }
        }
    }
    else if (id == 3){
        if(stage == 0){
            if(start_y - 2 < 1 || Grid[start_y-2][start_x+2] == Block || Grid[start_y-1][start_x+2] == Block){
                return false;
            }
        }
        else if (stage == 1){
            if(start_x+2 == width || start_y-2 < 0 || Grid[start_y][start_x+2] == Block || Grid[start_y-1][start_x+2] == Block || Grid[start_y-1][start_x] == Block){
                return false;
            }
        }
        else if (stage == 2){
            if(start_y + 2 == length || Grid[start_y+1][start_x] == Block || Grid[start_y+2][start_x] == Block){
                return false;
            }
        }
        else if (stage == 3){
            if(start_x + 2 == width || Grid[start_y+2][start_x+1] == Block || Grid[start_y+2][start_x+2] == Block){
                return false;
            }
        }
    }
    else if (id == 4){
        if(stage == 0){
            if(start_y - 1 < 0 || Grid[start_y][start_x] == Block || Grid[start_y-1][start_x] == Block){
                return false;
            }
        }
        else if (stage == 1){
            if(start_y - 1 < 0 || Grid[start_y+2][start_x] == Block || Grid[start_y+1][start_x+2] == Block){
                return false;
            }
        }
    }
    else if (id == 5){
        if(stage == 0){
            if(start_y - 1 < 0 || Grid[start_y-1][start_x] == Block){
                return false;
            }
        }
        else if (stage == 1){
            if(start_y - 1 < 0 || Grid[start_y+2][start_x+2] == Block){
                return false;
            }
        }
    }
    else if (id == 6){
        if(stage == 0){
            if(start_y - 2 < 0 || Grid[start_y-2][start_x+2] == Block || Grid[start_y-1][start_x+2] == Block){
                return false;
            }
        }
        else if (stage == 1){
            if(start_y - 1 < 0 || start_x - 1 < 0 || Grid[start_y-1][start_x-1] == Block || Grid[start_y-1][start_x] == Block) {
                return false;
            }
        }
        else if (stage == 2){
            if(Grid[start_y+1][start_x] == Block || Grid[start_y+2][start_x] == Block){
                return false;
            }
        }
        else if (stage == 3){
            if(start_x + 2 == width || Grid[start_y+2][start_x+1] == Block || Grid[start_y+2][start_x+2] == Block){
                return false;
            }
        }
    }


    return true;
}

bool Is_x_axys_movement_legal(std::vector<std::vector<std::string>> &Grid, int id, int stage, int start_x, int start_y, int direct_x)
{
    if(id == 0){
        if(stage == 0){
            if(direct_x == 1){
                if(start_x + 4 >= width || Grid[start_y][start_x + 4] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y][start_x - 1] == Block){
                    return false;
                }
            }
        }
        else if(stage == 1){
            if(start_x + direct_x < 0 || start_x + direct_x >= width || Grid[start_y][start_x + direct_x] == Block || Grid[start_y-1][start_x + direct_x] == Block || Grid[start_y - 2][start_x + direct_x] == Block || Grid[start_y - 3][start_x + direct_x] == Block){
                return false;
            }

        }
    }
    else if (id == 1){
        if(direct_x == 1){
            if(start_x + 2 == width  || Grid[start_y][start_x+2] == Block || Grid[start_y+1][start_x+2] == Block){
                return false;
            }
        }
        else if (direct_x == -1){
            if(start_x - 1 < 0 || Grid[start_y][start_x - 1] == Block || Grid[start_y+1][start_x - 1] == Block){
                return false;
            }
        }
    }
    else if (id == 2){
        if(stage == 0){
            if(direct_x == 1){
                if(start_x + 3 == width || Grid[start_y][start_x+3] == Block || Grid[start_y-1][start_x+3] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y][start_x-1] == Block || Grid[start_y-1][start_x+1] == Block){
                    return false;
                }
            }
        }
        else if(stage == 1){
            if(direct_x == 1){
                if(start_x + 2 == width || Grid[start_y][start_x+2] == Block || Grid[start_y+1][start_x+2] == Block || Grid[start_y+2][start_x+2] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y][start_x-1] == Block || Grid[start_y+1][start_x] == Block || Grid[start_y+2][start_x] == Block){
                    return false;
                }
            }
        }
        else if (stage == 2){
            if(direct_x == 1){
                if(start_x + 3 == width || Grid[start_y+2][start_x+2] == Block || Grid[start_y+1][start_x+1] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y][start_x-1] == Block || Grid[start_y+1][start_x-1] == Block){
                    return false;
                }
            }
        }
        else if (stage == 3){
            if(direct_x == 1){
                if(start_x + 2 == width || Grid[start_y][start_x+1] == Block || Grid[start_y+1][start_x+1] == Block || Grid[start_y+2][start_x+2] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y][start_x-1] == Block || Grid[start_y+1][start_x-1] == Block || Grid[start_y+2][start_x-1] == Block){
                    return false;
                }
            }
        }
    }
    else if (id == 3){
        if(stage == 0){
            if(direct_x == 1){
                if(start_x + 3 == width || Grid[start_y-1][start_x+1] == Block || Grid[start_y][start_x + 3] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y-1][start_x-1] == Block || Grid[start_y][start_x-1] == Block){
                    return false;
                }
            }
        }
        else if (stage == 1){
            if(direct_x == 1){
                if(start_x + 2 == width || Grid[start_y][start_x+2] == Block || Grid[start_y-1][start_x+2] == Block || Grid[start_y-2][start_x+2] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y][start_x-1] == Block || Grid[start_y-1][start_x] == Block || Grid[start_y-2][start_x] == Block){
                    return false;
                }
            }
        }
        else if (stage == 2){
            if(direct_x == 1){
                if(start_x + 3 == width || Grid[start_y][start_x + 3] == Block || Grid[start_y+1][start_x + 3] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y][start_x-1] == Block || Grid[start_y+1][start_x+1] == Block){
                    return false;
                }
            }
        }
        else if (stage == 3){
            if(direct_x == 1){
                if(start_x + 2 == width || Grid[start_y][start_x+2] == Block || Grid[start_y+1][start_x+1] == Block || Grid[start_y + 2][start_x+1] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y][start_x-1] == Block || Grid[start_y+1][start_x-1] == Block || Grid[start_y+2][start_x-1] == Block){
                    return false;
                }
            }
        }
    }
    else if (id == 4){
        if(stage == 0){
            if(direct_x == 1){
                if(start_x + 3 == width || Grid[start_y][start_x + 3] == Block || Grid[start_y+1][start_x+2] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y][start_x] == Block || Grid[start_y+1][start_x-1] == Block){
                    return false;
                }
            }
        }
        if(stage == 1){
            if(direct_x == 1){
                if(start_x +2 == width || Grid[start_y][start_x+1] == Block || Grid[start_y+1][start_x+2] == Block || Grid[start_y+2][start_x+2] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y][start_x-1] == Block || Grid[start_y+1][start_x-1] == Block || Grid[start_y+2][start_x] == Block){
                    return false;
                }
            }
        }
    }
    else if(id == 5){
        if(stage == 0){
            if(direct_x == 1){
                if(start_x + 3 == width || Grid[start_y][start_x+2] == Block || Grid[start_y+1][start_x+3] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x -1 < 0 || Grid[start_y][start_x-1] == Block || Grid[start_y+1][start_x] == Block){
                    return false;
                }
            }
        }
        else if (stage == 1){
            if(direct_x == 1){
                if(start_x + 2 == width || Grid[start_y][start_x+1] == Block || Grid[start_y+1][start_x+2] == Block || Grid[start_y+2][start_x+2] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y][start_x-1] == Block || Grid[start_y+1][start_x-1] == Block || Grid[start_y+2][start_x] == Block){
                    return false;
                }
            }
        }
    }
    else if (id == 6){
        if(stage == 0){
            if(direct_x == 1){
                if(start_x+3 == width || Grid[start_y-1][start_x+2] == Block || Grid[start_y][start_x+3] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y][start_x-1] == Block || Grid[start_y-1][start_x] == Block){
                    return false;
                }
            }
        }
        if(stage == 1){
            if(direct_x == 1){
                if(start_x + 2 == width || Grid[start_y-1][start_x+2] == Block || Grid[start_y][start_x+2] == Block || Grid[start_y+1][start_x+2] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y-1][start_x] == Block || Grid[start_y][start_x-1] == Block || Grid[start_y+1][start_x] == Block){
                    return false;
                }
            }
        }
        else if (stage == 2){
            if(direct_x == 1){
                if(start_x + 3 == width || Grid[start_y][start_x+3] == Block || Grid[start_y+1][start_x+2] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x - 1 < 0 || Grid[start_y][start_x-1] == Block || Grid[start_y+1][start_x] == Block){
                    return false;
                }
            }
        }
        else if (stage == 3){
            if(direct_x == 1){
                if(start_x + 2 == width || Grid[start_y][start_x+1] == Block || Grid[start_y+1][start_x+2] == Block || Grid[start_y+2][start_x+1] == Block){
                    return false;
                }
            }
            else if (direct_x == -1){
                if(start_x -1 < 0 || Grid[start_y][start_x-1] == Block || Grid[start_y+1][start_x-1] == Block || Grid[start_y+2][start_x-1] == Block){
                    return false;
                }
            }
        }
    }

    return true;
}

void Remove_lines(std::vector<std::vector<std::string>> &Grid, int &Full_lines, int &Level,int &Score)
{
    for(int i = 1 ; i < length + 1; i++){
        int count_blocks = 0;
        for(int j = 0; j < width; j++){
            if(Grid[i][j] == Block){
                count_blocks++;
            }
        }
        if(count_blocks == width){
            Full_lines++;
            Up_level(Level, Full_lines);
            Score += 100 + Full_lines * 11;
            Grid.erase(Grid.begin() + i);
            Grid.insert(Grid.begin() + 1, std::vector<std::string>(width, Space));
        }
    }
}

bool Game_over(std::vector<std::vector<std::string>> &Grid)
{
    int count_blocks = 0;
    for(int j = 0; j < width; j++){
        if(Grid[0][j] == Block){
            count_blocks++;
        }
    }
    if(count_blocks <= 2){
        return true;
    }
    return false;
}

void Up_level(int &Level, int Full_lines){
    if(Full_lines == 2){
        Level++;
        mili -= 10;
    }
    else if(Full_lines == 3){
        Level++;
        mili -= 10;
    }
    else if(Full_lines == 4){
        Level++;
        mili -= 10;
    }
}

