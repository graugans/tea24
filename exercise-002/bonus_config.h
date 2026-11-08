/*
 * Bonus 2: Include Guard absichtlich weglassen
 *
 * Experiment: Die drei Guard-Zeilen (#ifndef / #define / #endif) auskommentieren
 * und neu bauen. Da dieser Header zweimal in module.cpp landet
 * (direkt UND über module.h), meldet clang:
 *
 *   bonus_config.h: error: redefinition of 'BONUS_MAGIC'
 *   note: 'bonus_config.h' included multiple times, additional include site here
 *   note: unguarded header; consider using #ifdef guards or #pragma once
 *
 * ACHTUNG: Mit einem reinen Funktionsprototyp (z. B. `void bonus();`) gäbe es
 * KEINEN Fehler – Deklarationen dürfen beliebig oft wiederholt werden.
 * Erst eine *Definition* (hier die Konstante) macht das Problem sichtbar.
 */
#ifndef WORKSPACES_TEA24_EXERCISE_002_BONUS_CONFIG_H
#define WORKSPACES_TEA24_EXERCISE_002_BONUS_CONFIG_H

constexpr int BONUS_MAGIC = 4711;

#endif /* WORKSPACES_TEA24_EXERCISE_002_BONUS_CONFIG_H */
