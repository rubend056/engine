#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "gameobject.h"

glm::mat4 Camera::get_matrix() {
	if (auto p = get_parent_go()) {
		// This assumes the camera's default direction is forwards, into the -z axis
		// View matrix
		auto rmat = p->trans->get_rot_mat();
		auto eye = p->trans->get_pos();
		auto center = glm::vec3(rmat * glm::vec4(0, 0, -1, 1)) + p->trans->get_pos();
		auto up = glm::vec3(0, 1, 0);
		auto vmat =
			glm::lookAt(eye, center, up);
		// Projection matrix
		auto pmat =
			cam_ortho ? glm::ortho(-cam_ortho_ratio, cam_ortho_ratio, cam_ortho_ratio, -cam_ortho_ratio, cam_near, cam_far) : glm::perspective(glm::radians(cam_fov), cam_ratio, cam_near, cam_far);

		// View matrix, then proj mat
		return pmat * vmat;
	} else
		return glm::mat4(1.f);
}
void Camera::imgui_draw() {
	ImGui::Checkbox("Ortho", &cam_ortho);
	ImGui::DragFloat("Near", &cam_near);
	ImGui::DragFloat("Far", &cam_far);
	ImGui::Separator();
	if (cam_ortho) {
		ImGui::TextDisabled("Ortho");
		ImGui::DragFloat("Ratio", &cam_ortho_ratio);
	} else {
		ImGui::TextDisabled("Perspective");
		ImGui::DragFloat("FOV", &cam_fov);
		ImGui::DragFloat("Ratio", &cam_ratio);
	}
}