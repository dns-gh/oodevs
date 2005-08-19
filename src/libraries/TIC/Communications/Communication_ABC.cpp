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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/Communication_ABC.cpp $
// $Author: Sbo $
// $Modtime: 12/07/05 18:33 $
// $Revision: 4 $
// $Workfile: Communication_ABC.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"
#include "Types.h"
#include "Communication_ABC.h"
#include "CommunicationType.h"

using namespace TIC;

//-----------------------------------------------------------------------------
// Name: Communication_ABC::Communication_ABC
// Created: SBO 2005-06-28
//-----------------------------------------------------------------------------
Communication_ABC::Communication_ABC()
    : pLink_   ( 0 )
    , pType_   ( &CommunicationType::notDefined )
    , nArity_  ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: Communication_ABC::~Communication_ABC
// Created: SBO 2005-06-28
//-----------------------------------------------------------------------------
Communication_ABC::~Communication_ABC()
{
}
