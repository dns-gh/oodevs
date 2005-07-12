// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/PHY_Action_ABC.cpp $
// $Author: Nld $
// $Modtime: 21/10/04 10:19 $
// $Revision: 2 $
// $Workfile: PHY_Action_ABC.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_Action_ABC.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_Action_ABC::PHY_Action_ABC( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : bSuspended_( false )
    , pion_      ( pion )
{
    pion_.RegisterAction( *this );
    diaCall.GetResult().SetValue( (void*)this, &DEC_Tools::GetTypeAction() );
}

// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
PHY_Action_ABC::PHY_Action_ABC( MIL_AgentPion& pion )
    : bSuspended_( false )
    , pion_      ( pion )
{
    pion_.RegisterAction( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_Action_ABC::~PHY_Action_ABC()
{
    pion_.UnregisterAction( *this );
}


