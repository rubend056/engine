#ifndef idraw_h
#define idraw_h

#include <string>

#include "itype.h"
#include "my_imgui.h"

// The name of this class
#define IDRAW_IMGUI_NAME const std::string imgui_name()
#define IDRAW_IMGUI_DRAW void imgui_draw()

struct IDraw : virtual IType {
	virtual IDRAW_IMGUI_NAME { return "Name"; };
	virtual ITYPE { return "IDraw"; };
	virtual IDRAW_IMGUI_DRAW { ImGui::Text("Draw not implemented"); };
	virtual void imgui_draw_preview() { ImGui::Text("Preview not implemented"); };
};

#endif	// idraw_h