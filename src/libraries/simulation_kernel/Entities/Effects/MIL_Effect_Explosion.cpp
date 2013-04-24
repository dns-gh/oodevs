// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Effects/MIL_Effect_Explosion.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 3 $
// $Workfile: MIL_Effect_Explosion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Effect_Explosion.h"
#include "MIL_Singletons.h"
#include "MIL_Time_ABC.h"
#include "protocol/ClientSenders.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Explosion constructor
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
MIL_Effect_Explosion::MIL_Effect_Explosion( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory, double rLifeDuration, bool neutralization )
    : MIL_Effect_Fire_ABC( surface, ammoCategory )
    , nLifeLastTimeStep_ ( MIL_Singletons::GetTime().GetCurrentTick() + (unsigned int)rLifeDuration )
    , neutralization_    ( neutralization )
    , started_           ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Explosion destructor
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
MIL_Effect_Explosion::~MIL_Effect_Explosion()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Explosion::Execute
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool MIL_Effect_Explosion::Execute()
{
    if( !started_ )
    {
        SendMsgStartEffect( neutralization_ ? sword::StartFireEffect::neutralization : sword::StartFireEffect::explosion );
        started_ = true;
    }
    if( nLifeLastTimeStep_ <= MIL_Singletons::GetTime().GetCurrentTick() )
    {
        SendMsgStopEffect();
        delete this;
        return false;
    }
    return true;
}
