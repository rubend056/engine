#include "_assets.h"

namespace assets{
    thread* inotify_thread;
    void inotify_init(){
        int argc = 0;
        static char* argv[100];for(int i=0;i<100;i++)argv[i]=(char*)calloc(100,1);
        for(int i=0;i<entries.size()+1;++i){
            // path 0 is fullpath to our assets, others are real paths to folders inside our assets
            if(i){
                auto &e = entries[i-1];
                if(ENTRY_IS_DIR(e)){
                    realpath(e.path().c_str(), argv[argc]);
                    // cout << argv[argc] << endl;
                    argc++;
                }
            }else{
                realpath(engine::project_path.c_str(), argv[argc]);
                // cout << argv[argc] << endl;
                argc++;
            }
        }
        
        if(!inotify_thread)inotify_thread = new thread(inotify::init, argc, argv);
        
        
        // Populating inotify's allowed file list
        for(auto&entry:entries){if(!ENTRY_IS_DIR(entry))inotify::filesnames_allowed.push_back(entry.path().filename().string());}
    }
}
