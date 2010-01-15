// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Effects/MIL_Effect_Weather.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 3 $
// $Workfile: MIL_Effect_Weather.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Effect_Weather.h"
#include "MIL_AgentServer.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "game_asn/ASN_Delete.h"

MIL_IDManager MIL_Effect_Weather::idManager_;

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Weather constructor
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
MIL_Effect_Weather::MIL_Effect_Weather( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory, MT_Float rLifeDuration, MT_Float rDeploymentDuration )
    : nID_                ( idManager_.GetFreeId() )
    , surface_            ( surface )
    , ammoCategory_       ( ammoCategory )
    , nDeploymentTimeStep_( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + (uint)rDeploymentDuration )
    , nLifeLastTimeStep_  ( nDeploymentTimeStep_ + (uint)rLifeDuration ) 
    , bIsDeployed_        ( false )
{
    assert( ammoCategory_ == PHY_IndirectFireDotationClass::eclairant_ || ammoCategory_ == PHY_IndirectFireDotationClass::fumigene_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Weather destructor
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
MIL_Effect_Weather::~MIL_Effect_Weather()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Weather::Execute
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool MIL_Effect_Weather::Execute()
{
    const uint nCurrentTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( !bIsDeployed_ && nDeploymentTimeStep_ <= nCurrentTimeStep )
    {
        MIL_AgentServer::GetWorkspace().GetMeteoDataManager().RegisterWeatherEffect( surface_, ammoCategory_ );
        SendMsgStartEffect();
        bIsDeployed_ = true;
    }

    if( bIsDeployed_ && nLifeLastTimeStep_ <= nCurrentTimeStep )
    {
        SendMsgStopEffect();
        MIL_AgentServer::GetWorkspace().GetMeteoDataManager().UnregisterWeatherEffect( surface_, ammoCategory_ );
        delete this;
        return false;
    }
    return true; 
}

//-----------------------------------------------------------------------------
// Name: MIL_Effect_Weather::SendMsgStartEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void MIL_Effect_Weather::SendMsgStartEffect() const
{
    NET_ASN_MsgStartFireEffect asnMsg;

    asnMsg().effect_oid = nID_;
    asnMsg().type       = ammoCategory_ == PHY_IndirectFireDotationClass::fumigene_ ? EnumFireEffectType::fumigene : EnumFireEffectType::eclairant;
    NET_ASN_Tools::WriteEllipse( surface_, asnMsg().location );

    asnMsg.Send();
    ASN_Delete::Delete( asnMsg().location );
}

//-----------------------------------------------------------------------------
// Name: MIL_Effect_Weather::SendMsgStopEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void MIL_Effect_Weather::SendMsgStopEffect() const
{
    NET_ASN_MsgStopFireEffect asnMsg;
    asnMsg() = nID_;
    asnMsg.Send();
}
