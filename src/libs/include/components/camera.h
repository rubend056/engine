#ifndef camera_h
#define camera_h

#include <glm/mat4x4.hpp>

#include "cereal/types/polymorphic.hpp"
#include "component.h"

class Camera : public Component {
	bool cam_ortho = true;
	float cam_fov = 70.f, cam_ratio = 4.f / 3.f, cam_near = .1f, cam_far = 100.f;
	float cam_ortho_ratio = 10;

   public:
	glm::mat4 get_matrix();
	IDRAW_IMGUI_DRAW override;
	IDRAW_IMGUI_NAME override { return "Camera"; }

	COMPONENT_IS_REF override { return false; }

	template <class Archive>
	void serialize(Archive& ar) {
		ar(COMPONENT_SERIALIZE);
		ar(
			CEREAL_NVP(cam_ortho),
			CEREAL_NVP(cam_fov),
			CEREAL_NVP(cam_ratio),
			CEREAL_NVP(cam_near),
			CEREAL_NVP(cam_far),
			CEREAL_NVP(cam_ortho_ratio));
	}
};
CEREAL_REGISTER_TYPE(Camera)
// CEREAL_REGISTER_TYPE(Camera)
// namespace cereal { namespace detail { template <> struct binding_name<Camera> { CEREAL_STATIC_CONSTEXPR char const * name() { return "Camera"; } }; } } CEREAL_BIND_TO_ARCHIVES(Camera)

#endif	// camera_h