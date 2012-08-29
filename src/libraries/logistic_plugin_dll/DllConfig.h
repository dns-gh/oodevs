// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __logistic_plugin_dll_config_h_
#define __logistic_plugin_dll_config_h_

#ifdef LOGISTIC_PLUGIN_DLL_EXPORTS
#define LOGISTIC_PLUGIN_DLL_API extern "C" __declspec(dllexport)
#else
#define LOGISTIC_PLUGIN_DLL_API extern "C" __declspec(dllimport)
#endif

#endif // __logistic_plugin_dll_config_h_
