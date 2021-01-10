//
// Created by Azim Afroozeh on 1/9/21.
//

#pragma once


#define RENAME(x) x


#ifdef AVX2
#define RENAME(x) x ## _AVX2

#endif


