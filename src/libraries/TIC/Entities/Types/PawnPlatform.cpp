// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-17 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PawnPlatform.cpp $
// $Author: Sbo $
// $Modtime: 24/06/05 17:22 $
// $Revision: 1 $
// $Workfile: PawnPlatform.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"

#include "PawnPlatform.h"
#include "PlatformType.h"


using namespace TIC;

// -----------------------------------------------------------------------------
// Name: PawnPlatform constructor
// Created: SBO 2005-06-24
// -----------------------------------------------------------------------------
PawnPlatform::PawnPlatform( const PlatformType& platformType, bool bIsEmbarquable )
    : pType_          ( &platformType )
    , bIsEmbarquable_ ( bIsEmbarquable )
{
    // nothing
}

// -----------------------------------------------------------------------------
// Name: PawnPlatform destructor
// Created: SBO 2005-06-24
// -----------------------------------------------------------------------------
PawnPlatform::~PawnPlatform()
{
    // nothing
}