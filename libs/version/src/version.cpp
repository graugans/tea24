#include "version/version.h"

#include <fmt/format.h>

#include <tuple>

#include "version_config.h"  // generiert von CMake, nur für die Bibliothek sichtbar (PRIVATE)

namespace tea::version
{

const char* projectName()
{
    return config::NAME;
}

int major()
{
    return config::MAJOR;
}

int minor()
{
    return config::MINOR;
}

int patch()
{
    return config::PATCH;
}

std::string fullVersion()
{
    return fmt::format("{}-{}.{}.{}", config::NAME, config::MAJOR, config::MINOR, config::PATCH);
}

const char* configureDate()
{
    return config::CONFIGURE_DATE;
}

bool isAtLeast(int major, int minor, int patch)
{
    /* std::tie baut Tupel aus Referenzen; deren operator>= vergleicht lexikografisch –
     * genau die Regel für Versionsnummern: 1.0.0 > 0.99.99 */
    return std::tie(config::MAJOR, config::MINOR, config::PATCH) >= std::tie(major, minor, patch);
}

}  // namespace tea::version
