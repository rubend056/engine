#include "mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "engine_globals.h"
#include "helper.h"
#include "vao.h"
#include "imgui_helper.h"

// CONSTRUCTORS
// Mesh::Mesh(FILE_CONSTRUCT_PARAM) : File(FILE_CONSTRUCT_VARS) {
// 	// create_supposed_ext();
// 	glGenBuffers(1, &vbo_id);
// 	if (!rpath.empty())
// 		load();
// }
// Mesh::~Mesh() { glDeleteBuffers(1, &vbo_id); }

static Assimp::Importer importer;

// FUNCTIONS
bool Mesh::supported(const std::string& ext) { return importer.IsExtensionSupported(ext); }
void Mesh::load() {
	loaded = false;
	// Copy children before clearing since they'll be used later on
	auto old_children = children;
	children.clear();
	// Set d_path to the actual data path
	fs::path d_path = engine::get_absolute_from_project(data_path());
	auto ext = d_path.extension();

	if (supported(ext)) {
		auto scene = importer.ReadFile(d_path, import_flags);

		printf("Assimp %sread file %s\n", (!scene) ? "coudn't " : "", d_path.c_str());
		if (!scene)
			return;
		printf("Has %d mesh/es, %d texture/s\n", scene->mNumMeshes, scene->mNumTextures);

		if (scene->HasMeshes()) {
			// Scan all meshes for relevant values to gauge the size of vbo
			unsigned int vbo_size = 0;
			for (int i = 0; i < scene->mNumMeshes; ++i) {
				auto& mesh = scene->mMeshes[i];
				if (!mesh->HasPositions())
					continue;
				else {
					vbo_size += mesh->mNumVertices * 3 * sizeof(float);
					if (mesh->HasNormals())
						vbo_size += mesh->mNumVertices * 3 * sizeof(float);
					if (mesh->HasTextureCoords(0))
						vbo_size += mesh->mNumVertices * 3 * sizeof(float);
				}
			}
			vbo_bind();
			// Allocate VBO
			glBufferData(GL_ARRAY_BUFFER, vbo_size, NULL, GL_STATIC_DRAW);

			unsigned int i = 0;
			auto set_data = [&i](unsigned int size, void* data) {glBufferSubData(GL_ARRAY_BUFFER, i, size, data);i+=size; };
			for (int i = 0; i < scene->mNumMeshes; ++i) {
				auto& mesh = scene->mMeshes[i];
				if (!mesh->HasPositions())
					continue;

				auto mesh_name = std::string(mesh->mName.C_Str());
				auto vao = std::shared_ptr<VAO>();
				// Try to find the last VAO
				{
					auto it = std::find_if(old_children.begin(), old_children.end(), [&](std::shared_ptr<Referentiable>& v) {
						return std::dynamic_pointer_cast<VAO>(v)->name == mesh_name;
					});
					if (it != old_children.end())
						vao = std::dynamic_pointer_cast<VAO>(*it);
				}
				if (!vao)
					vao = std::make_shared<VAO>();
				vao->name = std::string(mesh->mName.C_Str());
				vao->positions = mesh->HasPositions();
				vao->normals = mesh->HasNormals();
				vao->tex_cords = mesh->HasTextureCoords(0);
				vao->n_vertices = mesh->mNumVertices;
				vao->vao_bind();

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(intptr_t)i);
				set_data(vao->n_vertices * 3 * sizeof(float), mesh->mVertices);
				if (vao->normals) {
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(intptr_t)i);
					set_data(vao->n_vertices * 3 * sizeof(float), mesh->mNormals);
				}
				if (vao->tex_cords) {
					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(intptr_t)i);
					set_data(vao->n_vertices * 3 * sizeof(float), mesh->mTextureCoords[0]);
				}
				foster(vao);
			}
		}
		loaded = true;
		importer.FreeScene();
	}
}

// DRAWING
void Mesh::imgui_draw() {
	auto vaos = helper::dynamic_pointer_cast_array<VAO>(children);
	ImGui::TextDisabled("VAO's");
	for (auto& vao : vaos) {
		// auto& vao = *it;int index = it-vaos.begin();
		ImGui::BeginChild(vao->name.c_str(), ImVec2(-1, 0), true);

		ImGui::Button("Drag Me", ImVec2(-1, 0));
		
		auto refs = vao->my_ref();
		imgui_dnd_ref(refs, true, ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceAllowNullID);
		
		vao->imgui_draw();
		ImGui::EndChild();
	}
}