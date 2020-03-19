#include "SOIL.h"
#include "_assets.h"

namespace assets {
//? RENDERING **************************
vector<Mesh *> meshes;
vector<Shader *> shaders;
vector<Texture *> textures;
vector<Program *> programs;

bool init_shader(fs::directory_entry &e) {
    auto path = e.path().string();
    auto ext = boost::algorithm::to_lower_copy(e.path().extension().string());
    auto filename = boost::algorithm::to_lower_copy(e.path().filename().string());
    if (ext.compare(".glsl") == 0 || ext.compare(".vert") == 0 || ext.compare(".frag") == 0) {
        SHADER_ENUM type = NOTHING;
        if (ext.compare(".frag") == 0)
            type = FRAGMENT;
        else if (ext.compare(".vert") == 0)
            type = VERTEX;
        else if (filename.find("fragment") != string::npos || filename.find("frag") != string::npos)
            type = FRAGMENT;
        else if (filename.find("geometry") != string::npos)
            type = GEOMETRY;
        else if (filename.find("vertex") != string::npos)
            type = VERTEX;

        if (type != NOTHING) {
            printf("Importing %s as type %d shader\n", filename.c_str(), type);
            auto sfile = ifstream(path.c_str());
            auto data = string((istreambuf_iterator<char>(sfile)), istreambuf_iterator<char>());
            auto shader = new Shader(type, e.path().filename().string().c_str(), data.c_str());
            shaders.push_back(shader);
        } else
            printf("Coudn't find type for shader %s, please update filename\n", filename.c_str());
        return true;
    }
    return false;
}

bool update_shader(string filename) {
    string path;
    for (auto &e : entries) {
        if (e.path().filename().string().compare(filename) == 0) path = e.path().string();
    }
    if (path.size() == 0) return false;

    auto lfilename = boost::algorithm::to_lower_copy(filename);
    for (auto &shader : shaders) {
        if (lfilename.compare(shader->filename) == 0) {
            auto sfile = ifstream(path.c_str());
            auto data = string((istreambuf_iterator<char>(sfile)), istreambuf_iterator<char>());
            printf("Compiling shader for %s\n", shader->filename);
            shader->update(data.c_str());
        }
    }
    return true;
}

const float testvertices[]{
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f};

void import_assets() {
    Assimp::Importer importer;

    // TEST MESHES
    auto tmesh = new Mesh("testmesh", 3);
    for (int i = 0; i < 3; ++i)
        memcpy(&tmesh->positions[i], &testvertices[i * 3], 3 * sizeof(float));
    tmesh->vbo_set_data();
    meshes.push_back(tmesh);

    // FilledCircleMesh* cmesh = new FilledCircleMesh("circlemesh",.3f, 20);
    // cmesh->vbo_set_data();
    // meshes.push_back(cmesh);
    // #################

    for (auto &e : entries) {
        if (ENTRY_IS_DIR(e)) continue;

        auto path = e.path().string();
        auto ext = boost::algorithm::to_lower_copy(e.path().extension().string());
        auto filename = boost::algorithm::to_lower_copy(e.path().filename().string());
        if (importer.IsExtensionSupported(ext)) {
            auto scene = importer.ReadFile(path.c_str(), aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

            printf("Assimp %sread file %s\n", (!scene) ? "coudn't " : "", path.c_str());
            if (!scene) continue;
            printf("Has %d mesh/es, %d material/s\n", scene->mNumMeshes, scene->mNumTextures);

            if (scene->HasMeshes())
                for (int i = 0; i < scene->mNumMeshes; ++i) {
                    auto aimesh = scene->mMeshes[i];

                    if (aimesh->HasPositions()) {
                        auto mesh = new Mesh(filename.c_str(), aimesh->mNumVertices);
                        for (int i = 0; i < aimesh->mNumVertices; ++i) {
                            memcpy(
                                &(mesh->positions[i]),
                                &(aimesh->mVertices[i]),
                                sizeof(mesh->positions[i]));
                            // if(aimesh->HasNormals())
                            // 	memcpy(
                            // 		&(mesh->normals[i]),
                            // 		&(aimesh->mNormals[i]),
                            // 		sizeof(mesh->normals[i]));
                            // if(aimesh->HasTextureCoords(0))
                            // 	memcpy(
                            // 		&(mesh->tex_cords[i]),
                            // 		&(aimesh->mTextureCoords[0][i]),
                            // 		sizeof(mesh->tex_cords[i]));
                        }
                        mesh->vbo_set_data();
                        meshes.push_back(mesh);
                    }
                }
        } else if (init_shader(e)) {
        } else if (
            ext.compare(".bmp") == 0 |
            ext.compare(".tga") == 0 |
            ext.compare(".dds") == 0 |
            ext.compare(".png") == 0 |
            ext.compare(".jpg") == 0 |
            ext.compare(".jpeg") == 0) {
            auto t = new Texture(filename.c_str(), path.c_str());
            printf("Imported image %s\n", filename.c_str());
			textures.push_back(t);
            // }else printf(ANSI_COLOR_RED "Error SOIL importing texture %s\n" ANSI_COLOR_RESET, filename.c_str());
        }
    }
}

}  // namespace assets