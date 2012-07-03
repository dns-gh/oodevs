// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef WEB_CONTROL_PLUGIN_DLL_H__
#define WEB_CONTROL_PLUGIN_DLL_H__

#ifdef WEB_CONTROL_PLUGIN_DLL_EXPORTS
#define WEB_CONTROL_PLUGIN_DLL_API extern "C" __declspec(dllexport)
#else
#define WEB_CONTROL_PLUGIN_DLL_API extern "C" __declspec(dllimport)
#endif

#endif // WEB_CONTROL_PLUGIN_DLL_H__
