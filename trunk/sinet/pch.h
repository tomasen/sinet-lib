// pch.h for precompiled headers
#ifndef PCH_H
#define PCH_H

#include <string>
#include <vector>
#include <map>

#ifdef WIN32

#ifndef WINVER
#define WINVER 0x0400
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#endif // WIN32

#ifdef _MAC_

#include <pthread.h>
#include <time.h>
#include <errno.h>

#endif

#endif // PCH_H