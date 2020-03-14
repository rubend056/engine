#include "menus/menus.h"

#include "misc/cpp/imgui_stdlib.h"
#include <filesystem>
#include <fstream>
#include <streambuf>

namespace fs = std::filesystem;

using namespace std;


namespace menus{
    
    struct File{
        fs::path path;
        string data;
        bool open = true;
        static int resize_callback(ImGuiInputTextCallbackData* data){
            cout << data->EventFlag << endl;
            return 0;
        }
    };
    static vector<File> text_files;
    static int current=0;
    void open_text_editor(fs::path path){
        cout << "Opened " << endl;
        for(auto&file : text_files){
            if(file.path == path){file.open=true; return;}
        }
        // File doesn't exist in ram, loading..
        File f;f.path = path;
        ifstream stream(path);
        f.data = string(std::istreambuf_iterator<char>(stream),std::istreambuf_iterator<char>());
        text_files.push_back(f);
    }
    void save_text_editor(File& file){
        ofstream stream(file.path, ofstream::trunc);
        stream << file.data;
    }
    void text_editor(){
        for(auto&file:text_files){
            if(!file.open)continue;
            // if(files.size() > 1)ImGui::BeginTabBar("Text Editor");
            // else 
            ImGui::SetNextWindowSize(ImVec2(200,300), ImGuiCond_FirstUseEver);
            ImGui:: Begin(file.path.filename().string().c_str(),&file.open);
            
            // for(auto&file: text_files){
                // ImGui::BeginTabItem(file.path.filename().c_str());
                
                // ImGui::Text();
                // ImGui::SameLine();
                if(ImGui::Button("Save")){
                    save_text_editor(file);
                }
                // static ImGuiInputTextFlags flags = ;
                ImGui::InputTextMultiline("",&file.data,ImVec2(-1,-1),
                    ImGuiInputTextFlags_AllowTabInput,
                    File::resize_callback
                );
                // ImGui::EndTabItem();
            // }
            
            // if(files.size() > 1)ImGui::EndTabBar();
            // else 
            ImGui::End();
        }
    }
}