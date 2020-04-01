#ifndef Component_h
#define Component_h

// #include "serialize.h"

#include "cereal/archives/json.hpp"
#include "idraw.h"

// #define COMPONENT_NAME static const char* name()
#define COMPONENT_MAX_NUM unsigned int max_num()


class Component : public IDraw{
	protected:
		// static int get_next_comp_type(){
		// 	static int next_type=0;
		// 	return ++next_type;
		// }
		
    public:
		// std::string ref;// If set, it's a reference object, should only save this and dirty variables
		
		
        bool enabled = true;
        virtual COMPONENT_MAX_NUM {return 0;}
		virtual IDRAW_IMGUI_NAME override {return "Component";}
		virtual IDRAW_IMGUI_DRAW override {ImGui::Text ("imgui_draw is not set here");}
		// virtual void imgui_draw() = 0;
		// Component(){get_next_comp_type();}
		
		template<class Archive>
		void serialize(Archive& ar){
			ar(CEREAL_NVP(enabled));
		}
};

#endif