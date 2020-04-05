#include "catch.hpp"
#include "file.h"

TEST_CASE( "File class responds", "[file]"){
	SECTION("intialized with nothing"){
		std::shared_ptr<File> f(new File);
		REQUIRE(f->data_path().compare("") == 0);
		REQUIRE(f->rel_path().compare("") == 0);
		REQUIRE(f->filename().compare("") == 0);
		REQUIRE(f->is_metadata() == false);
		REQUIRE(f->is_supposed_ext() == false);
	}
	SECTION("initialized with 'shaders/shader.glsl'"){
		std::shared_ptr<File> f(new File("shaders/shader.glsl"));
		REQUIRE(f->data_path().compare("shaders/shader.glsl") == 0);
		REQUIRE(f->rel_path().compare("shaders/shader.glsl") == 0);
		REQUIRE(f->filename().compare("shader.glsl") == 0);
		REQUIRE(f->is_metadata() == true);
		REQUIRE(f->is_supposed_ext() == true);
	}
}