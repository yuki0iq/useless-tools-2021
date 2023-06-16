#ifndef _BASE_WINDOWS_H_
#define _BASE_WINDOWS_H_

#include "compat.h"

#ifdef WIN_RUNTIME
#define DONT_USE_AFX

#   if !defined(NOMINMAX)
#       define NOMINMAX
#   endif

#   if defined DONT_USE_AFX || defined(__MINGW32__)
/*
#       if !defined(WIN32_LEAN_AND_MEAN)
#           define WIN32_LEAN_AND_MEAN
#       endif
*/
#       define _WIN32_DCOM
#       define _WINSOCKAPI_
#       include <windows.h>

#   else

#       ifndef WINVER
#           define WINVER 0x0400
#       endif // WINVER
#       include <afx.h>

#   endif // DONT_USE_AFX

#endif // defined(WIN_RUNTIME)

#endif // _BASE_WINDOWS_H_
