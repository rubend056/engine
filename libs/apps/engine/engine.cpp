#include "_engine.h"

#include "menus.h"
#include "assets.h"
#include "helper.h"
#include "components.h"

#include "gl.h"

#include <SDL.h>

#include <algorithm>

using namespace std;

namespace engine{
    bool run = true;
    std::shared_ptr<Scene> scene;
    // std::vector<std::shared_ptr<GameObject>> selected;
    fs::path project_path;
	fs::path get_absolute_from_project(const fs::path &asset_path){
		// cout << "Asset path: " << fs::absolute(asset_path) << endl;
		// cout << "Project path: " << project_path << endl;
		// Make sure asset path is relative
		assert(asset_path.is_relative());
		// Make sure asset path is inside project
		// assert(fs::absolute(asset_path).string().find(project_path.string()) != std::string::npos);
		auto pp = project_path.string();
		return fs::path(pp + "/" + asset_path.string());
	}
	fs::path get_relative_to_project(const fs::path &asset_path){
		// Make sure asset path is inside project
		assert(fs::absolute(asset_path).string().find(project_path.string()) != std::string::npos);
		return asset_path.lexically_relative(project_path);
	}
	void save_scene(){
		if(scene)
			File::save_file(scene);
	}
	void load_scene(const std::shared_ptr<Scene>& _scene){
		save_scene();
		scene = _scene;
	}
}


ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);
namespace engine
{
    void init(){
        menus::imgui_engine_init();
		assets::init();
        
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    }

    void update(){
		menus::imgui_engine_update();
		assets::update();
    }
    void render(){
		
        // Rendering
        
        
        // testing_draw();
        // RENDER
        
        // auto pred = [](Mesh*&m) -> bool{return string(m->filename).compare("testmesh") == 0;};
        // auto mesh = *find_if(assets::meshes.begin(), assets::meshes.end(), pred);
        
		
		// if(assets::textures.size()){
		// 	auto &texture = assets::textures[0];
		// 	glBindTexture(GL_TEXTURE_2D, texture->t_id);
		// }
		
		// for(auto&go:engine::objects){
		// 	helper::find_type_in_vector<Program>(go->comps);
		// }
		
        // auto& program = assets::programs[0];
        // if (program->link_status){
		// 	program->use();
		// 	for(auto&m:assets::meshes){
		// 		m->vao_bind();
		// 		m->vao_attrib_enable(program->attribs_enabled());
		// 		m->gl_draw();
		// 		m->vao_attrib_disable();
		// 	}
		// }
    }
	
	void exit(){
		assets::exit();
		save_scene();
	}
} // namespace engine
