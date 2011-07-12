// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_plugin_dll_config_h_
#define __crossbow_plugin_dll_config_h_

#ifdef CROSSBOW_PLUGIN_DLL_EXPORTS
#define CROSSBOW_PLUGIN_DLL_API extern "C" __declspec(dllexport)
#else
#define CROSSBOW_PLUGIN_DLL_API extern "C" __declspec(dllimport)
#endif

#endif // __crossbow_plugin_dll_config_h_
