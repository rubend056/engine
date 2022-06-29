
// #if(__GNUC__ < 8)
// # define OLD_GNU
// #define ENTRY_IS_DIR(e) (fs::is_directory(e.path()))
// #include <experimental/filesystem>
// namespace fs = std::experimental::filesystem;
// #else
// #define ENTRY_IS_DIR(e) (e.is_directory())
// #include <filesystem>
// namespace fs = std::filesystem;
// #endif

#if __has_include(<filesystem>)
#define ENTRY_IS_DIR(e) (e.is_directory())
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#define OLD_GNU
#define ENTRY_IS_DIR(e) (fs::is_directory(e.path()))
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
error "Missing the <filesystem> header."
#endif