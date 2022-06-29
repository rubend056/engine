#ifndef Component_h
#define Component_h

// #include "file.h"

#include "cereal/cereal.hpp"
#include "idraw.h"
#include "referentiable.h"

// Max num of components of this type in an GObj (0 means unlimited)[default]
#define COMPONENT_MAX_NUM unsigned int max_num()
// Will the class be instantiated(false), or referenced(true)[default]
#define COMPONENT_IS_REF bool is_ref()
#define COMPONENT_SET_REF void set_ref(const std::shared_ptr<Component>& val)

#define COMPONENT_SERIALIZE cereal::make_nvp("Component", cereal::base_class<Component>(this))

class GameObject;

class Component : public IDraw, public virtual Referentiable {
   private:
	parent_type last_parent = nullptr;
	GameObject* last_go = nullptr;

   protected:
   public:
	bool enabled = true;
	// int max = 1;

	GameObject* get_parent_go();

	// Override this to set the ref for the prefab
	virtual COMPONENT_SET_REF{};

	// Will the class be instantiated(false), or referenced(true)[default]
	virtual COMPONENT_IS_REF { return true; }
	virtual COMPONENT_MAX_NUM { return 1; }
	virtual IDRAW_IMGUI_NAME override { return type(); }
	virtual ITYPE override { return "Component"; };
	virtual IDRAW_IMGUI_DRAW override { ImGui::Text("imgui_draw is not set here"); }

	virtual ~Component() {}
	virtual std::shared_ptr<Component> clone() { throw(std::string("Trying to clone unclonable")); };

	template <class Archive>
	void serialize(Archive& ar) {
		ar(REFERENTIABLE_SERIALIZE);
		ar(CEREAL_NVP(enabled));
	}
};

#endif