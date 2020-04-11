#include "catch.hpp"

#include "gameobject.h"
#include "transform.h"

TEST_CASE("Testing GameObject"){
	auto go = std::make_shared<GameObject>();
	go->add(std::make_shared<Transform>(), typeid(Transform).name());
	REQUIRE(go->get_comps<Transform>().size() == 1);
	REQUIRE(go->components.size() == 1);
}