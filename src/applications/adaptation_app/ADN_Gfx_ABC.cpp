//*****************************************************************************
//
// $Created: JDY 03-07-10 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Gfx_ABC.cpp $
// $Author: Ape $
// $Modtime: 10/02/05 15:46 $
// $Revision: 2 $
// $Workfile: ADN_Gfx_ABC.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Gfx_ABC.h"

//-----------------------------------------------------------------------------
// Name: ADN_Gfx_ABC constructor
// Created: JDY 03-07-10
//-----------------------------------------------------------------------------
ADN_Gfx_ABC::ADN_Gfx_ABC( bool bAuto )
    : pConnector_( 0 )
    , bAutoEnabled_( bAuto )
    , bEnabledOnlyInAdminMode_( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Gfx_ABC destructor
// Created: JDY 03-07-10
//-----------------------------------------------------------------------------
ADN_Gfx_ABC::~ADN_Gfx_ABC()
{
    // NOTHING
}
