#include "game_scene.h"
#include "micrecord.h"
#include "sound_manage.h"

#define RECORD_TIME 150
milk* game_scene::spawn_milk(int pos_x, int pos_y) {
    //iprintf("milk, health: %d\n", _health);
    milk* m = new milk();
    m->logic_x = pos_x;
    m->logic_y = pos_y;
    m->rotation[0] = rand() % 30;
    m->rotation[1] = rand() % 30;
    _milks.insert(m);
    return m;
}

bubble* game_scene::spawn_bubble(int click_x) {
    int logic_x = click_x - 128;
    //iprintf("spawn_bubble %d\n", logic_x);
    bubble* b = new bubble();
    b->logic_x = logic_x;
    b->logic_y = -160;
    _bubbles.insert(b);
    return b;
}

game_scene::game_scene() {
    micrecordInit();
    play_music();
}

void game_scene::activate_buff(int vol) {
    //iprintf("O-PAO TIME!! %d\n", vol);
    _can_activate_buff = false;
    //if (vol > 8000)
        _health = FULL_HEALTH;
    bubble::BUBBLE_BLOW_ADDON += 1.0f;
    //play();
    play_update_effect();
    //iprintf("h:%d, s: %d\n", _health, _score);
}

void game_scene::delete_unused() {
    for (auto m : _milks) {
        if (m->logic_y < -200 || m->logic_y > 200) {
            milk* obj = m;
            _milks.erase(m);
            if (obj->following) {
                _bubbles.erase((bubble*)(obj->following));
                delete (bubble*)(obj->following);
            }
            if (obj->logic_y < 0) {
                _health -= 1;
            }
            if (obj->idx == 1) {
                _health -= 1;
            }
            if (_health < 0) {
                _health = 0;
            }
            //iprintf("milk logic xy: %d, %d\n", obj->logic_x, obj->logic_y);
            delete obj;
            break;
        }
    }
    for (auto it : _bubbles) {
        bubble* m = it;
        if (m->logic_y < -200 || m->logic_y > 200 || m->is_crash) {
            _bubbles.erase(m);
            if (!m->is_followed) {
                //delete m;
            }
            //iprintf("bubble logic xy: %d, %d\n", m->logic_x, m->logic_y);
            break; 
        }
    }
}

void game_scene::hit_test() {
    for (auto& b : _bubbles) {
        if (b->is_followed || b->scale[0] < 0.2f) {
            continue;
        }
        int test_bound = b->scale[0] * 45;
        //iprintf("%d\n",test_bound);
        for (auto& m : _milks) {
            if (abs(m->logic_y - b->logic_y) < 3 && abs(m->logic_x - b->logic_x) < test_bound) {
                m->following = b;
                b->is_followed = true;
                _score += 10;
                if (m->idx == 1) {
                    _score += 10;
                }
                play_hit_effect();
                if (_score % 50 == 0) {
                    _can_activate_buff = true;
                    //iprintf("Press L/R and shout O-PAO!!\n");
                }
                //iprintf("h:%d, s: %d\n", _health, _score);
            }
        }
    }
    if (_health <= 0) {
        is_game_over = true;
    }
}

game_scene::~game_scene() {
    michandler_callback = nullptr;
    while (!_milks.empty()) {
        auto m = _milks.begin();
        milk* obj = *m;
        _milks.erase(m);
        delete obj;
    }
    while (!_bubbles.empty()) {
        auto m = _bubbles.begin();
        bubble* obj = *m;
        _bubbles.erase(m);
        delete obj;
    }
}

