/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file common/maths.h
 *  Mathematical helpers.
 */

#ifndef COMMON_MATHS_H
#define COMMON_MATHS_H

#include <cmath>

#include "common/system.h"

#ifndef M_SQRT1_2
	#define M_SQRT1_2 0.70710678118654752440 /* 1/sqrt(2) */
#endif

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

extern const float sinTable16[8];
extern const float sinTable32[16];
extern const float sinTable64[32];
extern const float sinTable128[64];
extern const float sinTable256[128];
extern const float sinTable512[256];
extern const float sinTable1024[512];
extern const float sinTable2048[1024];
extern const float sinTable4096[2048];
extern const float sinTable8192[4096];
extern const float sinTable16384[8192];
extern const float sinTable32768[16384];
extern const float sinTable65536[32768];

extern const float cosTable16[8];
extern const float cosTable32[16];
extern const float cosTable64[32];
extern const float cosTable128[64];
extern const float cosTable256[128];
extern const float cosTable512[256];
extern const float cosTable1024[512];
extern const float cosTable2048[1024];
extern const float cosTable4096[2048];
extern const float cosTable8192[4096];
extern const float cosTable16384[8192];
extern const float cosTable32768[16384];
extern const float cosTable65536[32768];

namespace Common {

struct Complex {
	float re, im;
};

const float *getSineTable(int bits);
const float *getCosineTable(int bits);

} // End of namespace Common

#endif // COMMON_MATHS_H
