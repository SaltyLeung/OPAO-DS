#pragma once
#include "gameobject.h"

class bubble : public gameobject {
public:
    bubble();
    void update(int frame_count) override;
    void activate();
    bool blow();
    void on_render() override;
    bool is_crash = false;
    bool is_followed = false;
    static float BUBBLE_BLOW_ADDON;
private:
    bool _is_activate = false;
};