void game_scene::update(int frame_count) {
    if (!is_game_over) {
        if (!michandler_callback) {
            static int lock = 0;
            michandler_callback = [this](int16_t vol) {
                //iprintf("vol%d\n", vol);
                lock++;
                if (_record_timer < 0) {
                    return;
                }
	            if (lock % 5 == 0 && abs(vol) > 6000) {
                    _opao_time_succeed = true;
                    //_can_activate_buff = false;
                    vol_record = vol;
                    //this->activate_buff(vol);
                }
            };
        }
        if (_record_timer > 0) {
            --_record_timer;
            if (_record_timer == 0) {
                _opao_time_succeed = true;
                if (_opao_time_succeed) {
                    activate_buff(vol_record);
                }
                _record_timer = -1;
                vol_record = 0;
                _opao_time_succeed = false;
            }
        }
        int space = 300;// - frame_count / 10;
        if (space < 50) space = 50;
        if (frame_count % space == 0 || frame_count == 1) {
            spawn_milk(-120 + rand() % 250, 160 - rand() % 10);
        }
        hit_test();
    }

    for (auto& m : _milks) {
        m->update(frame_count);
    }
    for (auto& b : _bubbles) {
        b->update(frame_count);
    }
    delete_unused();
    on_input();
    draw_ui(frame_count);
    on_render();
}

void game_scene::on_input() {
    //如果输入
    scanKeys();

	touchPosition touch;
	touchRead(&touch);

	int pressed = keysDown();	// buttons pressed this loop
	int held = keysHeld();		// buttons currently held
    pressing_LR = pressed & KEY_L || pressed & KEY_R;
    if (!(held & KEY_L) && !(held & KEY_R)) {
        _record_timer = -1;
    }
    if (pressed & KEY_A) {
        record();
        //iprintf("record\n");
    }
    if (pressed & KEY_B) {
        play(); 
        //iprintf("play\n");
    }
    if (pressed & KEY_UP) {
        _health += 10;
        //iprintf("play\n");
    }
    if (pressed & KEY_DOWN) {
        _health -= 10;
        //iprintf("play\n");
    }
    if (is_game_over) {
        if (pressed & KEY_A || pressed & KEY_START || pressed & KEY_B
         || pressed & KEY_X  || pressed & KEY_Y) {
            request_new = true;
        }
        return;
    }

    if (held & KEY_TOUCH) {
        if (!_blowing_bubble) {
            _blowing_bubble = spawn_bubble(touch.px);
        } else {
            if(!_blowing_bubble->blow()) {
                _bubbles.erase(_blowing_bubble);
                _blowing_bubble->is_crash = true;
                //play_hit_effect();
            }
        }
    } else {
        if (_blowing_bubble && !_blowing_bubble->is_crash) {
            _blowing_bubble->activate();
        }
        _blowing_bubble = nullptr;
    }
    if (_can_activate_buff && (pressing_LR)) {
        //iprintf("recoring\n");
        _record_timer = RECORD_TIME;
        //record();
    }
}


void game_scene::on_render() {
    for (auto& m : _milks) {
        m->do_render();
    }
    for (auto& b : _bubbles) {
        b->do_render();
    }
}

void game_scene::draw_ui(int frame_count) {
    if (is_game_over) {
		printf("\x1b[7;2HThe BOY got too many O-PAO!\n");
	    printf("\x1b[12;10HGAME OVER.\n");
		printf("\x1b[13;11Hscore: %d\n", _score);
    } else {
	    printf("\x1b[1;1HHP:%d        ",_health);
	    printf("\x1b[1;22HScore:%d\n", _score);
		//printf("\x1b[13;14Hmilkcount: %d\n", milk::milk_count);
	    //printf("\x1b[2;1H Don't let the BOY get O-PAO!");
        if (_can_activate_buff &&  frame_count % 3 != 0 && _record_timer == -1) {
	         printf("\x1b[20;1HPress L/R and shout\n");
	         printf("\x1b[21;1HWO YAO O-PAO!!!\n");
        } else if(_record_timer > 0) {
	         printf("\x1b[21;1HShout it!!!!!!\n");
        } 
        else {
            printf("\x1b[21;1H                           \n");
        }
        if (_opao_time_succeed = true && frame_count % 2 == 0) {
	    	//printf("\x1b[14;4HO-PAO TIME!!!\n");
        }
    }
}