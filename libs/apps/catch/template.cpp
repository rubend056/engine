#include "catch.hpp"

#include "gameobject.h"

TEMPLATE_TEST_CASE("File for Prefab class", "[prefab][template]", GameObject){
	auto f = std::make_shared<Prefab<TestType>>();
	// SECTION(""){
		// REQUIRE_FALSE(f->ref);
	// }
}