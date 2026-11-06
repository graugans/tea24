#include "version/version.h"
#include "version_config.h"
#include <fmt/format.h>

const char* projectName() {
    return tea::PROJECT_NAME;
}

int major() {
    return tea::PROJECT_VER_MAJOR;
}

int minor() {
    return tea::PROJECT_VER_MINOR;
}

int patch() {
    return tea::PROJECT_VER_PATCH;
}

std::string fullVersion() {
    std::string theVersion;

    theVersion = fmt::format("{}.{}.{}",
        tea::PROJECT_VER_MAJOR, 
        tea::PROJECT_VER_MINOR, 
        tea::PROJECT_VER_PATCH
    );
    return theVersion;
}