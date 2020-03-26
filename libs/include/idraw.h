#ifndef idraw_h
#define idraw_h

#include "imgui.h"

struct IDraw{
	virtual const char* imgui_name()=0;
    virtual void imgui_draw() = 0;
};

#endif // idraw_h