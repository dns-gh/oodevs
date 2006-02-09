// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-04 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Report_ABC.cpp $
// $Author: Ape $
// $Modtime: 4/08/04 11:57 $
// $Revision: 1 $
// $Workfile: Report_ABC.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "Report_ABC.h"

#include "App.h"

// -----------------------------------------------------------------------------
// Name: Report_ABC constructor
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
Report_ABC::Report_ABC( const MT_Vector2D& vPos )
    : vPos_   ( vPos )
    , nTime_  ( App::GetApp().GetTime() )
    , bNew_   ( true )
{
    
}


// -----------------------------------------------------------------------------
// Name: Report_ABC destructor
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
Report_ABC::~Report_ABC()
{
}
