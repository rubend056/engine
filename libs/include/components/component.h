#ifndef Component_h
#define Component_h

// #include "serialize.h"

#include "idraw.h"

#define COMPONENT_NAME static const char* name()
#define COMPONENT_IMGUI_NAME const char* imgui_name()
#define COMPONENT_IMGUI_DRAW void imgui_draw()
#define COMPONENT_MAX_NUM unsigned int max_num()

class Component : public IDraw{
	protected:
		// static int get_next_comp_type(){
		// 	static int next_type=0;
		// 	return ++next_type;
		// }
		
    public:
        bool enabled = true;
        virtual COMPONENT_MAX_NUM {return 0;};
		virtual COMPONENT_IMGUI_NAME override {return "Component";}
		virtual COMPONENT_IMGUI_DRAW override {ImGui::Text ("imgui_draw is not set here");}
		// virtual void imgui_draw() = 0;
		// Component(){get_next_comp_type();}
		
};

#endif