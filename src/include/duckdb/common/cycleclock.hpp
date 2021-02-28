//===----------------------------------------------------------------------===//
//                         DuckDB
//
//
//
//
//===----------------------------------------------------------------------===//
#pragma once

#include "duckdb/common/chrono.hpp"

namespace duckdb {

inline int64_t Now() {
#if defined(BENCHMARK_OS_MACOSX)
	return std::chrono::system_clock::now();
#elif defined(__i386__)
	int64_t ret;
	__asm__ volatile("rdtsc" : "=A"(ret));
	return ret;
#elif defined(__x86_64__) || defined(__amd64__)
	uint64_t low, high;
	__asm__ volatile("rdtsc" : "=a"(low), "=d"(high));
	return (high << 32) | low;
#elif defined(__powerpc__) || defined(__ppc__)
	int64_t tbl, tbu0, tbu1;
	asm("mftbu %0" : "=r"(tbu0));
	asm("mftb  %0" : "=r"(tbl));
	asm("mftbu %0" : "=r"(tbu1));
	tbl &= -static_cast<int64>(tbu0 == tbu1);
	return (tbu1 << 32) | tbl;
#elif defined(__sparc__)
	int64_t tick;
	asm(".byte 0x83, 0x41, 0x00, 0x00");
	asm("mov   %%g1, %0" : "=r"(tick));
	return tick;
#elif defined(__ia64__)
	int64_t itc;
	asm("mov %0 = ar.itc" : "=r"(itc));
	return itc;
#elif defined(COMPILER_MSVC) && defined(_M_IX86)
	_asm rdtsc
#elif defined(COMPILER_MSVC)
	return __rdtsc();
#elif defined(__aarch64__)
	int64_t virtual_timer_value;
	asm volatile("mrs %0, cntvct_el0" : "=r"(virtual_timer_value));
	return virtual_timer_value;
#elif defined(__ARM_ARCH)
#if (__ARM_ARCH >= 6)
	uint32_t pmccntr;
	uint32_t pmuseren;
	uint32_t pmcntenset;
	asm volatile("mrc p15, 0, %0, c9, c14, 0" : "=r"(pmuseren));
	if (pmuseren & 1) { // Allows reading perfmon counters for user mode code.
		asm volatile("mrc p15, 0, %0, c9, c12, 1" : "=r"(pmcntenset));
		if (pmcntenset & 0x80000000ul) { // Is it counting?
			asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(pmccntr));
			return static_cast<int64_t>(pmccntr) * 64; // Should optimize to << 6
		}
	}
#endif
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return static_cast<int64_t>(tv.tv_sec) * 1000000 + tv.tv_usec;
#elif defined(__mips__)
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return static_cast<int64_t>(tv.tv_sec) * 1000000 + tv.tv_usec;
#else
// The soft failover to a generic implementation is automatic only for ARM.
// For other platforms the developer is expected to make an attempt to create
// a fast implementation and use generic version if nothing better is available.
#error You need to define CycleTimer for your OS and CPU
#endif
}
} // namespace duckdb