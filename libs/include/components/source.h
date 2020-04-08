#ifndef source_h
#define source_h

#include "component.h"
#include "file.h"
#include "_audio.h"



class Source: public Component, public File{
	Audio* audio;
	
	Source(FILE_CONSTRUCT_PARAM);
	~Source();
	
	void load();
	
	IDRAW_IMGUI_NAME override{return filename();};
	// template<class Archive>
	// void serialize(Archive& ar){
	// }
};



#endif // source_h