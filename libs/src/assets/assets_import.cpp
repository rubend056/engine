#include "SOIL.h"
#include "_assets.h"

namespace assets {
//? RENDERING **************************
std::vector<std::shared_ptr<File>> files;
std::unordered_map<std::string, std::shared_ptr<File>> rpath_asset_ht;
std::unordered_multimap<std::string, std::shared_ptr<File>> type_asset_ht;

void add(const std::shared_ptr<File>& file){
	if(!file)return;
	files.push_back(file);
	rpath_asset_ht.insert(std::make_pair(file->data_path(),file));
	type_asset_ht.insert(std::make_pair(helper::demangle(typeid(file.get()).name()), file));
}
void clear(){
	files.clear();
	rpath_asset_ht.clear();
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

void import_assets() {
    // Assimp::Importer importer;

    // TEST MESHES
    auto tmesh = std::shared_ptr<Mesh>(new Mesh(fs::path("tmesh")));
	tmesh->vbo_bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(testvertices)+sizeof(testtextcords), nullptr, GL_STATIC_DRAW);
	
	auto vao = std::make_unique<Mesh::VAO>();
	vao->vao_bind();
	vao->n_vertices = 3;
	vao->positions = vao->tex_cords = true;
	
	unsigned int i=0;auto set_data = [&i](unsigned int size, void*data){glBufferSubData(GL_ARRAY_BUFFER, i, size, data);i+=size;};
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)i);
	set_data(sizeof(testvertices), (void*)testvertices);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)i);
	set_data(sizeof(testtextcords), (void*)testtextcords);
	
    tmesh->loaded =true;
    tmesh->vaos.push_back(move(vao));
    add(tmesh);

    // #################
	for (auto &e : entries) {
		if (ENTRY_IS_DIR(e)) continue;
		
		if(e.path().extension().compare(METADATA_EXT) == 0){
			auto f = File::load_file(e.path());
			if(f)add(f);
		}
			
	}
	
    for (auto &e : entries) {
		// Skip for all directories
		// Skip for all metadata
        if (ENTRY_IS_DIR(e)) continue;
		
		auto asset_path = engine::get_relative_to_project(e.path());
		
		std::shared_ptr<File> f;
		auto ext = asset_path.extension();
		// if(e.path().extension().compare(METADATA_EXT) == 0)
			
		if(Mesh::supported(ext))
			f = std::make_shared<Mesh>(asset_path);
		else if(Texture::supported(ext))
			f = std::make_shared<Texture>(asset_path);
		else if(Shader::supported(ext))
			f = std::make_shared<Shader>(asset_path);
		else if(Program::supported(ext))
			f = std::make_shared<Program>(asset_path);
		
		if(f)add(f);
    }
	cout << ANSI_COLOR_GREEN << "Finished Importing" << ANSI_COLOR_RESET << endl;
}

}  // namespace assets