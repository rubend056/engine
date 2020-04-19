#ifndef Component_h
#define Component_h

// #include "file.h"

#include "cereal/archives/json.hpp"
#include "idraw.h"

// Max num of components of this type in an GObj (0 means unlimited)[default]
#define COMPONENT_MAX_NUM unsigned int max_num()
// Will the class be instantiated(false), or referenced(true)[default]
#define COMPONENT_IS_REF bool is_ref()
#define COMPONENT_SET_REF void set_ref(const std::shared_ptr<Component>& val)

#define COMPONENT_SERIALIZE cereal::make_nvp("component", cereal::base_class<Component>(this))

class Component : public IDraw{
	protected:
		// std::shared_ptr<Component> c_ref; // Link to our prefab inst 
    public:
        bool enabled = true;
		
		// Override this to set the ref for the prefab
		virtual COMPONENT_SET_REF{};
		
		// Will the class be instantiated(false), or referenced(true)[default]
		virtual COMPONENT_IS_REF{return true;}
        virtual COMPONENT_MAX_NUM {return 1;}
		virtual IDRAW_IMGUI_NAME override {return "Component";}
		virtual IDRAW_IMGUI_DRAW override {ImGui::Text ("imgui_draw is not set here");}
		
		virtual ~Component(){}
		virtual Component* clone(){throw("Trying to clone unclonable");};
		
		template<class Archive>
		void serialize(Archive& ar){
			ar(CEREAL_NVP(enabled));
		}
};

#endif