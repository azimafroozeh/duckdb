//
// Created by Azim Afroozeh on 1/9/21.
//

#pragma once


#define SET_ARCH(x) x


#ifdef AVX2
#define SET_ARCH(x) x ## _AVX2

#endif

