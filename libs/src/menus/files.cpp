#include "menus.h"

#include <filesystem>
namespace fs = std::filesystem;

using namespace std;




namespace menus{
    // struct File{
    //     bool is_dir;
    //     fs::path path;
    //     string parent;
    //     string parent_stem;
    //     string filename;
    //     string extension;
    //     File(bool _is_dir, fs::path _p, string p, string ps, string f, string e)
    //     :is_dir(_is_dir),path(_p),parent(p),parent_stem(ps),filename(f),extension(e){};
    // };

    static vector<fs::directory_entry> fl;
    void list_dir(string d){
        
        for (const auto &entry : fs::directory_iterator(d)){
            // auto f = File(
            //     entry.is_directory(),
            //     entry.path(),
            //     entry.path().parent_path().string(),
            //     entry.path().parent_path().stem().string(), 
            //     entry.path().stem().string(),
            //     entry.path().extension().string()
            // );
            fl.push_back(entry);
            if (entry.is_directory()){list_dir(entry.path().string());}
        }
    }
    // void draw_dir(int i){

    // }
    void files(string root){
        ImGui::Begin("Files");

        //Find files
        
        if(fl.size() == 0){
            
            list_dir(root);
        }
            
        // Filter code
        static ImGuiTextFilter filter;
        filter.Draw();

        // vector<File*> f_files;
        // string last_parent = root;
        // for (int i = 0; i < fl.size(); i++){
        //     if (fl[i].is_directory() || filter.PassFilter(fl[i].path().stem().c_str())){
        //         if(fl[i].is_directory())
        //             ImGui::TreeNode(fl[i].path().stem().string().c_str());
        //         else 
        //             ImGui::Text("%s", 
        //                 fl[i].path().filename().string().c_str());

        //     }
        // }
        for (int i = 0; i < fl.size(); i++){
            if (!fl[i].is_directory() && filter.PassFilter(fl[i].path().filename().c_str())){
                ImGui::Text("%s", fl[i].path().filename().string().c_str());
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                   menus::open_text_editor(fl[i].path());
                }
            }
        }
        

        ImGui::End();
    }
}

