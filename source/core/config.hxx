#pragma once

#if defined(_MSC_VER)
	#define COMPILER "MSVC"
	#define CPP_VERSION _MSVC_LANG
#elif defined(__GNUC__) && !defined(__clang__)
	#define COMPILER "g++"
	#define CPP_VERSION __cplusplus
#elif defined(__clang__)
	#define COMPILER "clang++"
	#define CPP_VERSION __cplusplus
#else
	#define COMPILER "unknown"
	#define CPP_VERSION __cplusplus
#endif

#if(CPP_VERSION >= 202302L)
	#define CPP "c++23"
#elif(CPP_VERSION >= 202002L)
	#define CPP "c++20"
#elif(CPP_VERSION >= 201703L)
	#define CPP "c++17"
#elif(CPP_VERSION >= 201402L)
	#define CPP "c++14"
#elif(CPP_VERSION >= 201103L)
	#define CPP "c++11"
#elif(CPP_VERSION >= 199711L)
	#define CPP "c++98"
#else
	#define CPP "c++"
#endif
