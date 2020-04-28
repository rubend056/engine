#include "catch.hpp"

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