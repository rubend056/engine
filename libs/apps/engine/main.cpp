#include "_engine.h"

using namespace std;

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "config.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


int main( int argc, char* args[] )
{
	
    if (argc <= 1){cout << "Need path to project folder" << endl;return -1;}
	// cout << argc << endl;
	engine::project_path = fs::canonical(fs::absolute(fs::path(args[1])));
	if(engine::project_path.empty()){cout << "Path to project empty" << endl;return -1;}

    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    
    if( SDL_Init( SDL_INIT_VIDEO|SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return -1;
    }
    

    const char* glsl_version = GLAD_OPENGL_CORE_VERSION>=45?"#version 450":"#version 330";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#if (GLAD_OPENGL_CORE_VERSION>=45)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
#else
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    // Create Window
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_UTILITY);
    window = SDL_CreateWindow( "Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags );
    if( window == NULL ){printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );}
    
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#endif
    if (err){fprintf(stderr, "Failed to initialize OpenGL loader!\n");return 1;}
    
	cout << glGetString(GL_VERSION) << endl;
	
    //? IMGUI *****************
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
	// auto iniPath=;
	auto is = (std::string(engine::project_path) + "/imgui.ini");
	char* ini_path = new char[is.length()+1];
	strcpy(ini_path, is.c_str());
	io.IniFilename = ini_path;
    //ImGui::StyleColorsClassic();
	
    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
	//?#########################
    
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    engine::init();
    while (engine::run)
    {
        
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                engine::run = false;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            	engine::run = false;
			
			// Put your input here
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        
        GLenum err;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            printf("OpenGL error %d\n", err);
        }
        
        engine::update();
        
		ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x/2, (int)io.DisplaySize.y/2);
        glClear(GL_COLOR_BUFFER_BIT);
		
        engine::render();
		
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
	
	engine::exit();
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow( window );
    SDL_Quit();

    return 0;
}



