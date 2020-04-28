#include "SOIL.h"
#include "_assets.h"

namespace assets {
//? RENDERING **************************
std::vector<std::shared_ptr<File>> files;
std::unordered_map<std::string, std::shared_ptr<File>> rpath_asset_ht;
std::unordered_map<unsigned int, std::string> id_rpath_ht;
std::unordered_multimap<std::string, std::shared_ptr<File>> type_asset_ht;

void add(const std::shared_ptr<File>& file){
	if(!file)return;
	if(std::find(files.begin(), files.end(), file) == files.end())
		files.push_back(file);
	rpath_asset_ht.insert(std::make_pair(file->data_path(),file));
	id_rpath_ht.insert(std::make_pair(file->get_id(), file->data_path().string()));
	type_asset_ht.insert(std::make_pair(helper::demangle(typeid(*file).name()), file));
}
void clear(){
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
	0.0f, 0.0f
};

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
    for(int load_meta=1;load_meta>=0;--load_meta) 
	for (auto &e : entries) {
		// Skip for all directories
        if (ENTRY_IS_DIR(e)) continue;
		// If you're not loading meta, skip all meta extensions
		// If you are loading meta, skill all other extensions
		// You're gonna skip if they're not the same
		bool is_meta = e.path().extension().compare(METADATA_EXT) == 0;
		if (is_meta^load_meta)continue;
		
		// Get asset path
		auto asset_path = engine::get_relative_to_project(e.path());
		auto data_path = asset_path;
		if(is_meta)data_path.replace_filename(data_path.stem());
		
		// Skip for files already present
		if(assets::rpath_asset_ht.find(data_path) != assets::rpath_asset_ht.end())continue;
		
		auto ext = data_path.extension();
		// Get real extension if it's meta
		// if(load_meta)ext = asset_path.stem().extension();
		
		std::shared_ptr<File> f;
		
		if(is_meta){
			if(	Mesh::supported(ext) || 
				Texture::supported(ext) || 
				Shader::supported(ext))
			{
				cout << "Loading " << asset_path << "as meta" << endl;
				f = File::load_file(asset_path);
			}
		}else{
			if (Mesh::supported(ext)){
				cout << "Loading " << asset_path << "as new" << endl;
				f = std::make_shared<Mesh>(asset_path);
				
			}else if(Texture::supported(ext)){
				cout << "Loading " << asset_path << "as new" << endl;
				f = std::make_shared<Texture>(asset_path);
				
			}else if(Shader::supported(ext)){
				cout << "Loading " << asset_path << "as new" << endl;
				f = std::make_shared<Shader>(asset_path);
				
			}else if (Program::supported(ext) || Scene::supported(ext)){
				cout << "Loading " << asset_path << "as meta" << endl;
				f = File::load_file(asset_path);
			}
		}
			
		if(f){
			f->create_supposed_ext();
			assets::add(f);
		}
    }
	cout << ANSI_COLOR_GREEN << "Finished Importing" << ANSI_COLOR_RESET << endl;
}

}  // namespace assets