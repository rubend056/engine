#include "engine_globals.h"
#include "engine.h"

#include "menus/menus.h"
#include "assets.h"
#include "components.h"

#include <SDL.h>

using namespace std;

GLuint load_vertex(const GLfloat* vertex_buffer){
	GLuint vbo;
	glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), vertex_buffer, GL_STATIC_DRAW);
	return vbo;
}

void testing(){
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
    
    GLuint vbo = load_vertex(g_vertex_buffer_data);
    
	
    
    //! GLint posAttrib = glGetAttribLocation()
    
    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glVertexAttribPointer(
    //     0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    //     3,                  // size
    //     GL_FLOAT,           // type
    //     GL_FALSE,           // normalized?
    //     0,                  // stride
    //     (void*)0            // array buffer offset
    // );
    // // Draw the triangle !
    // glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    // glDisableVertexAttribArray(0);
}


ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);
namespace engine
{
    void init(){
        selected.push_back(instantiate());
		assets::init();
    }

    void update(){
        menus::stats();
        menus::inspector(selected[0]);
        menus::files(project_path);
        menus::text_editor();
		assets::update();
    }
    void render(SDL_Window* window, ImGuiIO& io){
        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        
        
        testing();
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
	
	void exit(){
		assets::exit();
	}
} // namespace engine

