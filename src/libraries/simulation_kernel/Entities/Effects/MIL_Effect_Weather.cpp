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
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectWeatherFire.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_IDManager.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Weather constructor
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
MIL_Effect_Weather::MIL_Effect_Weather( const MT_Ellipse& surface, const PHY_DotationCategory_IndirectWeatherFire& dotationCategory, double rLifeDuration, double rDeploymentDuration )
    : MIL_Effect_Fire_ABC ( surface, dotationCategory.GetIndirectFireDotationCategory() )
    , dotationCategory_( dotationCategory )
    , nDeploymentTimeStep_( MIL_Time_ABC::GetTime().GetCurrentTimeStep() + static_cast< unsigned int >( rDeploymentDuration ) )
    , nLifeLastTimeStep_  ( nDeploymentTimeStep_ + static_cast< unsigned int >( rLifeDuration ) )
    , bIsDeployed_        ( false )
{
    assert( dotationCategory.GetIndirectFireDotationCategory() == PHY_IndirectFireDotationClass::eclairant_ ||
            dotationCategory.GetIndirectFireDotationCategory() == PHY_IndirectFireDotationClass::fumigene_ );
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
    const unsigned int nCurrentTimeStep = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    if( !bIsDeployed_ && nDeploymentTimeStep_ <= nCurrentTimeStep )
    {
        MIL_AgentServer::GetWorkspace().GetMeteoDataManager().RegisterWeatherEffect( surface_, ammoCategory_ );
        SendMsgStartEffect( ammoCategory_ == PHY_IndirectFireDotationClass::fumigene_ ? sword::StartFireEffect::smoke : sword::StartFireEffect::light );
        dotationCategory_.NotifyEffectStarted( *this );
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
