#include <fmt/chrono.h>
#include <fmt/format.h>

#include "config.h"
#include "version/version.h"

int main(int argc, char **argv) {

    /**
     * The {fmt} lib is a cross platform library for printing and formatting text
     * it is much more convenient than std::cout and printf
     * More info at https://fmt.dev/latest/api.html
     */
    fmt::println("Hello, {}!", tea::PROJECT_NAME);
    fmt::println("Hello, from the lib: {}!",projectName());

    fmt::println("The full version from the lib: {}",fullVersion());

    return 0; /* exit gracefully*/
}
