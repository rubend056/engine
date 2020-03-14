#ifndef Camera_h
#define Camera_h

// #include "irenderable.h"
#include "gameobject.h"

class Camera : public Component{
    public:
        void render(float x,float y, float w, float h);
};

#endif