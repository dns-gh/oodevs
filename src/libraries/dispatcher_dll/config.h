// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __dispatcher_dll_config_h_
#define __dispatcher_dll_config_h_

#ifdef DISPATCHER_DLL_EXPORTS
#define DISPATCHER_DLL_API __declspec(dllexport)
#else
#define DISPATCHER_DLL_API __declspec(dllimport)
#endif

#endif // __dispatcher_dll_config_h_
