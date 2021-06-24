/**
 * @file  imgui_helper.h
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines  imgui_helper
 * @version 0.1
 * 
 */
#ifndef imgui_helper_h
#define imgui_helper_h

#include <memory>

#include "imgui.h"
#include "referentiable.h"

void imgui_dnd_ref(std::shared_ptr<Referentiable>& obj) {
	auto refs = obj->my_ref();	// Get the refs to the current referentiable

	// Create a static data package and fill it
	unsigned int data[11];
	data[0] = refs.size() > 10 ? 0 : refs.size();
	int c = 0;
	for (auto& ref : refs) data[++c] = ref;

	ImGui::SetDragDropPayload("DND_REF", &data[0], sizeof(data));

	std::string r_string = "Refs: ";
	for (auto& ref : refs) {
		r_string += std::to_string(ref) + ' ';
	}

	ImGui::Text(
		refs.size() > 10 ? "Refs size too big?" : r_string.c_str());
}

#endif	//  imgui_helper_h