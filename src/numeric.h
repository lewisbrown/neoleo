#pragma once

#include <stdint.h>

#ifdef __cplusplus
//typedef std::decimal::decimal64 num_cc;
//typedef __decfloat64 num_cc;
#define USE_DECIMAL 1
//#include <decimal/decimal.h>
//typedef __d64 _Decimal64;
#else
//#typedef _Decimal64 num_c;
#define USE_DECIMAL 0
#endif

#if USE_DECIMAL

	#ifdef __cplusplus
		#include <decimal/decimal>
		typedef std::decimal::decimal64::__decfloat64 _num_t;
	#else
		typedef _Decimal64 _num_t;
	#endif
	#define E100 1E100DD
	#define NUM_HUNDREDTH 0.01DD
	#define NUM_TEN 10.0DD
#else
	typedef double _num_t;
	#define E100 1E100
	#define NUM_HUNDREDTH 0.01
	#define NUM_TEN 10.0
#endif

typedef _num_t num_t;	
//typedef  uint8_t num_t[8];

//typedef struct { uint8_t bytes[8]; }  num_s;
//typedef  uint8_t num_s[8];
