
#include <math.h>
#include <iostream>
#include <chrono>
#include <ncurses.h>

using namespace std;



struct player_t {
    float pos_x;
    float pos_y;
    float angle;
};

const int screen_size_width = 120;
const int screen_size_height = 40;

const int map_size_width = 16;
const int map_size_height = 16;

const float field_of_view = 3.14159 / 4.0;
const float field_depth = 16.0;

struct player_t player = {
    0.0f, 0.0f, 0.0f,
};

void screen_clear(void) { cout << "\033[2J" << flush; }
void screen_set_cursor(unsigned int y, unsigned int x) {
        cout << "\033[" << +x << ";" << +y << "H" << flush;
}

void screen_print(WINDOW *win,char * screen) {
    wclear(win);
    wmove(win, 0,0);
    wprintw(win, "%f ", player.pos_y);
    wprintw(win, "%f ", player.pos_y);
    wprintw(win, "%f ", player.angle);
    wmove(win, 1,0);
    while (*screen != '\0') {
        waddch(win, *screen++);
    }
    wrefresh(win);
}

int main(void) {
    char * screen = new char[screen_size_width*screen_size_height + 1];
    screen[screen_size_width*screen_size_height + 1] = '\0';
    const char *map = 
        "################" 
        "#..............#" 
        "#..............#" 
        "#..............#" 
        "#..............#" 
        "#..............#" 
        "#...........#..#" 
        "#...........#..#" 
        "#...........#..#" 
        "#..............#" 
        "#..#...........#" 
        "#..#...........#" 
        "#..#......######" 
        "#..#...........#" 
        "#..#...........#" 
        "################";


        initscr();
        cbreak();
        noecho();
        keypad(stdscr, true);

        WINDOW *win = newwin(screen_size_height, screen_size_width, 0,0);
        timeout(200);


      player.pos_x = 8.0f;
      player.pos_y = 8.0f;

      auto tp1 = chrono::system_clock::now();
      auto tp2 = chrono::system_clock::now();



    for(;;) {
        tp2 = chrono::system_clock::now();
        chrono::duration<float> duration = tp2 - tp1;
        tp1 = tp2;
        float elapsed_time = duration.count();


        int ch = getch();
        if (ch == KEY_LEFT) {
            player.angle -= 0.8f * elapsed_time;
        } else if (ch == KEY_RIGHT) {
            player.angle += 0.8f * elapsed_time;
        } else if (ch == KEY_UP) {
            float val_x = player.pos_x + sinf(player.angle) * 5.0 * elapsed_time;
            float val_y = player.pos_y + cosf(player.angle) * 5.0 * elapsed_time;

            if (map[(int)val_x + (int)val_y * map_size_height] != '#') {
            player.pos_x = val_x;
            player.pos_y = val_y;
            }

        } else if (ch == KEY_DOWN) {
            float val_x = player.pos_x - sinf(player.angle) * 5.0 * elapsed_time;
            float val_y = player.pos_y - cosf(player.angle) * 5.0 * elapsed_time;

            if (map[(int)val_x + (int)val_y * map_size_height] != '#') {
            player.pos_x = val_x;
            player.pos_y = val_y;
            }
        }


        for (int x = 0; x < screen_size_width; x++) {
            float field_ray_angle = (player.angle - field_of_view / 2.0) + ((float)x / (float)screen_size_width) *
            field_of_view;

            float distance = 0;

            float eye_x = sinf(field_ray_angle);
            float eye_y = cosf(field_ray_angle);

            int cell_x = 0;
            int cell_y = 0;

            do {
                distance += 0.1f;
                cell_x = (float)player.pos_x + eye_x * distance;
                cell_y = (float)player.pos_y + eye_y * distance;
            } while(map[cell_x  + cell_y  * map_size_width] != '#' && distance < field_depth );

            int ceiling = (float)screen_size_height / 2.0 - screen_size_height / distance;
            int floor = screen_size_height - ceiling;

            short shade_wall = ' ';
            if (distance <= field_depth / 4.0) {
                shade_wall = '$';
            } else if (distance < field_depth / 3.0) {
                shade_wall = '%';
            } else if (distance < field_depth / 2.0) {
                shade_wall = '@';
            } else if (distance < field_depth) {
                shade_wall = '#';
            }
               
            for (int y = 0; y < screen_size_height ; y++) {
                if ( y >= ceiling && y <= floor) {
                    screen[x + y * screen_size_width] = shade_wall;
                } else {
                    char shade_floor = ' ';
                    float b = 1.0 - ((float) y - screen_size_height / 2.0) / ((float) screen_size_height / 2.0);
                    if (b < 0.25) {
                        shade_floor = '+';
                    } else if (b < 0.75) {
                        shade_floor = '-';
                    } else if (b < 0.9) {
                        shade_floor = '.';
                    }
                    screen[x + y * screen_size_width] = shade_floor;
                } 
            }
                
        }

        screen_print(win, screen);
    }



    return 0;
}
