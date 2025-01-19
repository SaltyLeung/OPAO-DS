#pragma once
#include "milk.h"
#include "bubble.h"
#define FULL_HEALTH 10000
class game_scene {
public:
    game_scene();
    ~game_scene();
    void update(int frame_count);
    void on_input();
    void on_render();
    bool is_game_over = false;
    bool request_new = false;
private:
    void draw_ui(int f);
    void delete_unused();
    void hit_test();
    bubble* spawn_bubble(int pos_x);
    milk* spawn_milk(int pos_x, int pos_y);
    void activate_buff(int level);
    std::unordered_set<milk*> _milks;
    std::unordered_set<bubble*> _bubbles;
    bubble* _blowing_bubble;

    int _health = FULL_HEALTH;
    int _score = 0;
    bool _can_activate_buff = false;
    int _record_timer = -1;
    bool _opao_time_succeed = false;
    int vol_record = 0;
    bool pressing_LR = false;
};