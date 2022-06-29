/**
 * @file engine.cpp
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Engine Editor initialization and loop
 * @version 0.1
 * 
 * Loads assets from project/game folder
 * Initializes OpenGL context and window
 * Runs engine loop
 */

// Cereal
#include "cereal/archives/json.hpp"

// Ours
#include "_engine.h"
#include "assets.h"
#include "engine_scene.h"
#include "menus.h"
// #include "helper.h"
#include "components.h"
#include "gl.h"
#include "mesh.h"
#include "vao.h"

// #include <SDL.h>
// #include <algorithm>

using namespace std;

fs::path relativePath(const fs::path &path, const fs::path &relative_to) {
	// create absolute paths
	fs::path p = fs::absolute(path);
	fs::path r = fs::absolute(relative_to);

	// if root paths are different, return absolute path
	if (p.root_path() != r.root_path())
		return p;

	// initialize relative path
	fs::path result;

	// find out where the two paths diverge
	fs::path::const_iterator itr_path = p.begin();
	fs::path::const_iterator itr_relative_to = r.begin();
	while (itr_path != p.end() && itr_relative_to != r.end() && *itr_path == *itr_relative_to) {
		++itr_path;
		++itr_relative_to;
	}

	// add "../" for each remaining token in relative_to
	if (itr_relative_to != r.end()) {
		++itr_relative_to;
		while (itr_relative_to != r.end()) {
			result /= "..";
			++itr_relative_to;
		}
	}

	// add remaining path
	while (itr_path != p.end()) {
		result /= *itr_path;
		++itr_path;
	}

	return result;
}

namespace engine {

bool run = true;
double deltaTime = 0;
double time = 0;

std::shared_ptr<Scene> scene;
// std::vector<std::shared_ptr<GameObject>> selected;
fs::path project_path;

void check_project_path() {
	if (project_path.empty() || project_path.is_relative())
		throw "Project path is empty or relative";
}
fs::path &normalize_absolute(fs::path &p) {
	check_project_path();
	// Make sure asset path is always absolute
	if (p.is_relative())
		p = project_path / p;
	return p;
}
bool is_within_project(const fs::path &full_path) {
	return full_path.string().find(project_path.string()) == 0;
}

bool is_relative_to_project(const fs::path &asset_path) {
	if (asset_path.is_relative())
		return true;
	auto p = asset_path;
	normalize_absolute(p);
	return is_within_project(p);
}
fs::path get_absolute_from_project(const fs::path &asset_path) {
	check_project_path();
	if (asset_path.is_relative())
		return project_path / asset_path;
	else
		assert(is_relative_to_project(asset_path));
	return asset_path;
}
fs::path get_relative_to_project(const fs::path &asset_path) {
	check_project_path();
	if (asset_path.is_absolute()) {
		assert(is_within_project(asset_path));
		return relativePath(asset_path, project_path);
	}
	return asset_path;
}

#define ENGINE_CACHE_PATH engine::get_absolute_from_project("cache.json")
void save_engine_state() {
	if (!scene)
		return;

	std::ofstream f(ENGINE_CACHE_PATH);
	cereal::JSONOutputArchive ar(f);

	// Saving Scene
	ar(cereal::make_nvp("scene", scene->hash_path()));
}
void load_scene(const std::shared_ptr<Scene> &_scene) {
	save_engine_state();
	scene = _scene;
}
}  // namespace engine

#include "source.h"

ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);
namespace engine {
void init() {
	assets::init();

	if (fs::exists(ENGINE_CACHE_PATH)) {
		std::ifstream f(ENGINE_CACHE_PATH);
		cereal::JSONInputArchive ar(f);

		// Loading last scene
		std::string last_scene;
		ar(cereal::make_nvp("scene", last_scene));
		printf("Loading last scene %s\n", last_scene.c_str());
		auto scene = assets::get_load_file<Scene>(last_scene);
		if (scene)
			engine::load_scene(scene);

		// Loading last inspector
		// std::vector<fs::path> ins_paths;
		// ar(cereal::make_nvp("inspector", ins_paths));
		// if(auto draw = std::dynamic_pointer_cast<IDraw>(assets::data_path_find(ins_paths)))
		// 	menus::inspector_o.insert(draw);
	}

	menus::imgui_engine_init();

	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
}

void update() {
	menus::imgui_engine_update();
	assets::update();
}
// void for_gameobjects(const std::shared_ptr<GameObject>&go,
// 		const std::function<void(const std::shared_ptr<GameObject>&go)>&f){
// 	for(auto&child:go->children){}
// }
// #include <stdio.h>
void render() {
	// Rendering
	if (engine::scene) {
		// Get all GameObjects in scene
		auto gobjects = helper::dynamic_pointer_cast_array<GameObject>(engine::scene->children);

		// Get all cameras in scene
		std::vector<std::shared_ptr<Camera>> cameras;
		for (auto &go : gobjects) {
			auto _cameras = go->get_comps<Camera>();
			cameras.insert(cameras.end(), _cameras.begin(), _cameras.end());
		}

		// For all GameObjects in scene
		for (auto &go : gobjects) {
			// Get all Programs in GameObject
			auto progs = go->get_comps<Program>();
			if (!progs.size())
				continue;  // If there are no Programs, skip object

			// Get all mesh VAO's in GameObject
			auto vaos = go->get_comps<VAO>();
			if (!vaos.size())
				continue;  // If there are no Mesh VAO's, skip object

			// Get GameObject's Transform Matrix
			auto go_trans_matrix = go->trans->get_matrix_trans();

			for (auto &prog : progs) {
				if (!prog->link_status)
					continue;  // If link status of program false, skip program

				// 1. Use program
				prog->use();
				for (auto &vao : vaos) {
					if (!vao->parent)
						throw(std::string("VAO has no parent???"));
					// 2. Bind VBO
					dynamic_cast<Mesh *>(vao->parent)->vbo_bind();
					// 3. Bind VAO
					vao->vao_bind();
					vao->vao_attrib_enable(0xff);

					for (auto &cam : cameras) {
						auto pmat = cam->get_matrix();
						prog->set_pmat(pmat * go_trans_matrix);
						vao->gl_draw();
					}
					// Backup rendering in case there are no cameras
					if (cameras.size() == 0) {
						prog->set_pmat(glm::mat4(1));
						vao->gl_draw();
					}
				}
			}
		}
	}
}

void exit() {
	menus::imgui_engine_exit();
	save_engine_state();
	assets::exit();
}
}  // namespace engine
