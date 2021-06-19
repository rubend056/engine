// Cereal
#include "cereal/archives/json.hpp"
#include "cereal/types/unordered_map.hpp"

// Ours
#include "_assets.h"
#include "engine_globals.h"
#include "gameobject.h"
#include "mesh.h"
#include "scene.h"

// System
#include <algorithm>

//? Assets IMPORT ****************************************************************
namespace assets {
//? RENDERING **************************
std::vector<std::shared_ptr<File>> files;
std::unordered_map<std::string, std::shared_ptr<File>> rpath_asset_ht;
std::unordered_map<unsigned int, std::string> id_rpath_ht;
std::unordered_multimap<std::string, std::shared_ptr<File>> type_asset_ht;

void add(const std::shared_ptr<File>& file) {
	if (!file)
		return;
	if (std::find(files.begin(), files.end(), file) == files.end())
		files.push_back(file);
	rpath_asset_ht.insert(std::make_pair(file->data_path(), file));
	id_rpath_ht.insert(std::make_pair(file->get_id(), file->data_path().string()));
	type_asset_ht.insert(std::make_pair(helper::demangle(typeid(*file).name()), file));
}
void clear() {
	files.clear();
	rpath_asset_ht.clear();
	id_rpath_ht.clear();
	type_asset_ht.clear();
}

const float testvertices[]{
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f};
const float testtextcords[]{
	0.5f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f};

// void load_all(bool (*pred)(const std::string& ext)){

// }

void import_assets() {
	// Assimp::Importer importer;

	// TEST MESHES
	// auto tmesh = std::shared_ptr<Mesh>(new Mesh(fs::path("tmesh")));
	// tmesh->vbo_bind();
	// glBufferData(GL_ARRAY_BUFFER, sizeof(testvertices)+sizeof(testtextcords), nullptr, GL_STATIC_DRAW);

	// auto vao = std::make_unique<Mesh::VAO>();
	// vao->vao_bind();
	// vao->n_vertices = 3;
	// vao->positions = vao->tex_cords = true;

	// unsigned int i=0;auto set_data = [&i](unsigned int size, void*data){glBufferSubData(GL_ARRAY_BUFFER, i, size, data);i+=size;};
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)i);
	// set_data(sizeof(testvertices), (void*)testvertices);
	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)i);
	// set_data(sizeof(testtextcords), (void*)testtextcords);

	// tmesh->loaded =true;
	// tmesh->vaos.push_back(move(vao));
	// assets::add(tmesh, typeid(Mesh).name());
	// #################

	// Loading all files, starting with meta files
	for (int load_meta = 1; load_meta >= 0; --load_meta)
		for (auto& e : entries) {
			// Skip for all directories
			if (ENTRY_IS_DIR(e))
				continue;
			// If you're not loading meta, skip all meta extensions
			// If you are loading meta, skill all other extensions
			// You're gonna skip if they're not the same
			bool is_meta = e.path().extension().compare(METADATA_EXT) == 0;
			if (is_meta ^ load_meta)
				continue;

			// Asset path is relative path of entrie in project
			auto asset_path = engine::get_relative_to_project(e.path());
			// Data path will have the extension removed
			auto data_path = asset_path;
			if (is_meta)
				data_path.replace_filename(data_path.stem());

			// Skip for files already present
			if (assets::rpath_asset_ht.find(data_path) != assets::rpath_asset_ht.end())
				continue;

			auto ext = data_path.extension();
			// Get real extension if it's meta
			// if(load_meta)ext = asset_path.stem().extension();

			std::shared_ptr<File> f;

			if (is_meta) {
				// If file has a .meta extension
				if (Mesh::supported(ext) ||
					Texture::supported(ext) ||
					Shader::supported(ext)) {
					cout << "Loading " << asset_path << "as meta" << endl;
					f = File::load_file(asset_path);
				}
			} else {
				// If file doesn't have .meta extension
				if (Mesh::supported(ext)) {
					cout << "Loading " << asset_path << "as new" << endl;
					f = std::make_shared<Mesh>(asset_path);

				} else if (Texture::supported(ext)) {
					cout << "Loading " << asset_path << "as new" << endl;
					f = std::make_shared<Texture>(asset_path);

				} else if (Shader::supported(ext)) {
					cout << "Loading " << asset_path << "as new" << endl;
					f = std::make_shared<Shader>(asset_path);

				} else if (Program::supported(ext) || Scene::supported(ext)) {
					cout << "Loading " << asset_path << "as meta" << endl;
					f = File::load_file(asset_path);
				}
			}

			if (f) {
				f->create_supposed_ext();
				assets::add(f);
			}
		}
	cout << ANSI_COLOR_GREEN << "Finished Importing" << ANSI_COLOR_RESET << endl;
}

}  // namespace assets

