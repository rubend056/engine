#include <cstring>
#include <fstream>
#include <set>
#include <string>

#include "my_filesystem.h"

#define FILENAME_SIZE 30
#define METADATA_EXT ".meta"

#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"



struct File {
   public:
	unsigned int file_id = 0;
	fs::path rel_path = "";

	template <class Archive>
	void serialize(Archive& ar) {
		ar(CEREAL_NVP(file_id));
		ar(CEREAL_NVP(rel_path));
	}

	// ? Constructors
	File(const fs::path _rel_path) {
		rel_path = _rel_path;
	}
	File() {}
	virtual ~File() {}
};

class File2 {
   public:
	File2(const int j = 0) {}
	virtual ~File2() {}
	int file_id = 0;

	template <class Archive>
	void serialize(Archive& ar) {
		ar(CEREAL_NVP(file_id));
		// file_ids.insert(file_id);
	}
};
class Test
	: public File2 {
	int j;

   public:
	virtual void testing(){};
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<File2>(this));
		ar(j);
	}
};
// CEREAL_REGISTER_TYPE(Test)



int main() {
	auto j = std::shared_ptr<File2>(new Test);
	auto f = std::shared_ptr<File>(new File("Testing123"));
	cereal::JSONOutputArchive o(std::cout);
	// o(f);
	return 0;
}

// #include ""
// #include <fstream>
