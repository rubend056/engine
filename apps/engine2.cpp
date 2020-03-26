#include "engine.h"

#include "menus/menus.h"
#include "assets.h"
#include "components/components.h"

#include "groups/gl.h"
#include "menus/imgui_engine.h"

#include <SDL.h>

#include <algorithm>

using namespace std;

// GLuint load_vertex(const GLfloat* vertex_buffer){
	
// 	return vbo;
// }

// static GLuint vba;
// static GLuint vbo;
// void testing_init(){
//     static const GLfloat g_vertex_buffer_data[] = {
//         -1.0f, -1.0f,
//         1.0f, -1.0f,
//         0.0f,  1.0f
//     };
    
//     glGenVertexArrays(1, &vba);
//     glEnableVertexAttribArray(vba);
    
//     // GLuint vbo = load_vertex(g_vertex_buffer_data);
    
// 	glGenBuffers(1, &vbo);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
//     glVertexAttribPointer(
//     0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//     2,                  // size
//     GL_FLOAT,           // type
//     GL_FALSE,           // normalized?
//     0,                  // stride
//     (void*)0            // array buffer offset
//     );
// }
// void testing_draw(){
//     glEnableVertexAttribArray(vba);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
//     glDisableVertexAttribArray(vba);
    
// }

// #include "cereal/types/polymorphic.hpp"
// struct A{
	
// };
// struct B{
// 	int j=4;
// 	virtual void t()=0;
// 	template<class Archive>
// 	void serialize(Archive & ar){
// 		ar(CEREAL_NVP(j));
// 	}
// };
// struct J:public B{
// 	void t(){};
// 	int l=2;
// 	template<class Archive>
// 	void serialize(Archive & ar){
// 		ar(cereal::make_nvp("B",cereal::base_class<B>(this)), CEREAL_NVP(l));
// 	}
// };
// #include <cereal/archives/json.hpp>
// CEREAL_REGISTER_TYPE_WITH_NAME(J, "JType");
// // CEREAL_REGISTER_POLYMORPHIC_RELATION(B, J)


ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);
namespace engine
{
    void init(){
		// std::shared_ptr<B> b(new J);
		// cereal::JSONOutputArchive oarchive(std::cout);
		// oarchive(b);
		
        load_settings_glEnable();
		auto go = instantiate();
		strcpy(go->filename,"First GObject");
		selected.push_back(go);
		assets::init();
        
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    }

    void update(){
		
		menus::imgui_engine_update();
        
		
        // window_glEnable_config();
        
		assets::update();
    }
    void render(SDL_Window* window, ImGuiIO& io){
		
        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // testing_draw();
        // RENDER
        
        // auto pred = [](Mesh*&m) -> bool{return string(m->filename).compare("testmesh") == 0;};
        // auto mesh = *find_if(assets::meshes.begin(), assets::meshes.end(), pred);
        
		if(assets::textures.size()){
			auto &texture = assets::textures[0];
			glBindTexture(GL_TEXTURE_2D, texture->t_id);
		}
		
        auto& program = assets::programs[0];
        if (program->link_status){
			program->use();
			for(auto&m:assets::meshes){
				m->vao_bind();
				m->vao_attrib_enable(program->attribs_enabled());
				m->gl_draw();
				m->vao_attrib_disable();
			}
		}
        
        
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
	
	void exit(){
		assets::exit();
	}
} // namespace engine

