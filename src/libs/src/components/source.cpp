#include "components/source.h"

#include "audio.h"

Source::Source(const fs::path& rpath) : File(FILE_CONSTRUCT_VARS) {
	initAudio();
	if (!rpath.empty())
		load();
}
Source::~Source() { freeAudio(audio); }

void Source::load() {
	audio = createAudio(data_path().c_str(), false, SDL_MIX_MAXVOLUME);
}

void Source::play() {
	playMusicFromMemory(audio, SDL_MIX_MAXVOLUME);
}