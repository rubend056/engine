#ifndef idraw_h
#define idraw_h

#include "imgui.h"

#define IDRAW_IMGUI_NAME const char* imgui_name()
#define IDRAW_IMGUI_DRAW void imgui_draw()

struct IDraw{
	virtual IDRAW_IMGUI_NAME=0;
    virtual IDRAW_IMGUI_DRAW{ImGui::Text("Draw not implemented");};
	virtual void imgui_draw_preview(){ImGui::Text("Preview not implemented");};
};

#endif // idraw_h