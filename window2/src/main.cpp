#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Rect.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <math.h>

#include <chrono>
#include <iostream>

const char *map =
    "################"
    "#..........#...#"
    "#.#####....#.#.#"
    "#....#.....#.#.#"
    "#.#..###.###.#.#"
    "#.#............#"
    "#.######....#..#"
    "#...........#..#"
    "#######.#####..#"
    "#.....#.....#..#"
    "#..#..####..#..#"
    "#..#...........#"
    "#..#..#...######"
    "#..#..###......#"
    "#..#..#....##..#"
    "################";

struct player_t {
    double pos_x;
    double pos_y;
    double angle;
};

const int screen_size_width = 120;
const int screen_size_height = 60;

const int map_size_width = 16;
const int map_size_height = 16;

const double field_of_view = 3.14159 / 4.0;
const double field_depth = 6.0;

struct player_t player = {
    8.0f,
    8.0f,
    0.0f,
};

class Draw : Fl_Widget {
   public:
    Draw(int x, int y, int w, int h, const char *label);
    void draw(void) override;
    int handle(int event) override;
    void update(void) { redraw(); }
};

Draw::Draw(int x, int y, int w, int h, const char *label)
    : Fl_Widget(x, y, w, h, label) {}

void Draw::draw(void) {
    fl_rectf(0, 0, w(), h(), FL_WHITE);
    for (int x = 0; x < screen_size_width; x++) {
        double field_ray_angle =
            (player.angle - field_of_view / 2.0) +
            ((double)x / (double)screen_size_width) * field_of_view;

        double distance = 0.0f;
        double eye_x = sinf(field_ray_angle);
        double eye_y = cosf(field_ray_angle);
        int cell_x;
        int cell_y;

        do {
            distance += 0.1f;
            cell_x = (double)player.pos_x + eye_x * distance;
            cell_y = (double)player.pos_y + eye_y * distance;
        } while (map[cell_x + cell_y * map_size_width] != '#' &&
                 distance < field_depth);

        double ceiling = (double)screen_size_height / 2.0 -
                         (double)screen_size_height / distance;
        double floor = (double)screen_size_height - ceiling;

        fl_color(180, 180, 180);
        if (distance <= field_depth / 4.0) {
            fl_color(160, 160, 160);
        } else if (distance < field_depth / 3.5) {
            fl_color(144, 144, 144);
        } else if (distance < field_depth / 3.0) {
            fl_color(120, 120, 120);
        } else if (distance < field_depth / 2.5) {
            fl_color(96, 96, 96);
        } else if (distance < field_depth / 2.0) {
            fl_color(72, 72, 72);
        } else if (distance < field_depth / 1.5) {
            fl_color(48, 48, 48);
        } else if (distance < field_depth / 1.2) {
            fl_color(32, 32, 32);
        } else if (distance < field_depth) {
            fl_color(24, 24, 24);
        } else if (distance < field_depth / 0.5) {
            fl_color(8, 8, 8);
        }

        fl_begin_polygon();
        fl_vertex(x * 10, ceiling * 10.0);
        fl_vertex(x * 10, (floor * 10.0 - ceiling * 10.0));
        fl_vertex(x * 10 + 10, ceiling * 10.0);
        fl_vertex(x * 10, (floor * 10.0 - ceiling * 10.0));
        fl_vertex(x * 10 + 10, (floor * 10.0 - ceiling * 10.0));
        fl_vertex(x * 10 + 10, ceiling * 10.0);
        fl_end_polygon();

        fl_color(224, 224, 224);
        fl_begin_polygon();
        fl_vertex(x * 10, (floor * 10.0 - ceiling * 10.0));
        fl_vertex(x * 10, screen_size_height * 10.0);
        fl_vertex(x * 10 + 10, (floor * 10.0 - ceiling * 10.0));
        fl_vertex(x * 10, screen_size_height * 10.0);
        fl_vertex(x * 10 + 10, screen_size_height * 10.0);
        fl_vertex(x * 10 + 10, (floor * 10.0 - ceiling * 10.0));
        fl_end_polygon();
    }
}

auto tp1 = std::chrono::system_clock::now();
auto tp2 = std::chrono::system_clock::now();
std::chrono::duration<float> duration = tp2 - tp1;

int Draw::handle(int event) {
    if (event == FL_FOCUS || event == FL_UNFOCUS) {
        return 1;
    }
    if (event == FL_KEYDOWN) {
        float elapsed_time = duration.count();
        if (Fl::event_key(FL_Left)) {
            player.angle -= 3.5f * elapsed_time;
        } else if (Fl::event_key(FL_Right)) {
            player.angle += 3.5f * elapsed_time;
        } else if (Fl::event_key(FL_Up)) {
            double val_x =
                player.pos_x + sinf(player.angle) * 5.0 * elapsed_time;
            double val_y =
                player.pos_y + cosf(player.angle) * 5.0 * elapsed_time;
            if (map[(int)val_x + (int)val_y * map_size_height] != '#') {
                player.pos_x = val_x;
                player.pos_y = val_y;
            }
        } else if (Fl::event_key(FL_Down)) {
            double val_x =
                player.pos_x - sinf(player.angle) * 5.0 * elapsed_time;
            double val_y =
                player.pos_y - cosf(player.angle) * 5.0 * elapsed_time;
            if (map[(int)val_x + (int)val_y * map_size_height] != '#') {
                player.pos_x = val_x;
                player.pos_y = val_y;
            }
        }
        return 1;
    }
    return 0;
}

void timer_callback(void *data) {
    Draw *draw = static_cast<Draw *>(data);
    draw->update();
    Fl::repeat_timeout(0.01, timer_callback, data);
    tp2 = std::chrono::system_clock::now();
    duration = tp2 - tp1;
    tp1 = tp2;
}

int main(int argc, char **argv) {
    Fl_Window *window = new Fl_Window(1200, 800);
    Draw *draw = new Draw(0, 0, 1200, 800, "test");

    Fl::add_timeout(0.1, timer_callback, draw);
    window->end();
    window->show(argc, argv);
    return Fl::run();
}
