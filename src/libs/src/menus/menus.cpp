#include <fstream>

#include "_menus.h"
#include "assets.h"
#include "cereal/archives/json.hpp"
#include "engine_globals.h"
#include "engine_scene.h"
#include "gl.h"
#include "helper.h"
#include "imgui_helper.h"
#include "imgui_internal.h"
#include "my_filesystem.h"
#include "my_imgui.h"

#define MENUS_CACHE_APATH engine::get_absolute_from_project("cache_menus.json")

static bool
	show_glEnable = false,
	show_metrics_window = false,
	show_stats = false,
	show_inspector = true,
	show_scene = true,
	show_files = false,
	show_assets = true;

#define NVP(x) CEREAL_NVP(x)
template <class Archive>
void menus_serialize(Archive& ar) {
	ar(
		NVP(show_glEnable),
		NVP(show_metrics_window),
		NVP(show_stats),
		NVP(show_inspector),
		NVP(show_scene),
		NVP(show_files),
		NVP(show_assets));
}

void menus::imgui_engine_init() {
	load_settings_glEnable();
	std::ifstream f(MENUS_CACHE_APATH);
	if (f.is_open()) {
		cereal::JSONInputArchive ar(f);
		menus_serialize(ar);
	}
}

void menus::imgui_engine_exit() {
	std::ofstream f(MENUS_CACHE_APATH);
	if (f.is_open()) {
		cereal::JSONOutputArchive ar(f);
		menus_serialize(ar);
	}
}
using namespace std;
typedef shared_ptr<Ref> Ref_shared;

bool menus::path_exists(const string& path) {
	return fs::directory_entry(engine::get_absolute_from_project(path)).exists() ||
		   std::any_of(assets::files.begin(), assets::files.end(), [&](const std::shared_ptr<File>& f) {
			   return path == f->_rel_path;
		   });
}

// Function to check if any valid nodes underneath are filter valid
bool valid_recurse(const Ref_shared& ref, const select_filter& f) {
	auto parent = dynamic_pointer_cast<Parent>(ref);
	auto _valid = f(ref);
	auto _valid_children = (parent && any_of(parent->children.begin(), parent->children.end(), [&f](const Ref_shared& c) { return valid_recurse(c, f); }));
	return _valid || _valid_children;
};

// typedef pair<Ref_shared, bool> recurse_return;
void select_recurse(
	const Ref_shared& obj,
	const select_filter& f,
	Ref_shared& selected,
	int i = 0,
	bool flat = false) {
	// If undefined return
	if (!obj)
		return;

	// auto filter_size = strlen(filter.InputBuf);
	auto draw = dynamic_pointer_cast<IDraw>(obj);
	auto parent = dynamic_pointer_cast<Parent>(obj);
	auto file = dynamic_pointer_cast<File>(obj);
	auto name = ((draw ? draw->imgui_name() : string("Object_no_idraw?"))
				 // + helper::string_format("###%i", i)
				 )
					.c_str();

	// Only draw ans setup drag and drop if valid
	if (valid_recurse(obj, f)) {
		if (parent && parent->children.size()) {
			if (ImGui::TreeNodeEx(name,
								  (flat ? ImGuiTreeNodeFlags_DefaultOpen : 0) |
									  (selected == obj ? ImGuiTreeNodeFlags_Selected : 0) |
									  ImGuiTreeNodeFlags_OpenOnArrow |
									  0)) {
				for (int e = 0; e < parent->children.size(); ++e)
					select_recurse(parent->children[e], f, selected, e, flat);
				ImGui::TreePop();
			}
		} else {
			ImGui::Text(name);
		}

		if (f(obj) && ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			selected = obj;
		}
		// Enable drag and drop for every item
		auto ref = obj->my_ref();
		imgui_dnd_ref(ref, true, ImGuiDragDropFlags_SourceAllowNullID);
	}

	return;
}

