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
std::vector<std::shared_ptr<File>> files;
std::map<std::string, std::shared_ptr<File>> rpath_asset_ht;
std::map<uint, std::string> id_rpath_ht;
std::multimap<std::string, std::shared_ptr<File>> type_asset_ht;

void add(const std::shared_ptr<File>& file) {
	// STop if file is null, or if we have already added file
	if (!file || std::find(files.begin(), files.end(), file) != files.end())
		return;

	files.push_back(file);

	file->create_supposed_ext();
	auto hash_path = file->hash_path();
	rpath_asset_ht.insert({hash_path, file});
	id_rpath_ht.insert({file->get_id(), hash_path});
	type_asset_ht.insert({file->type(), file});
}
void clear() {
	files.clear();
	rpath_asset_ht.clear();
	id_rpath_ht.clear();
	type_asset_ht.clear();
}

void import_assets() {
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
					cout << "Loading " << asset_path << "as mesh" << endl;
					f = std::make_shared<Mesh>(asset_path);

				} else if (Texture::supported(ext)) {
					cout << "Loading " << asset_path << "as texture" << endl;
					f = std::make_shared<Texture>(asset_path);

				} else if (Shader::supported(ext)) {
					cout << "Loading " << asset_path << "as shader" << endl;
					f = std::make_shared<Shader>(asset_path);

				} else if (Program::supported(ext) || Scene::supported(ext)) {
					cout << "Loading " << asset_path << "as prog or scene" << endl;
					f = File::load_file(asset_path);
				}
			}

			if (f) {
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

std::shared_ptr<Referentiable> get_file_parent(std::vector<unsigned int> refs, std::shared_ptr<Referentiable> r) {
	if (!r)
		return std::shared_ptr<Referentiable>();
	if (!refs.size())
		return r;

	// Consume first id
	auto id = refs.front();
	refs.erase(refs.begin());

	// Look in children of Parent
	if (auto p = std::dynamic_pointer_cast<Parent>(r)) {
		auto it = std::find_if(p->children.begin(), p->children.end(),
							   [&](const std::shared_ptr<Referentiable>& r) {
								   return r ? r->get_id() == id : false;
							   });
		if (it != p->children.end()) {
			return get_file_parent(refs, *it);
		}
	}

	// Look in components of Gameobject
	if (auto go = std::dynamic_pointer_cast<GameObject>(r)) {
		auto it = std::find_if(go->components.begin(), go->components.end(),
							   [&](const std::shared_ptr<Component>& r) {
								   return r ? r->get_id() == id : false;
							   });
		if (it != go->components.end()) {
			return get_file_parent(refs, *it);
		}
	}

	// If nothing found, return null
	return std::shared_ptr<Referentiable>();
}
template <>
std::shared_ptr<Referentiable> get_file(const std::vector<unsigned int>& refs) {
	std::vector<unsigned int> _refs = refs;
	// Consume first id
	int id = _refs.front();
	_refs.erase(_refs.begin());

	// Find in our id to relative path map
	auto it = id_rpath_ht.find(id);
	if (it != id_rpath_ht.end()) {
		auto path = it->second;
		return get_file_parent(_refs, get_load_file(path));
	}

	// Return null if nothing found
	return std::shared_ptr<Referentiable>();
}

//? ENTRIES **************************
// All entries, including project_path to the project
vector<fs::directory_entry> entries;
void list_dir(string d, int depth = 0) {
	// Push the first dir
	if (!d.size())
		return;
	auto entry = fs::directory_entry(d);
	if (!entry.exists())
		return;
	assets::entries.push_back(entry);
	if (ENTRY_IS_DIR(entry)) {
		for (const auto& entry : fs::directory_iterator(d)) {
			list_dir(entry.path().string(), depth + 1);
		}
	}
}
void reload_project() {
	printf("Initializing inotify");
	entries.clear();
	list_dir(engine::project_path);

	// Initialize inotify
	{
		inotify::entries_allowed.clear();

		int argc = 0;
		for (auto& e : entries)
			if (ENTRY_IS_DIR(e))
				++argc;
		char* argv[argc];

		int c = 0;
		for (auto& e : entries) {
			// Populate filenames and argv
			if (!ENTRY_IS_DIR(e))
				inotify::entries_allowed.push_back(e.path().filename().string());
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

	// Update id to path
	id_rpath_ht.clear();
	for (auto& f : files) {
		id_rpath_ht.insert(std::make_pair(f->get_id(), f->hash_path()));
	}

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
	if (engine::project_path.empty())
		return;

	printf("Assets Init: project_path is %s\n", engine::project_path.c_str());

	// List all files
	reload_project();

	load_cache();
	import_assets();
}

void update() {
	inotify::inotify_update();

	bool reload = false;

	for (auto& event : inotify::events) {
		// std::printf("Event %d on %s\n", event.event, event.filename.c_str());
		switch (event.event) {
			case inotify::MODIFY: {
				auto p = fs::path(event.folder_path);
				p /= event.filename;
				p = engine::get_relative_to_project(p);

				if (auto f = assets::get_file_path(p)) {
					f->load();
					// If udpated file is a shader, link all programs that currently have that shader
					if (auto shader = std::dynamic_pointer_cast<Shader>(f)) {
						// Go through all shaders of every program, if any program has this shader, the program will be relinked
						for (auto& p : assets::get_files_type<Program>()) {
							auto _shaders = p->get_shaders();
							if (std::find(_shaders.begin(), _shaders.end(), shader->s_id) != _shaders.end()) {
								p->link();
								break;
							}
						}
					}
				}
			} break;
			case inotify::CREATE:
			case inotify::DELETE:
				std::printf(ANSI_COLOR_YELLOW "Create or delete triggered" ANSI_COLOR_RESET ENDL);
				reload = true;
				break;
			default:
				break;
		}
	}
	inotify::events.clear();

	if (reload)
		reload_project();
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