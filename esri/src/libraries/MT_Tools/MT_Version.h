// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2003-09-05 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Version.h $
// $Author: Nld $
// $Modtime: 11/08/04 15:32 $
// $Revision: 2 $
// $Workfile: MT_Version.h $
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
