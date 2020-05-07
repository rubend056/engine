#ifndef referentiable_h
#define referentiable_h

#include <iostream>
#include <unordered_set>
#include <memory>
#include <vector>

#include "cereal/cereal.hpp"

class Referentiable;
typedef Referentiable* parent_type;

// ? REFERENTIABLE ###################
class Referentiable{
private:
	static std::unordered_set<unsigned int> used_ids;
	unsigned int _id=0;
	
	// Generate a unique id if current
	void gen(unsigned int wanted_id=0);
public:
	unsigned int get_id();
	// Referentiable(Referentiable* _parent=nullptr):parent(_parent){}
	virtual ~Referentiable()=default;
	
	// Parent referentiable to get the reference vector recursively
	parent_type parent=nullptr; // Backward link
private:
	// Recursive internal function to fill the vector of id's
	void _my_ref(std::vector<unsigned int>& v);
public:
	// Get vector of id's up until this object
	std::vector<unsigned int> my_ref();
	// Orpahns itself from parent, effectively destroying if no other reference
	static void orphan(const std::shared_ptr<Referentiable>& obj);

	template<class Archive>
	void serialize(Archive& ar){
		unsigned int sid=_id;
		ar(cereal::make_nvp("id", sid));
		if(sid && sid!=_id)gen(sid);
	}
};
// ?################### REFERENTIABLE

class Parent: public virtual Referentiable{
public:
	using Referentiable::Referentiable;
	std::vector<std::shared_ptr<Referentiable>> children;
	
	// Foster a child, add a child to our children, removing it from other parents
	virtual std::shared_ptr<Referentiable> foster(const std::shared_ptr<Referentiable>& child);
	
	template<class Archive>
	void serialize(Archive& ar){
		ar(cereal::make_nvp("children", children));
		ar(cereal::virtual_base_class<Referentiable>(this));
	}
	
};

#endif // referentiable_h