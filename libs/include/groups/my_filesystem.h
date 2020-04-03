
#if(__GNUC__ < 7)
# define OLD_GNU
#define ENTRY_IS_DIR(e) (fs::is_directory(e.path()))
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#define ENTRY_IS_DIR(e) (e.is_directory())
#include <filesystem>
namespace fs = std::filesystem;
#endif