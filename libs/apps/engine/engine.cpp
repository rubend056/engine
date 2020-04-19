#include "_engine.h"

#include "menus.h"
#include "assets.h"
#include "helper.h"
#include "components.h"
#include "mesh.h"

#include "gl.h"

#include <SDL.h>

#include <algorithm>

using namespace std;

fs::path relativePath( const fs::path &path, const fs::path &relative_to ){
    // create absolute paths
    fs::path p = fs::absolute(path);
    fs::path r = fs::absolute(relative_to);

    // if root paths are different, return absolute path
    if( p.root_path() != r.root_path() )
        return p;

    // initialize relative path
    fs::path result;

    // find out where the two paths diverge
    fs::path::const_iterator itr_path = p.begin();
    fs::path::const_iterator itr_relative_to = r.begin();
    while( itr_path != p.end() && itr_relative_to != r.end() && *itr_path == *itr_relative_to ) {
        ++itr_path;
        ++itr_relative_to;
    }

    // add "../" for each remaining token in relative_to
    if( itr_relative_to != r.end() ) {
        ++itr_relative_to;
        while( itr_relative_to != r.end() ) {
            result /= "..";
            ++itr_relative_to;
        }
    }

    // add remaining path
    while( itr_path != p.end() ) {
        result /= *itr_path;
        ++itr_path;
    }

    return result;
}

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
		// return asset_path.lexically_relative(project_path);
		return relativePath(asset_path, project_path);
	}
#define ENGINE_CACHE_PATH engine::get_absolute_from_project("cache.json")
	void save_scene(){
		if(scene){
			std::ofstream f(ENGINE_CACHE_PATH);
			cereal::JSONOutputArchive ar(f);
			
			// Saving Scene
			// File::save_file(scene);
			ar(cereal::make_nvp("scene", scene->hash_path()));
			
			// Saving Inspector File
			// auto ins_paths = assets::data_path(std::dynamic_pointer_cast<File>(menus::inspector_o));
			// ar(cereal::make_nvp("inspector", ins_paths));
		}
	}
	void load_scene(const std::shared_ptr<Scene>& _scene){
		save_scene();
		scene = _scene;
		menus::inspector_o.clear();
	}
}


ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);
namespace engine
{
    void init(){
		assets::init();
		
		if(fs::exists(ENGINE_CACHE_PATH)){
			
			
			std::ifstream f(ENGINE_CACHE_PATH);
			cereal::JSONInputArchive ar(f);
			
			// Loading last scene
			std::string last_scene;
			ar(cereal::make_nvp("scene", last_scene));
			printf("Loading last scene %s\n", last_scene.c_str());
			auto scene = assets::get_load_file<Scene>(last_scene);
			if(scene)engine::load_scene(scene);
			
			// Loading last inspector
			// std::vector<fs::path> ins_paths;
			// ar(cereal::make_nvp("inspector", ins_paths));
			// if(auto draw = std::dynamic_pointer_cast<IDraw>(assets::data_path_find(ins_paths)))
			// 	menus::inspector_o.insert(draw);
		}
		
		menus::imgui_engine_init();
        
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    }

    void update(){
		menus::imgui_engine_update();
		assets::update();
    }
    void render(){
		
        // Rendering
        
		if(engine::scene){
			for(auto&go:engine::scene->objects){
				auto progs = go->get_comps<Program>();
				if(progs.size()>0){
					auto trans = go->get_comp<Transform>();
					auto vaos = go->get_comps<Mesh::VAO>();
					for(auto&prog:progs){
						if(!prog->link_status)continue;
						prog->use();
						for(auto&vao:vaos){
							vao->vao_bind();
							vao->vao_attrib_enable(0xff);
							vao->gl_draw();
						}
					}
				}
			}
		}
        
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
		menus::imgui_engine_exit();
		save_scene();
		assets::exit();
	}
} // namespace engine

