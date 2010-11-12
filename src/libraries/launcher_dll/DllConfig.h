// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __launcher_dll_config_h_
#define __launcher_dll_config_h_

#ifdef LAUNCHER_DLL_EXPORTS
#define LAUNCHER_DLL_API __declspec(dllexport)
#else
#define LAUNCHER_DLL_API __declspec(dllimport)
#endif

#endif // __launcher_dll_config_h_
