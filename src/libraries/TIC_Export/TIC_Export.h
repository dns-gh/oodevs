// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-18 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/Pawn.h $
// $Author: Sbo $
// $Modtime: 20/07/05 18:32 $
// $Revision: 17 $
// $Workfile: Pawn.h $
//
// *****************************************************************************

#ifndef __TICExport_h_
#define __TICExport_h_

#ifdef ENGINE_EXPORT
#   define ENGINE_API __declspec(dllexport)
#else

#   define ENGINE_API __declspec(dllimport)
#endif

#endif // __TICExport_h_
