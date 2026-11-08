/*
 * Bonus 2: Ein ZWEITER Nutzer der Versionsbibliothek.
 *
 * Kein kopierter Versionierungs-Code: alle Werte kommen aus tea::version.
 * Nebenbei ein Werkzeug, das man in Skripten benutzen kann:
 *
 *   ./build/bin/exercise-003-cli                    # version_library-0.1.2
 *   ./build/bin/exercise-003-cli --part minor       # 1
 *   ./build/bin/exercise-003-cli --json             # {"name": ..., "major": ...}
 *   ./build/bin/exercise-003-cli --at-least 0.2.0   # Exit-Code 0 = ja, 1 = nein
 */
#include <CLI/CLI.hpp>
#include <fmt/format.h>

#include <cstdio>
#include <nlohmann/json.hpp>
#include <string>

#include "version/version.h"

int main(int argc, char** argv)
{
    namespace version = tea::version;

    CLI::App app{"Gibt die Version der tea24-Versionsbibliothek aus"};

    std::string part{"full"};
    app.add_option("-p,--part", part, "Nur einen Teil ausgeben")
        ->check(CLI::IsMember({"full", "name", "major", "minor", "patch"}));

    bool asJson{false};
    app.add_flag("-j,--json", asJson, "Ausgabe als JSON");

    std::string atLeast;
    app.add_option("-a,--at-least", atLeast, "Exit-Code 0, wenn Version >= MAJOR.MINOR.PATCH, sonst 1");

    CLI11_PARSE(app, argc, argv);

    if (!atLeast.empty()) {
        int M{0};
        int m{0};
        int p{0};
        if (std::sscanf(atLeast.c_str(), "%d.%d.%d", &M, &m, &p) != 3) {
            fmt::println(stderr, "Ungültige Version '{}', erwartet MAJOR.MINOR.PATCH", atLeast);
            return 2;
        }
        const bool ok = version::isAtLeast(M, m, p);
        fmt::println("{} {} {}", version::fullVersion(), ok ? ">=" : "<", atLeast);
        return ok ? 0 : 1;
    }

    if (asJson) {
        const nlohmann::json json = {
            {"name", version::projectName()}, {"major", version::major()},       {"minor", version::minor()},
            {"patch", version::patch()},      {"full", version::fullVersion()},
        };
        fmt::println("{}", json.dump(2));
        return 0;
    }

    if (part == "name") {
        fmt::println("{}", version::projectName());
    } else if (part == "major") {
        fmt::println("{}", version::major());
    } else if (part == "minor") {
        fmt::println("{}", version::minor());
    } else if (part == "patch") {
        fmt::println("{}", version::patch());
    } else {
        fmt::println("{}", version::fullVersion());
    }
    return 0;
}
