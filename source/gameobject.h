#pragma once
#include <vector>
#include <stdlib.h>
#include <malloc.h>
#include <nds.h>
#include <stdio.h>
#include <unordered_set>

class gameobject {
public:
    virtual ~gameobject() {}
    int pos[3];
    float scale[3];
    float rotation[3];
    int logic_x = 0;
    int logic_y = 0;

    gameobject() {
        pos[0] = 0;
        pos[1] = 0;
        pos[2] = 0;
        scale[0] = 1.0f;
        scale[1] = 1.0f;
        scale[2] = 1.0f;
        rotation[0] = .0f;
        rotation[1] = .0f;
        rotation[2] = .0f;
    }

    virtual void init() {}
    virtual void update(int f) {}
    void do_render();
protected:
    virtual void on_render() {}
};