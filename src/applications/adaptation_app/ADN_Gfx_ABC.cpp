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
#include "ADN_Connector_ABC.h"

//-----------------------------------------------------------------------------
// Name: ADN_Gfx_ABC constructor
// Created: JDY 03-07-10
//-----------------------------------------------------------------------------
ADN_Gfx_ABC::ADN_Gfx_ABC( bool bAuto )
    : bAutoEnabled_( bAuto )
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

//-----------------------------------------------------------------------------
// Name: ADN_Gfx_ABC::GetConnector
// Created: JDY 03-07-10
//-----------------------------------------------------------------------------
ADN_Connector_ABC& ADN_Gfx_ABC::GetConnector()
{
    return *pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Gfx_ABC::SetAutoEnabled
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
void ADN_Gfx_ABC::SetAutoEnabled( bool bAuto )
{
    bAutoEnabled_ = bAuto;
}

//-----------------------------------------------------------------------------
// Name: ADN_Gfx_ABC::IsAutoEnabled
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
bool ADN_Gfx_ABC::IsAutoEnabled()
{
    return bAutoEnabled_;
}
