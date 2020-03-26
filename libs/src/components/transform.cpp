#include "components/transform.h"

#include "imgui.h"

void Transform::imgui_draw(){
	ImGui::DragFloat3("Position",&pos[0]);
	ImGui::DragFloat3("Rotation",&rot[0]);
	ImGui::DragFloat3("Scale",&sca[0]);
}