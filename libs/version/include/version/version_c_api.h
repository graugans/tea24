/*
 * Bonus 3: C-kompatible API der Versionsbibliothek.
 *
 * Dieser Header muss von einem C-Compiler UND einem C++-Compiler gelesen werden können:
 *  - kein namespace, keine Klassen, kein std::string, keine Überladung
 *  - (void) statt () für "keine Parameter" (in C bedeutet () "beliebige Parameter")
 *  - extern "C" nur für C++ sichtbar machen (__cplusplus ist nur dort definiert)
 *
 * extern "C" schaltet das Name Mangling ab:
 *   C++-API:  tea::version::fullVersion[abi:cxx11]()  ->  _ZN3tea7version11fullVersionB5cxx11Ev
 *   C-API:    version_full_c                          ->  version_full_c
 */
#ifndef TEA24_VERSION_C_API_H
#define TEA24_VERSION_C_API_H

#ifdef __cplusplus
extern "C" {
#endif

/** Komplette Versionskennung, z. B. "version_library-0.1.2" – Speicher gehört der Bibliothek */
const char* version_full_c(void);

const char* version_name_c(void);
int version_major_c(void);
int version_minor_c(void);
int version_patch_c(void);

/** 1, wenn die Bibliothek mindestens major.minor.patch ist, sonst 0 (C kennt kein bool ohne <stdbool.h>) */
int version_is_at_least_c(int major, int minor, int patch);

#ifdef __cplusplus
}
#endif

#endif /* TEA24_VERSION_C_API_H */
