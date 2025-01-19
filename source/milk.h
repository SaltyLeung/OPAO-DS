#pragma once
#include "gameobject.h"

class milk : public gameobject {
public:
    milk();
    virtual ~milk();

    void update(int f) override;

    void on_render() override;

    gameobject * following = nullptr;
    int idx = 0;
    static int milk_count;
private:
    float v = 0.0f;
};