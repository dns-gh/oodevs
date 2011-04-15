// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __dis_plugin_dll_config_h_
#define __dis_plugin_dll_config_h_

#ifdef DIS_PLUGIN_DLL_EXPORTS
#define DIS_PLUGIN_DLL_API extern "C" __declspec(dllexport)
#else
#define DIS_PLUGIN_DLL_API extern "C" __declspec(dllimport)
#endif

#endif // __dis_plugin_dll_config_h_
