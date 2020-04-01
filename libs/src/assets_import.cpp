#include "SOIL.h"
#include "_assets.h"

namespace assets {
//? RENDERING **************************
std::unordered_map<std::string, std::shared_ptr<File>> rpath_asset_ht;
std::unordered_multimap<std::string, std::shared_ptr<File>> type_asset_ht;

void asset_hash_table_add(const std::shared_ptr<File>& file){
	rpath_asset_ht.insert(std::make_pair(file->rel_path.string(),file));
	type_asset_ht.insert(std::make_pair(helper::demangle(typeid(file.get()).name()), file));
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
    asset_hash_table_add(tmesh);

    // #################

    for (auto &e : entries) {
        if (ENTRY_IS_DIR(e)) continue;
		
		auto asset_path = engine::get_relative_to_project(e.path());
		
		std::shared_ptr<File> f;
		
		if(Mesh::supported(asset_path.extension()))
			f = std::make_shared<Mesh>(asset_path);
		else if(Texture::supported(asset_path.extension()))
			f = std::make_shared<Texture>(asset_path);
		else if(Shader::supported(asset_path.extension()))
			f = std::make_shared<Shader>(asset_path);
		else if(Program::supported(asset_path.extension()))
			f = std::make_shared<Program>(asset_path);
		
		if(f)asset_hash_table_add(f);
    }
}

}  // namespace assets