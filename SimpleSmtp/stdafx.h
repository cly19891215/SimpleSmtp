// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define _DBG_							1
#define STMP_DBG_OUTPUT(format, ...)	if(_DBG_) printf("%s: %d: %s: \n"format, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);   
// TODO: reference additional headers your program requires here
