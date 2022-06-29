#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>

#include "my_imgui.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

void Transform::imgui_draw() {
#define PUSH(name)                   \
	if (PREFAB_VAR_NAME_DIRTY(name)) \
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, .5, 0, 1));
#define POP(name)                    \
	if (PREFAB_VAR_NAME_DIRTY(name)) \
		ImGui::PopStyleColor();
	PUSH(pos);
	ImGui::DragFloat3("Position", &pos[0], .1f);
	POP(pos);
	PUSH(rot);
	ImGui::DragFloat3("Rotation", &rot[0], .1f);
	POP(rot);
	PUSH(sca);
	ImGui::DragFloat3("Scale", &sca[0], .1f);
	POP(sca);
	check_pos();
	check_rot();
	check_sca();
}

glm::mat4 Transform::get_matrix_trans() {
	auto pmat = get_pos_mat();
	auto rmat = get_rot_mat();
	auto smat = get_sca_mat();
	return smat * rmat * pmat;
}
glm::quat Transform::get_rot_quat() {
	return glm::quat(rot);
}
glm::mat4 Transform::get_pos_mat() {
	return glm::translate(glm::mat4(1.f), pos);
}
glm::mat4 Transform::get_rot_mat() {
	return glm::toMat4(get_rot_quat());
}
glm::mat4 Transform::get_sca_mat() {
	return glm::scale(glm::mat4(1.f), sca);
}
