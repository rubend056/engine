// int main(){
// 	return 0;
// }


#define CATCH_CONFIG_MAIN
#include "catch.hpp"



#include "file.h"

TEST_CASE( "File class responds", "[file]"){
	SECTION("intialized with nothing"){
		std::shared_ptr<File> f(new File);
		REQUIRE(f->data_path().compare("") == 0);
		REQUIRE(f->filename().compare("") == 0);
		REQUIRE(f->is_metadata() == false);
		REQUIRE(f->is_supposed_ext() == false);
		REQUIRE(f->_rel_path.compare("") == 0);
		f->create_supposed_ext();
		REQUIRE(f->data_path().compare("") == 0);
		REQUIRE(f->filename().compare("") == 0);
		REQUIRE(f->is_metadata() == false);
		REQUIRE(f->is_supposed_ext() == false);
		REQUIRE(f->_rel_path.compare("") == 0);
	}
	SECTION("initialized with 'shaders/shader.glsl'"){
		std::shared_ptr<File> f(new File("shaders/shader.glsl"));
		REQUIRE(f->data_path().compare("shaders/shader.glsl") == 0);
		REQUIRE(f->filename().compare("shader.glsl") == 0);
		REQUIRE(f->is_metadata() == false);
		REQUIRE(f->is_supposed_ext() == false);
		REQUIRE(f->_rel_path.compare("shaders/shader.glsl") == 0);
		f->create_supposed_ext();
		REQUIRE(f->data_path().compare("shaders/shader.glsl") == 0);
		REQUIRE(f->filename().compare("shader.glsl") == 0);
		REQUIRE(f->is_metadata() == true);
		REQUIRE(f->is_supposed_ext() == true);
		REQUIRE(f->_rel_path.compare("shaders/shader.glsl.meta") == 0);
	}
}


#include "gameobject.h"
#include "transform.h"

TEST_CASE("Testing GameObject"){
	{
		auto go = std::make_shared<GameObject>("Test");
		REQUIRE(go->_rel_path.compare("Test") == 0);
	}
	
	{
		auto go = std::make_shared<GameObject>();
		go->add(std::make_shared<Transform>());
		REQUIRE(go->get_comps<Transform>().size() == 1);
		REQUIRE(go->components.size() == 1);
	}
	
}



#include "scene.h"
#include "transform.h"

TEST_CASE("Scene class behaviour"){
	SECTION("empty args"){
		auto s = std::make_shared<Scene>();
		REQUIRE(s->is_supposed_ext() == false);
		REQUIRE(s->is_metadata() == false);
		REQUIRE(s->_rel_path.compare("") == 0);
	}
	SECTION("initializing new scene, providing path"){
		auto s = std::make_shared<Scene>("scenes/test");
		REQUIRE(s->is_supposed_ext() == false);
		REQUIRE(s->is_metadata() == false);
		REQUIRE(s->_rel_path.compare("scenes/test") == 0);
		s->create_supposed_ext();
		REQUIRE(s->is_supposed_ext() == true);
		REQUIRE(s->is_metadata() == false);
		REQUIRE(s->_rel_path.compare("scenes/test.scene") == 0);
	}
	SECTION("gamobejct path"){
		auto s = std::make_shared<Scene>("scenes/test");
		s->create_supposed_ext();
		assets::add(s);
		s->foster(std::make_shared<GameObject>());
		
		REQUIRE(s->children.size() == 1);
		
		// REQUIRE(go_paths[0].compare("scenes/test.scene") == 0);
		// REQUIRE(go_paths[1].compare("go") == 0);
		// REQUIRE(assets::data_path_find(go_paths) == go);
	}
}