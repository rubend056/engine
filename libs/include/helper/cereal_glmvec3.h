#ifndef cereal_glmvec3_h
#define cereal_glmvec3_h

#include "cereal/cereal.hpp"
namespace glm {
	// cereal::traits::EnableIf<cereal::traits::is_text_archive<Archive>::value> = cereal::traits::sfinae
// ? SAVING
template <class Archive>
void save(Archive& ar, vec3 const& vec) {
	ar(cereal::make_nvp("x", vec.x));
	ar(cereal::make_nvp("y", vec.y));
	ar(cereal::make_nvp("z", vec.z));
}
// ? LOADING
template <class Archive>
inline void load(Archive& ar, vec3& vec) {
	ar(cereal::make_nvp("x", vec.x));
	ar(cereal::make_nvp("y", vec.y));
	ar(cereal::make_nvp("z", vec.z));
}
}

#endif // cereal_glmvec3_h