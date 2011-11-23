// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __bml_plugin_dll_config_h_
#define __bml_plugin_dll_config_h_

#ifdef BML_PLUGIN_DLL_EXPORTS
# define BML_PLUGIN_DLL_API extern "C" __declspec(dllexport)
#else
# define BML_PLUGIN_DLL_API extern "C" __declspec(dllimport)
#endif

#endif // __bml_plugin_dll_config_h_
