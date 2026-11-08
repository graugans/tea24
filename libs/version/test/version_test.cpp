/*
 * Bonus 1: Unit-Tests für die Versionsbibliothek (Catch2 v3).
 *
 * Die Tests verwenden nur die öffentliche API – sie gehen nicht von einer festen Versionsnummer
 * aus. So bleiben sie grün, wenn in libs/version/CMakeLists.txt die Version erhöht wird.
 *
 *   cmake --build build --target version_library_test
 *   ./build/bin/version_library_test            # oder: ctest --test-dir build -R version
 */
#include <catch2/catch_test_macros.hpp>

#include <string>

#include "version/version.h"
#include "version/version_c_api.h"

using namespace tea::version;

TEST_CASE("Versionsbestandteile sind plausibel", "[version]")
{
    CHECK(major() >= 0);
    CHECK(minor() >= 0);
    CHECK(patch() >= 0);
    CHECK(std::string{projectName()} == "version_library");
}

TEST_CASE("fullVersion hat das Format name-major.minor.patch", "[version]")
{
    const auto expected = std::string{projectName()} + "-" + std::to_string(major()) + "." +
                          std::to_string(minor()) + "." + std::to_string(patch());
    CHECK(fullVersion() == expected);
}

TEST_CASE("isAtLeast vergleicht lexikografisch", "[version][isAtLeast]")
{
    const int M = major();
    const int m = minor();
    const int p = patch();

    SECTION("exakt die eigene Version")
    {
        CHECK(isAtLeast(M, m, p));
    }
    SECTION("ältere Versionen")
    {
        CHECK(isAtLeast(0, 0, 0));
        CHECK(isAtLeast(M, m, p - 1));
        CHECK(isAtLeast(M - 1, 99, 99));  // major schlägt alles dahinter
        CHECK(isAtLeast(M, m - 1, 99));   // minor schlägt patch
    }
    SECTION("neuere Versionen")
    {
        CHECK_FALSE(isAtLeast(M, m, p + 1));
        CHECK_FALSE(isAtLeast(M, m + 1, 0));  // kleinere patch-Nummer hilft nicht
        CHECK_FALSE(isAtLeast(M + 1, 0, 0));
    }
}

TEST_CASE("C-API liefert dieselben Werte wie die C++-API", "[version][c-api]")
{
    CHECK(std::string{version_full_c()} == fullVersion());
    CHECK(std::string{version_name_c()} == projectName());
    CHECK(version_major_c() == major());
    CHECK(version_minor_c() == minor());
    CHECK(version_patch_c() == patch());
    CHECK(version_is_at_least_c(major(), minor(), patch()) == 1);
    CHECK(version_is_at_least_c(major() + 1, 0, 0) == 0);

    /* Zeiger muss stabil bleiben (static-Variable in version_full_c) */
    CHECK(version_full_c() == version_full_c());
}
