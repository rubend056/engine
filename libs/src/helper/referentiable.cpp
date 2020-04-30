#include "referentiable.h"
#include "debug.h"

#include <algorithm>

std::unordered_set<unsigned int> Referentiable::used_ids;

unsigned int Referentiable::get_id(){if(!_id)gen();return _id;};

void Referentiable::gen(unsigned int wanted_id){
	// If id > 0 erase it from set
	if(_id)used_ids.erase(_id);
	// Set the id to the id we want, or just rand()
	if(wanted_id)_id = wanted_id;
	else _id=rand()+1;
	// Limits the execution to find a uniqueid to 10 times
	int max_it=10;
	// Generate random id until that id doesn't exist in set
	while((used_ids.find(_id) != used_ids.end() || !_id) && max_it > 0)
		{_id=rand()+1;--max_it;}
	
	// Return if there were any errors
	if(!max_it){printf(ANSI_COLOR_RED "ERROR max_it == 0" ANSI_COLOR_RESET "\n");return;}
	if(!_id){printf(ANSI_COLOR_RED "id is zero" ANSI_COLOR_RESET "\n");return;}
	// Insert the found id
	used_ids.insert(_id);
}
void Referentiable::_my_ref(std::vector<unsigned int>& v){
	if(parent)parent->_my_ref(v); // If I have a parent, push back parent id
	v.push_back(get_id()); // Then push back my ID
}
std::vector<unsigned int> Referentiable::my_ref(){
	std::vector<unsigned int> v;
	_my_ref(v);
	return v;
}
void Referentiable::orphan(const std::shared_ptr<Referentiable>& obj){
	if(obj && obj->parent)
	if(auto p = dynamic_cast<Parent*>(obj->parent)){
		auto it = std::find(p->children.begin(),p->children.end(), obj);
		if(it!=p->children.end())p->children.erase(it);
	}
}

// void Parent::orphan(const std::shared_ptr<Referentiable>& child){
// 	if(child && child->parent)
// 	if(auto p = dynamic_cast<Parent*>(child->parent))
// 		p->children.erase(
// 			// std::find_if(p->children.begin(),p->children.end(), 
// 			// [&](const std::shared_ptr<Referentiable>& r){
// 			// 	return r == child;
// 			// })
// 			std::find(p->children.begin(),p->children.end(), child)
// 		);
// }

std::shared_ptr<Referentiable> Parent::foster(const std::shared_ptr<Referentiable>& child){
	if(!child || (child && (child.get() == this || child->parent == this)))return std::shared_ptr<Referentiable>();
	Referentiable::orphan(child);
	children.push_back(child);
	child->parent = this;
	return child;
}