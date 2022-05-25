#include "imgui_helper.h"

// #include <iostream>

#include "cereal/archives/binary.hpp"
#include "cereal/types/vector.hpp"
// #include "engine_globals.h"

std::string cereal_sstream_serialize(std::function<void(cereal::BinaryOutputArchive&)> func) {
	std::stringstream ss;
	cereal::BinaryOutputArchive ar(ss);
	func(ar);
	return ss.str();
}
void cereal_sstream_deserialize(std::function<void(cereal::BinaryInputArchive&)> func, const char* arr, size_t size) {
	std::stringstream ss;
	for (int i = 0; i < size; ++i) ss.put(arr[i]);
	cereal::BinaryInputArchive ar(ss);
	func(ar);
}
void cereal_sstream_deserialize(std::function<void(cereal::BinaryInputArchive&)> func, std::string str) {
	cereal_sstream_deserialize(func, str.c_str(), str.size());
}

void imgui_dnd_ref(std::vector<uint>& refs, bool source, ImGuiDragDropFlags source_flags) {
	const auto payload_name = "DND_REF";
	if (source ? ImGui::BeginDragDropSource(source_flags) : ImGui::BeginDragDropTarget()) {
		if (source) {
			auto str = cereal_sstream_serialize([&](auto& ar) {
				ar(refs);
			});
			ImGui::SetDragDropPayload(payload_name, str.c_str(), str.size());

		} else {
			refs.clear();
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payload_name)) {
				// std::cout << "Data size " << payload->DataSize << ": " << (char*)payload->Data << std::endl;
				
				cereal_sstream_deserialize([&](auto& ar){
					ar(refs);
				}, (char*)payload->Data, payload->DataSize);

				// Print
				// for (auto& r : refs) {
				// 	std::cout << r;
				// }
				// std::cout << std::endl;
			}
		}
		// Set payload text
		ImGui::Text(payload_name);
		source ? ImGui::EndDragDropSource() : ImGui::EndDragDropTarget();

	} else if (!source)
		refs.clear();
}