Ref_shared menus::select_asset(const char* name, select_filter select_filter, FactoryType_ create_types, MenuSelectFlags flags) {
	const bool modal = flags & MenuSelectFlags_Modal, new_only = flags & MenuSelectFlags_NewOnly;

	static Ref_shared selected;
	ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Appearing);

	bool done = false;
	static Ref_shared new_obj;
	static bool is_new_obj = false;
	static std::string new_rpath;

	static auto checkbox_disabled = [](const char* name, bool& checked, bool disabled) {
		if (disabled)
			ImGui::PushDisabled();
		ImGui::Checkbox(name, &checked);
		if (disabled)
			ImGui::PopDisabled();
	};

	if (modal ? ImGui::BeginPopupModal(name) : ImGui::BeginPopup(name)) {
		if (new_only) {
			is_new_obj = true;
		}
		// Making sure path doesn't exist
		bool new_rpath_exists = false;
		if (auto f = std::dynamic_pointer_cast<File>(new_obj)) {
			auto rpath = new_rpath + f->supposed_ext();
			new_rpath_exists = menus::path_exists(rpath);
		}
		// Allow ok
		bool allow_ok = is_new_obj ? (new_obj && (std::dynamic_pointer_cast<File>(new_obj) ? (new_rpath.size() && !new_rpath_exists) : true)) : (selected != 0);

		auto create_types_length = __builtin_popcount(create_types);

		//*** UI
		checkbox_disabled("Is new object", is_new_obj, new_only);

		if (is_new_obj) {
			if (create_types_length > 1) {
				// Selecting Type of item
				const int division = 3;
				// ImGui::PushItemWidth();
				const auto width = ImGui::GetContentRegionAvail().x / division - 5.f;
				int i = 0;
				for (auto& tname : factory::factory.get_names()) {
					bool tvalid = create_types & factory::factory.get_name_key(tname);
					if (tvalid) {
						if (i && i % division != 0)
							ImGui::SameLine();

						if (ImGui::Button(tname.c_str(), ImVec2(width, 0))) {
							new_obj = factory::factory.create(tname);
						}
						++i;
					}
				}
				// ImGui::PopItemWidth();
			} else if (create_types_length == 1) {
				if (!new_obj) {
					new_obj = factory::factory.create(create_types);
					if (!new_obj)
						throw "Tried setting new object but couldn't? is create_types_length wrong?";
				}
			}

			if (new_obj) {
				auto draw = dynamic_pointer_cast<IDraw>(new_obj);
				if (draw)
					ImGui::TextDisabled(helper::string_format("Creating new %s", draw->imgui_type_name()).c_str());
				auto file = dynamic_pointer_cast<File>(new_obj);
				if (file) {
					ImGui::InputText(file ? file->supposed_ext().c_str() : "rel_path", &new_rpath);
					if (new_rpath_exists)
						ImGui::TextColored(ImVec4(1.f, 0, 0, 1.f), "Path exists, change it");
				}
			}

		} else {
			static ImGuiTextFilter filter_text;
			filter_text.Draw("Filter", -1);
			auto filter = [&](const Ref_shared& ref) {
				auto draw = dynamic_pointer_cast<IDraw>(ref);
				auto file = dynamic_pointer_cast<File>(ref);
				return select_filter(ref) &&
					   ((draw && filter_text.PassFilter(draw->imgui_name().c_str())) ||
						(file && filter_text.PassFilter(file->data_path().c_str())));
			};
			for (auto& ref : assets::files) {
				select_recurse(ref, filter, selected, 0, strlen(filter_text.InputBuf));
			}
		}

		// BUTTONS
		if (!allow_ok)
			ImGui::PushDisabled();

		auto ok = ImGui::Button("Ok", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0));
		if (!allow_ok)
			ImGui::PopDisabled();

		if (allow_ok && (ok || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))) {
			done = true;
			ImGui::CloseCurrentPopup();
		}
		if (modal) {
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(-1.0, 0)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
				ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (done) {
		done = false;
		if (is_new_obj) {
			auto obj = new_obj;
			new_obj.reset();
			is_new_obj = false;

			if (auto file = dynamic_pointer_cast<File>(obj)) {
				file->set_rel_path(new_rpath);
			}
			return obj;
		} else {
			auto s = selected;
			selected.reset();
			return s;
		}
	}
	return Ref_shared();
}

void menus::imgui_engine_update() {
	if (show_glEnable)
		menus::window_glEnable_config(&show_glEnable);
	// if (show_open) menus::window_open_project(&show_open);
	if (show_metrics_window)
		ImGui::ShowMetricsWindow(&show_metrics_window);

	if (show_stats)
		menus::stats(&show_stats);
	if (show_inspector)
		menus::inspector(&show_inspector);
	if (show_scene)
		menus::scene();
	if (show_files)
		menus::files(&show_files);
	if (show_assets)
		menus::assets(&show_assets);

	menus::text_editor();

	// ? POPUP VARIABLES
#define POPUP_DEFINE(name) name = false,
#define POPUP_RESET(name) name =
#define POPUP_OPEN(name) \
	if (name)            \
		ImGui::OpenPopup(#name);
#define POPUP_MAIN_MENU_EXPANSION(FUNC) \
	FUNC(save_as_popup_modal)           \
	FUNC(open_scene_popup_modal)        \
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
			}
			if (ImGui::MenuItem("Reload Project")) {
				assets::reload_project();
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
			if (ImGui::BeginMenu("Assets")) {
				ImGui::MenuItem("Assets", NULL, &show_assets);
				ImGui::MenuItem("Files", NULL, &show_files);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Debug")) {
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
		auto scene = select_asset<Scene>("open_scene_popup_modal", FactoryType_Scene, MenuSelectFlags_Modal);
		if (scene)
			engine::scene = scene;
	}
	// static auto get_rel_path = []() {
	// };

	{
		ImGui::SetNextWindowSize(ImVec2(200, 0));
		if (ImGui::BeginPopupModal("save_as_popup_modal", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
			static std::string path;
			bool exists = true;
			if (engine::scene) {
				ImGui::InputText(engine::scene->supposed_ext().c_str(), &path);
				exists = menus::path_exists(path + engine::scene->supposed_ext());
				if (exists)
					ImGui::TextColored(ImVec4(1.f, 0, 0, 1.f), "Path exists");
			} else {
				ImGui::TextColored(ImVec4(1.f, 0, 0, 1.f), "Open a scene to be able to 'save as'");
			}

			if (ImGui::Button("Ok") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
				ImGui::CloseCurrentPopup();
				if (engine::scene)
					File::save_file(engine::scene);
			}

			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
	}

	{
		ImGui::SetNextWindowSize(ImVec2(200, 0));
		if (ImGui::BeginPopupModal("new_scene_popup", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
			static std::string scene_name;
			ImGui::InputText(".scene", &scene_name);

			if (ImGui::Button("Ok") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
				if (scene_name.size()) {
					auto scene = std::make_shared<Scene>(scene_name);
					assets::add(scene);
					engine::load_scene(scene);
				}
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 50);
			if (ImGui::Button("Cancel", ImVec2(50, 0)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}
	}
	ImGui::PopID();

	POPUP_MAIN_MENU_EXPANSION(POPUP_RESET)
	false;
}
