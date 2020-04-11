#ifndef menus_h
#define menus_h

#include "my_imgui.h"
#include "gameobject.h"

#include <iostream>
#include <string>
#include <vector>
#include "my_filesystem.h"

namespace menus{	
	extern std::shared_ptr<IDraw> inspector_o;
	
	void imgui_engine_init();
	void imgui_engine_update();
	void imgui_engine_exit();
	
	// Open "add_popup" for picking File derivate, returns pointer to picked
	// template <class T>
	std::shared_ptr<File> add_popup(
		const std::vector<std::shared_ptr<File>>& vector, 
		bool modal=false, 
		const char*label="add_popup")
	;
}; // namespace menus



#endif