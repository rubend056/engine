#include "catch.hpp"

#include "scene.h"

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
}