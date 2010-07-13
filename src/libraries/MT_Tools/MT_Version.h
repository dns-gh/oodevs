// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_Version_h_
#define __MT_Version_h_

#ifndef VERSION
#   define VERSION "dev version"
#endif

#ifdef _DEBUG
#   define MT_COMPILE_TYPE "Debug"
#else
#   define MT_COMPILE_TYPE "Release"
#endif

#endif // __MT_Version_h_
