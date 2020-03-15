#include "_assets.h"

namespace assets{
    //? RENDERING **************************
    vector<Mesh*> meshes;
	vector<Shader*> shaders;
	vector<Program*> programs;
	
	bool init_shader(fs::directory_entry &e){
		auto path = e.path().string();
		auto ext = boost::algorithm::to_lower_copy(e.path().extension().string());
		auto filename = boost::algorithm::to_lower_copy(e.path().filename().string());
		if(ext.compare(".glsl") == 0 || ext.compare(".vert") == 0 || ext.compare(".frag") == 0){
			SHADER_ENUM type = NOTHING;
			if(ext.compare(".frag") == 0)
				type = FRAGMENT;
			else if(ext.compare(".vert") == 0)
				type = VERTEX;
			else if(filename.find("fragment") != string::npos || filename.find("frag") != string::npos)
				type = FRAGMENT;
			else if(filename.find("geometry") != string::npos)
				type = GEOMETRY;
			else if(filename.find("vertex") != string::npos)
				type = VERTEX;
			
			if(type != NOTHING){
				printf("Importing %s as %d shader\n",filename.c_str(), type);
				auto sfile = ifstream(path.c_str());
				auto data = string((istreambuf_iterator<char>(sfile)),istreambuf_iterator<char>());
				auto shader = new Shader(type, e.path().filename().string().c_str(), data.c_str());
				shaders.push_back(shader);
			}else printf("Coudn't find type for shader %s, please update filename\n", filename.c_str());
            return true;
		}
        return false;
	}
	
	bool update_shader(string filename){
		string path;
		for(auto&e:entries){if(e.path().filename().string().compare(filename) == 0)path = e.path().string();}
		if(path.size() == 0) return false;
		
		auto lfilename = boost::algorithm::to_lower_copy(filename);
		for(auto&shader:shaders){
			if(lfilename.compare(shader->name) == 0){
				auto sfile = ifstream(path.c_str());
				auto data = string((istreambuf_iterator<char>(sfile)),istreambuf_iterator<char>());
				printf("Compiling shader for %s\n", shader->name);
				shader->update(data.c_str());	
			}
		}
		return true;
	}
    
    void import_assets(){
        Assimp::Importer importer;
		
		for(auto&e:entries){
			if(!ENTRY_IS_DIR(e)){
				auto path = e.path().string();
				auto ext = boost::algorithm::to_lower_copy(e.path().extension().string());
				auto filename = boost::algorithm::to_lower_copy(e.path().filename().string());
				if(importer.IsExtensionSupported(ext)){
					auto scene = importer.ReadFile(path.c_str(), aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
					
					printf ("Assimp %sread file %s\n", (!scene)?"coudn't ":"", path.c_str());
					if(!scene)continue;
					printf ("Has %d mesh/es, %d material/s\n", scene->mNumMeshes, scene->mNumTextures);
					
					if(scene->HasMeshes())
					for(int i=0;i<scene->mNumMeshes;++i){
						auto aimesh = scene->mMeshes[i];
						if(i==0)printf("vertex size is %d\n", sizeof(aimesh->mVertices[0]));
						// if(aimesh->HasPositions()){
						// 	Mesh mesh(aimesh->mNumVertices);
						// 	for(int i=0;i<aimesh->mNumVertices;++i){
						// 		mesh.vertices[i]=aimesh->mVertices[i].
						// 	}
						// }
						
						
					}
				}else {
					init_shader(e);
				}
			}
		}
    }
    
}