#include "transform.h"

#include "my_imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

void Transform::imgui_draw(){
	ImGui::DragFloat3("Position",&pos[0],.1f);
	ImGui::DragFloat3("Rotation",&rot[0],.1f);
	ImGui::DragFloat3("Scale",&sca[0],.1f);
}

glm::mat4 Transform::get_matrix_trans(){
	auto pmat = get_pos_mat();
	auto rmat = get_rot_mat();
	auto smat = get_sca_mat();
	return smat * rmat * pmat;
}
glm::quat Transform::get_rot_quat(){
	return glm::quat(rot);
}
glm::mat4 Transform::get_pos_mat(){
	return glm::translate(glm::mat4(1.f), pos);
}
glm::mat4 Transform::get_rot_mat(){
	return glm::toMat4(get_rot_quat());
}
glm::mat4 Transform::get_sca_mat(){
	return glm::scale(glm::mat4(1.f), sca);
}
