#ifndef cereal_glmvec3_h
#define cereal_glmvec3_h

#include "cereal/cereal.hpp"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat2x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"

namespace glm {
	// cereal::traits::EnableIf<cereal::traits::is_text_archive<Archive>::value> = cereal::traits::sfinae
// // ? SAVING
// template <class Archive>
// void save(Archive& ar, vec3 const& vec) {
// 	ar(cereal::make_nvp("x", vec.x));
// 	ar(cereal::make_nvp("y", vec.y));
// 	ar(cereal::make_nvp("z", vec.z));
// }
// // ? LOADING
// template <class Archive>
// inline void load(Archive& ar, vec3& vec) {
// 	ar(cereal::make_nvp("x", vec.x));
// 	ar(cereal::make_nvp("y", vec.y));
// 	ar(cereal::make_nvp("z", vec.z));
// }

template<class Archive>
void serialize(Archive & ar, vec2 & m){ar( m.x, m.y);}
template<class Archive>
void serialize(Archive & ar, vec3 & m){ar( m.x, m.y, m.z );}
template<class Archive>
void serialize(Archive & ar, vec4 & m){ar( m.x, m.y, m.z, m.w );}


template<class Archive>
void serialize(Archive & ar, mat2 & m){for(int i=0;i<2;++i)ar( m[i].x, m[i].y);}
template<class Archive>
void serialize(Archive & ar, mat3 & m){for(int i=0;i<3;++i)ar( m[i].x, m[i].y, m[i].z);}
template<class Archive>
void serialize(Archive & ar, mat4 & m){for(int i=0;i<4;++i)ar( m[i].x, m[i].y, m[i].z, m[i].w);}

}

#endif // cereal_glmvec3_h