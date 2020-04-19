#include "_menus.h"

#include "gl.h"
#include "my_imgui.h"
#include "cereal/archives/json.hpp"
#include "engine_globals.h"
#include "assets.h"

#include <fstream>

#define ENGINE_INI engine::get_absolute_from_project("cache_menus.json")

static bool
        show_glEnable = false,
        // show_open = false,
        show_metrics_window = true,
        show_stats = false,
        show_inspector = true,
		show_scene = true,
		// show_textures = true,
		// show_programs = true,
        show_files = true,
		show_assets = true;

void menus::imgui_engine_init(){
	load_settings_glEnable();
	
	{
		std::ifstream f(ENGINE_INI);
		if(f){
			cereal::JSONInputArchive ar(f);
			// fs::path p;
			// ar(p);
			// if(!p.empty()){
			// 	menus::inspector_o = assets::get_file<IDraw>(p);
			// }
			ar( show_glEnable,
				// show_open = false,
				show_metrics_window,
				show_stats,
				show_inspector,
				show_scene,
				// show_textures,
				// show_programs,
				show_files,
				show_assets);
		}
		
	}
}

std::shared_ptr<File> menus::add_popup(const std::vector<std::shared_ptr<File>>& vector, bool modal, const char*label) {
	// static_assert(std::is_base_of<File, T>::value, "T not derived from File");

	if (modal?ImGui::BeginPopupModal(label):ImGui::BeginPopup(label)) {
		for (auto& v : vector) {
			// if(!draw_inst) ImGui::Text("%s ")
			if (ImGui::Button(v->filename().c_str())) {
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return v;
			}
		}
		if(modal && ImGui::Button("Cancel"))ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
	return std::shared_ptr<File>();
}

void menus::imgui_engine_update() {
    
    if (show_glEnable) menus::window_glEnable_config(&show_glEnable);
    // if (show_open) menus::window_open_project(&show_open);
    if (show_metrics_window) ImGui::ShowMetricsWindow(&show_metrics_window);

    if (show_stats) menus::stats(&show_stats);
    if (show_inspector) menus::inspector(&show_inspector);
	if (show_scene) menus::scene();
    if (show_files) menus::files(&show_files);
	if (show_assets) menus::assets(&show_assets);

    menus::text_editor();
	
	// ? POPUP VARIABLES
#define POPUP_DEFINE(name) name=false,
#define POPUP_RESET(name) name=
#define POPUP_OPEN(name) if(name)ImGui::OpenPopup(#name);
#define POPUP_MAIN_MENU_EXPANSION(FUNC)	\
	FUNC(save_as_popup_modal)			\
	FUNC(open_scene_popup_modal)		\
	FUNC(new_scene_popup)
	
	static std::vector<std::shared_ptr<File>> scenes;
	bool POPUP_MAIN_MENU_EXPANSION(POPUP_DEFINE) _PuP;
	// ? #################
	
	// ? MENU
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
			
			if (ImGui::MenuItem("New Scene")) {
				new_scene_popup = true;
            }
			
			// Open Scene menu
            if (ImGui::MenuItem("Open Scene")) {
				open_scene_popup_modal = true;
				#warning "Scene class used here"
				scenes = assets::get_files("Scene");
            }
			// ############
			
            if (ImGui::MenuItem("Save as"))
                save_as_popup_modal = true;

            

            if (ImGui::MenuItem("Quit")) {
                engine::run = false;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings")) {
            if (ImGui::BeginMenu("OpenGL")) {
                ImGui::MenuItem("glEnable", NULL, &show_glEnable);
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
			if(ImGui::BeginMenu("Assets")){
				ImGui::MenuItem("Files", NULL, &show_files);
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Debug")){
				ImGui::MenuItem("Metrics", NULL, &show_metrics_window);
            	ImGui::MenuItem("Stats", NULL, &show_stats);
				ImGui::EndMenu();
			}
            ImGui::MenuItem("Inspector", NULL, &show_inspector);
            
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
	// ? #############
	
	// ? POPUP AGAIN
	ImGui::PushID("popup_id");
	POPUP_MAIN_MENU_EXPANSION(POPUP_OPEN)
	
	
	// ? Open scene popup
	{
		ImGui::SetNextWindowSize(ImVec2(200,0));
		auto scene = add_popup(scenes, true, "open_scene_popup_modal");
		if(scene) engine::scene = std::dynamic_pointer_cast<Scene>(scene);
	}
	
	{
		ImGui::SetNextWindowSize(ImVec2(200,0));
		if (ImGui::BeginPopupModal("save_as_popup_modal", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
			ImGui::Text("Save as modal");
			if (ImGui::Button("Ok")) ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
	}
	
	
	{
		ImGui::SetNextWindowSize(ImVec2(200,0));
		if(ImGui::BeginPopupModal("new_scene_popup", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)){
			static std::string scene_name;
			ImGui::InputText(".scene", &scene_name);
			if(ImGui::Button("Ok")){
				auto scene = std::make_shared<Scene>(scene_name);
				scene->create_supposed_ext();
				assets::add(scene, typeid(Scene).name());
				engine::load_scene(scene);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 50);
			if(ImGui::Button("Cancel",ImVec2(50,0)))ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
	}
	ImGui::PopID();
	
	POPUP_MAIN_MENU_EXPANSION(POPUP_RESET) false;
}

void menus::imgui_engine_exit(){
	// if(menus::inspector_o){
		
		std::ofstream f(ENGINE_INI);
		if(f){
			cereal::JSONOutputArchive ar(f);
			// auto ins_file = std::dynamic_pointer_cast<File>(menus::inspector_o);
			// if(ins_file)ar(ins_file->data_path());
			ar( show_glEnable,
				// show_open = false,
				show_metrics_window,
				show_stats,
				show_inspector,
				show_scene,
				// show_textures,
				// show_programs,
				show_files,
				show_assets);
		}
	// }
}

// void window_open_project(bool *p_open) {
//     ImGui::BeginPopup("Open Folder");
//     ImGui::EndPopup();
// }