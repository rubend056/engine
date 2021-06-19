#include "component.h"
#include "gameobject.h"

GameObject* Component::get_parent_go(){
	if(parent != last_parent){
		if(parent)last_go = dynamic_cast<GameObject*>(parent);
		else last_go = nullptr;
		last_parent = parent;
	}
	return last_go;
}