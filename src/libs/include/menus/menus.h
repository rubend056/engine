#ifndef menus_h
#define menus_h

#include <iostream>
#include <string>
#include <vector>

#include "factory.h"
#include "gameobject.h"
#include "my_filesystem.h"
#include "my_imgui.h"

typedef int MenuSelectFlags;
enum MenuSelectFlags_ {
	MenuSelectFlags_None = 0,
	// Make menu a modal, will cover the whole screen
	MenuSelectFlags_Modal = 1 << 0,
	MenuSelectFlags_NewOnly = 2 << 0,
	// MenuSelectFlags_Asset = 1 << 1,
};

typedef std::function<bool(const std::shared_ptr<Ref>&)> select_filter;
#define SELECT_FILTER_DEFAULT [](const std::shared_ptr<Ref>& r) { return true; }

// typedef std::function<std::shared_ptr<Ref>()> select_new;
// #define SELECT_NEW_DEFAULT []() { return std::make_shared<Ref>(); }

namespace menus {

struct FileText {
	fs::path path;
	std::string data;
	bool open = true;
};

extern std::vector<FileText> text_files;
extern std::set<std::shared_ptr<IDraw>> inspector_o;

void imgui_engine_init();
void imgui_engine_update();
void imgui_engine_exit();

bool path_exists(const std::string& path);

std::shared_ptr<Referentiable> select_asset(const char* name, select_filter select_filter = SELECT_FILTER_DEFAULT, FACTORY_KEY_TYPE create_types = Factory_None, MenuSelectFlags flags = 0);

template <typename T>
std::shared_ptr<T> select_asset(const char* name, FACTORY_KEY_TYPE create_types = Factory_None, MenuSelectFlags flags = 0) {
	return std::dynamic_pointer_cast<T>(
		select_asset(
			name,
			[](const std::shared_ptr<Ref>& ref) -> bool { return !!std::dynamic_pointer_cast<T>(ref); }, create_types,
			flags));
}

// Open "add_popup" for picking File derivate, returns pointer to picked
// template <class T>
// std::shared_ptr<File> add_popup(
// 	const std::vector<std::shared_ptr<File>>& vector,
// 	bool modal = false,
// 	const char* label = "add_popup");

};	// namespace menus

#endif