#include <fstream>
#include <streambuf>

#include "_menus.h"

#include "my_filesystem.h"

using namespace std;

namespace menus {

struct File {
    fs::path path;
    string data;
    bool open = true;
};
static vector<File> text_files;
static int current = 0;
void open_text_editor(fs::path full_path) {
    cout << "Opened " << endl;
    // Looking for file in our list
    for (auto& file : text_files) {
        if (file.path == full_path) {
            file.open = true;
            return;
        }
    }
    // File doesn't exist in ram, loading.. (creating new file instance and loading new info)
	ifstream stream(full_path);
    File f;
    f.path = full_path;
    f.data = string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    text_files.push_back(f);
}
void save_text_editor(File& file) {
    ofstream stream(file.path, ofstream::trunc);
    stream << file.data;
}
void text_editor() {
	// int i=0;
    for (auto& file : text_files) {
        if (!file.open) continue;

        ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_FirstUseEver);
		
		char name[file.path.filename().string().size()+1];
		sprintf(name, "Editor <%s>", file.path.filename().c_str());
        ImGui::Begin(name, &file.open);

        if (ImGui::Button("Save")) {
            save_text_editor(file);
        }

        ImGui::InputTextMultiline("", &file.data, ImVec2(-1, -1),
                                  ImGuiInputTextFlags_AllowTabInput
								//   ,File::resize_callback
								  );

        ImGui::End();
    }
}
}  // namespace menus