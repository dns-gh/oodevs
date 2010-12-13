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
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_IDManager.h"

MIL_IDManager MIL_Effect_Weather::idManager_;

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Weather constructor
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
MIL_Effect_Weather::MIL_Effect_Weather( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory, double rLifeDuration, double rDeploymentDuration )
    : nID_                ( idManager_.GetFreeId() )
    , surface_            ( surface )
    , ammoCategory_       ( ammoCategory )
    , nDeploymentTimeStep_( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + (unsigned int)rDeploymentDuration )
    , nLifeLastTimeStep_  ( nDeploymentTimeStep_ + (unsigned int)rLifeDuration )
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
    const unsigned int nCurrentTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( !bIsDeployed_ && nDeploymentTimeStep_ <= nCurrentTimeStep )
    {
        MIL_AgentServer::GetWorkspace().GetMeteoDataManager().RegisterWeatherEffect( surface_, ammoCategory_ );
        SendStartEffect();
        bIsDeployed_ = true;
    }

    if( bIsDeployed_ && nLifeLastTimeStep_ <= nCurrentTimeStep )
    {
        SendStopEffect();
        MIL_AgentServer::GetWorkspace().GetMeteoDataManager().UnregisterWeatherEffect( surface_, ammoCategory_ );
        delete this;
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_Effect_Weather::SendStartEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void MIL_Effect_Weather::SendStartEffect() const
{
    client::StartFireEffect asnMsg;

    asnMsg().mutable_fire_effect()->set_id( nID_ );
    asnMsg().set_type( ammoCategory_ == PHY_IndirectFireDotationClass::fumigene_ ? sword::smoke : sword::light );
    NET_ASN_Tools::WriteEllipse( surface_, *asnMsg().mutable_location() );

    asnMsg.Send( NET_Publisher_ABC::Publisher() );
    asnMsg().clear_location();
}

//-----------------------------------------------------------------------------
// Name: MIL_Effect_Weather::SendStopEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void MIL_Effect_Weather::SendStopEffect() const
{
    client::StopFireEffect asnMsg;
    asnMsg().mutable_fire_effect()->set_id( nID_ );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}