namespace assets {

std::shared_ptr<File> get_file_type(const std::string& t_name) {
	auto it = type_asset_ht.find(t_name);
	if (it == type_asset_ht.end())
		return std::shared_ptr<File>();
	else
		return it->second;
}

std::vector<std::shared_ptr<File>> get_files_type(const std::string& t_name) {
	auto eq_range = type_asset_ht.equal_range(t_name);
	std::vector<std::shared_ptr<File>> v;
	for (auto it = eq_range.first; it != eq_range.second; ++it)
		v.push_back(it->second);
	return v;
}

template <>
std::shared_ptr<File> get_file_path(const std::string& t_name) {
	auto it = rpath_asset_ht.find(t_name);
	if (it == rpath_asset_ht.end())
		return std::shared_ptr<File>();
	else
		return it->second;
}

template <>
std::shared_ptr<Referentiable> get_file(const std::vector<unsigned int>& refs) {
	if (!refs.size())
		return std::shared_ptr<Referentiable>();

	int c = 0;
	std::shared_ptr<Referentiable> target;
	// If finding path was successful

	auto path_it = id_rpath_ht.find(refs[c++]);
	if (path_it != id_rpath_ht.end()) {
		auto path = path_it->second;
		// Get target object based on path
		if (target = get_load_file(path)) {
			// As long as refs has more references we keep going
			while (c < refs.size()) {
				// Convert taget to parent
				auto parent = std::dynamic_pointer_cast<Parent>(target);
				if (!parent)
					break;	// Stop searching, refs is asking for more but you're not a parent?

				int id = refs[c++];
				// Find a child with the desired id
				auto it_child = std::find_if(parent->children.begin(), parent->children.end(),
											 [&](const std::shared_ptr<Referentiable>& r) {
												 return r ? r->get_id() == id : false;
											 });
				// Set target to child if it was found
				if (it_child != parent->children.end())
					target = *it_child;
				else if (auto go = std::dynamic_pointer_cast<GameObject>(target)) {
					// Find a child with the desired id
					auto it_component = std::find_if(go->components.begin(), go->components.end(),
													 [&](const std::shared_ptr<Component>& r) {
														 return r ? r->get_id() == id : false;
													 });
					if (it_component != go->components.end())
						target = *it_component;
					else
						break;
				} else
					break;
			}
		}
	}

	// Only return target if we reached the end of the refs vector in the search
	return c == refs.size() ? target : std::shared_ptr<Referentiable>();
}

//? ENTRIES **************************
// All entries, including project_path to the project
vector<fs::directory_entry> entries;
void list_dir(string d) {
	for (const auto& entry : fs::directory_iterator(d)) {
		assets::entries.push_back(entry);
		if (ENTRY_IS_DIR(entry)) {
			list_dir(entry.path().string());
		}
	}
}
void reload_project() {
	entries.clear();
	entries.push_back(fs::directory_entry(engine::project_path));
	list_dir(engine::project_path);

	// Initialize inotify
	{
		inotify::filenames_allowed.clear();

		int argc = 0;
		for (auto& e : entries)
			if (ENTRY_IS_DIR(e))
				++argc;
		char* argv[argc];

		int c = 0;
		for (auto& e : entries) {
			// Populate filenames and argv
			if (!ENTRY_IS_DIR(e))
				inotify::filenames_allowed.push_back(e.path().filename().string());
			else {
				// String and length defined
				auto path = e.path().string();
				auto p = path.c_str();
				auto l = strlen(p) + 1;
				argv[c] = (char*)malloc(l);	 // Allocating
				memcpy(argv[c], p, l);
				cout << argv[c] << endl;
				++c;
			}
		}

		inotify::inotify_init(argc, argv);
		// Free argv's memory
		for (auto& av : argv) free(av);
	}
}

#define ASSETS_CACHE_APATH engine::get_absolute_from_project("cache_assets.json")
void save_cache() {
	std::ofstream file(ASSETS_CACHE_APATH);
	if (!file.is_open())
		return;
	cereal::JSONOutputArchive ar(file);
	ar(id_rpath_ht);
}
void load_cache() {
	std::ifstream file(ASSETS_CACHE_APATH);
	if (!file.is_open())
		return;
	cereal::JSONInputArchive ar(file);
	ar(id_rpath_ht);
}

void init() {
	if (!engine::project_path.c_str())
		return;

	printf("Assets path is %s\n", engine::project_path.c_str());

	// List all files
	reload_project();

	load_cache();
	import_assets();
}

void update() {
	{  // INOTIFY EVENT HANDLING
		inotify::inotify_update();
		bool reload = false;
		if (inotify::events.size()) {
			for (auto& event : inotify::events) {
				std::printf("Event %d on %s\n", (int)event->event, std::string(event->filename).c_str());
				if (event->event == inotify::MODIFY) {
					auto p = fs::path(event->folder_path);
					p /= event->filename;
					p = engine::get_relative_to_project(p);

					if (auto f = assets::get_file_path(p)) {
						f->load();
						if (auto shader = std::dynamic_pointer_cast<Shader>(f)) {
							auto programs = assets::get_files_type<Program>();
							for (auto& p : programs) {
								auto _shaders = p->get_shaders();
								if (std::find(_shaders.begin(), _shaders.end(), shader->s_id) != _shaders.end())
									p->link();
							}
						}
						// else if(auto mesh = std::dynamic_pointer_cast<Mesh>(f))mesh->load();
					}
				} else if (event->event == inotify::CREATE || event->event == inotify::DELETE) {
					std::printf(ANSI_COLOR_YELLOW "Create or delete triggered" ANSI_COLOR_RESET "\n");
					reload = true;
				}
			}
			inotify::events.clear();
			inotify::_events.clear();
		}
		if (reload)
			reload_project();
	}
}

void exit() {
	// INOTIFY EXIT
	inotify::inotify_exit();

	//! Enable saving all files
	for (auto& f : assets::files) {
		File::save_file(f);
	}

	save_cache();
}
}  // namespace assets