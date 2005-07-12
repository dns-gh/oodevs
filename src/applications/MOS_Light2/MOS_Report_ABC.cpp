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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Report_ABC.cpp $
// $Author: Ape $
// $Modtime: 4/08/04 11:57 $
// $Revision: 1 $
// $Workfile: MOS_Report_ABC.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_Report_ABC.h"

#include "MOS_App.h"

#ifndef MOS_USE_INLINE
#   include "MOS_Report_ABC.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_Report_ABC constructor
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
MOS_Report_ABC::MOS_Report_ABC( const MT_Vector2D& vPos )
    : vPos_   ( vPos )
    , nTime_  ( MOS_App::GetApp().GetTime() )
    , bNew_   ( true )
{
    
}


// -----------------------------------------------------------------------------
// Name: MOS_Report_ABC destructor
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
MOS_Report_ABC::~MOS_Report_ABC()
{
}
