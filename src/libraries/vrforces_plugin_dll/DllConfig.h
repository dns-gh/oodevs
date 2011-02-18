// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __vrforces_plugin_dll_config_h_
#define __vrforces_plugin_dll_config_h_

#ifdef VRFORCES_PLUGIN_DLL_EXPORTS
#define VRFORCES_PLUGIN_DLL_API extern "C" __declspec(dllexport)
#else
#define VRFORCES_PLUGIN_DLL_API extern "C" __declspec(dllimport)
#endif

#endif // __vrforces_plugin_dll_config_h_
