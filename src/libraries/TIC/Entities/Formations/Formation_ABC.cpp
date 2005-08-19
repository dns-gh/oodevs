// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Formations/Formation_ABC.cpp $
// $Author: Sbo $
// $Modtime: 20/06/05 17:07 $
// $Revision: 2 $
// $Workfile: Formation_ABC.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"
#include "Entities/Formations/Formation_ABC.h"
#include "Entities/Pawn.h"
#include "Entities/Platform.h"
#include "Tools/Position.h"

using namespace TIC;

//-----------------------------------------------------------------------------
// Name: Formation_ABC::Formation_ABC
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
Formation_ABC::Formation_ABC()
    : pPawn_         ( 0 )
    , nPlatformNbr_  ( 0 )
    , bIsReverse_    ( false )
{
    // nothing
}

//-----------------------------------------------------------------------------
// Name: Formation_ABC::~Formation_ABC
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
Formation_ABC::~Formation_ABC()
{
    // nothing
}
