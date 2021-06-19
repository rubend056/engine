#ifndef source_h
#define source_h

#include "component.h"
#include "file.h"
#include "_audio.h"
// #include "SDL"


class Source: public Component, public File{
	Audio* audio;
	
	Source(FILE_CONSTRUCT_PARAM);
	~Source();
	
	void play();
	void load() override;
	
	
	IDRAW_IMGUI_NAME override{return filename();};
	// template<class Archive>
	// void serialize(Archive& ar){
	// }
};



#endif // source_h