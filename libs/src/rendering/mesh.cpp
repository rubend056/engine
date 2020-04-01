#include "rendering/mesh.h"

#include "engine_globals.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"

// ? MESH ******************************************************************
static Assimp::Importer importer;

bool Mesh::supported(const std::string& ext){return importer.IsExtensionSupported(ext);}

void Mesh::load(){
	loaded = false;
	// Set d_path to the actual data path
	fs::path d_path = engine::get_absolute_from_project(data_path());
	auto ext = d_path.extension();
	
	if (importer.IsExtensionSupported(ext)) {
		auto scene = importer.ReadFile(d_path, import_flags);

		printf("Assimp %sread file %s\n", (!scene) ? "coudn't " : "", d_path.c_str());
		if (!scene) return;
		printf("Has %d mesh/es, %d texture/s\n", scene->mNumMeshes, scene->mNumTextures);
		
		if (scene->HasMeshes()){
			// Scan all meshes for relevant values to gauge the size of vbo
			unsigned int vbo_size=0;
			for (int i = 0; i < scene->mNumMeshes; ++i) {
				auto& mesh = scene->mMeshes[i];
				if (!mesh->HasPositions()) continue;
				else {
					vbo_size += mesh->mNumVertices * 3 * sizeof(float);
					if(mesh->HasNormals())
					vbo_size += mesh->mNumVertices * 3 * sizeof(float);
					if(mesh->HasTextureCoords(0))
					vbo_size += mesh->mNumVertices * 3 * sizeof(float);
				}
			}
			vbo_bind();
			glBufferData(GL_ARRAY_BUFFER, vbo_size, NULL , GL_STATIC_DRAW);
			
			unsigned int i=0;auto set_data = [&i](unsigned int size, void*data){glBufferSubData(GL_ARRAY_BUFFER, i, size, data);i+=size;};
			for (int i = 0; i < scene->mNumMeshes; ++i) {
				auto& mesh = scene->mMeshes[i];
				if (!mesh->HasPositions()) continue;
				
				auto vao = std::unique_ptr<VAO>(new VAO());
				vao->positions = mesh->HasPositions();
				vao->normals = mesh->HasNormals();
				vao->tex_cords = mesh->HasTextureCoords(0);
				vao->n_vertices = mesh->mNumVertices;
				vao->vao_bind();
				
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)i);
				set_data(vao->n_vertices*3*sizeof(float), mesh->mVertices);
				if(vao->normals){
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)i);
					set_data(vao->n_vertices*3*sizeof(float), mesh->mNormals);
				}
				if(vao->tex_cords){
					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)i);
					set_data(vao->n_vertices*3*sizeof(float), mesh->mTextureCoords[0]);
				}
				
				vaos.push_back(move(vao));
			}
			
		}
		loaded = true;
		importer.FreeScene();
	}
	
}

// Mesh::Mesh(const char* _f,int n, bool _normals, bool _tex_cords)

// Mesh::Mesh(const fs::path _f):File(_f){
	
	
	
// }
// void Mesh::gl_draw(){
// 	for(auto&vao:vaos){
// 		vao->vao_bind();
// 		glDrawArrays(vao->draw_function, 0, vao->n_vertices);
// 	}
// }
// void Mesh::vbo_bind()
// void Mesh::vbo_set_data(){
	
// }
// void Mesh::vao_set_vertex_attrib_pointer(){
	
// 	auto vbo_size = ((positions?sizeof(*positions):0) + 
// 					 (normals?sizeof(*normals):0) + 
// 					 (tex_cords?sizeof(*tex_cords):0)) * n_vertices;
	
// 	auto pos_s = sizeof(*positions)*n_vertices,
// 	norm_s = sizeof(*normals)*n_vertices,
// 	tex_s = sizeof(*tex_cords)*n_vertices;
// 	vbo_bind();
// 	// Setting the array buffer size
// 	glBufferData(GL_ARRAY_BUFFER, vbo_size, NULL , GL_STATIC_DRAW);
// 	// Filling the array buffer with our RAM data
// 	unsigned long i=0;
// 	auto f = [&i](unsigned long size, void*data) -> void{  glBufferSubData(GL_ARRAY_BUFFER, i, size, data);i+=size;  };
// 	f(pos_s, positions);
// 	if(normals)f(norm_s, normals);
// 	if(tex_cords)f(tex_s, tex_cords);
// 	// glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * n_vertices, vertices, GL_STATIC_DRAW);
	
// 	vao_bind();
// 	vbo_bind();
// 	auto pos_s = sizeof(*positions)*n_vertices,
// 	norm_s = sizeof(*normals)*n_vertices,
// 	tex_s = sizeof(*tex_cords)*n_vertices;
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
// 	if(normals)glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)pos_s);
// 	if(tex_cords)glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(pos_s+norm_s));
// }