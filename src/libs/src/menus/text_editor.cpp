#include <fstream>
#include <streambuf>

#include "_menus.h"
#include "engine_globals.h"
#include "my_filesystem.h"

using namespace std;

namespace menus {

std::vector<FileText> text_files;
// static int current = 0;
void open_text_editor(fs::path path) {
	auto rel = engine::get_relative_to_project(path);
	// Return if path is already part of our
	for (auto& file : text_files) {
		if (file.path == rel) {
			file.open = true;
			return;
		}
	}

	auto full = engine::get_absolute_from_project(path);
	// File doesn't exist in ram, loading.. (creating new file instance and loading new info)
	ifstream stream(full);
	if (!stream.is_open()) {
		printf(ANSI(RED, "File %s couldn't be opened") ENDL, full.c_str());
		throw "File can't be opened";
	}
	FileText f;
	f.path = rel;
	f.data = string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	text_files.push_back(f);
}
void save_text_editor(FileText& file) {
	// cout << "Saving " << file.path << endl;
	ofstream stream(engine::get_absolute_from_project(file.path), ofstream::trunc);
	stream << file.data;
}
void text_editor() {
	for (auto& file : text_files) {
		if (!file.open)
			continue;

		ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_FirstUseEver);

		auto fname = file.path.filename().string();
		char name[fname.size() + 20];
		sprintf(name, "Editor <%s>", fname.c_str());
		ImGui::Begin(name, &file.open);

		if (ImGui::Button("Save")) {
			save_text_editor(file);
		}

		ImGui::InputTextMultiline("Editor", &file.data, ImVec2(-1, -1),
								  ImGuiInputTextFlags_AllowTabInput
								  //   ,File::resize_callback
		);

		ImGui::End();
	}
}
}  // namespace menus