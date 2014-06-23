// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DotationCategory_IndirectFire_ABC.h"
#include "MIL_AgentServer.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Effects/MIL_Effect_DetectionRange.h"
#include "Entities/Orders/MIL_Report.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_terrain/TER_World.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::PHY_DotationCategory_IndirectFire_ABC
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC::PHY_DotationCategory_IndirectFire_ABC( const PHY_IndirectFireDotationClass& category, const PHY_DotationCategory& dotationCategory,
                                                                              unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange )
    : category_( category )
    , dotationCategory_( dotationCategory )
    , nInterventionType_( nInterventionType )
    , rDispersionX_( rDispersionX )
    , rDispersionY_( rDispersionY )
    , rDetectionRange_( rDetectionRange )
{
    rDispersionX_ = MIL_Tools::ConvertMeterToSim( rDispersionX_ );
    rDispersionY_ = MIL_Tools::ConvertMeterToSim( rDispersionY_ );
    rDetectionRange_ = MIL_Tools::ConvertMeterToSim( rDetectionRange_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC::~PHY_DotationCategory_IndirectFire_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::GetDotationCategory
// Created: NLD 2006-08-07
// -----------------------------------------------------------------------------
const PHY_DotationCategory& PHY_DotationCategory_IndirectFire_ABC::GetDotationCategory() const
{
    return dotationCategory_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::GetIndirectFireDotationCategory
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
const PHY_IndirectFireDotationClass& PHY_DotationCategory_IndirectFire_ABC::GetIndirectFireDotationCategory() const
{
    return category_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::HasHit
// Created: NLD 2005-08-04
// -----------------------------------------------------------------------------
bool PHY_DotationCategory_IndirectFire_ABC::HasHit( const MIL_Agent_ABC& /*target*/, double ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::ApplyStrikeEffect
// Created: MGD 2010-02-16
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectFire_ABC::ApplyStrikeEffect( const MIL_Agent_ABC& /*firer*/, MIL_Agent_ABC& /*target*/, double /*rInterventionTypeFired*/, PHY_FireResults_ABC& /*fireResult*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::ApplyDetectionRangeEffect
// Created: JSR 2013-04-26
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectFire_ABC::ApplyDetectionRangeEffect( const MT_Vector2D& vTargetPosition, const std::vector< unsigned int >& fireEffectsIds ) const
{
    if( !MIL_AgentServer::IsInitialized() )
        return;
    TER_Agent_ABC::T_AgentPtrVector perceivers;
    TER_World::GetWorld().GetAgentManager().GetListWithinCircle( vTargetPosition, rDetectionRange_, perceivers );
    std::vector< unsigned int > unitsIds;
    for( auto it = perceivers.begin(); it != perceivers.end(); ++it )
    {
        unitsIds.push_back( static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent().GetID() );
        MIL_Agent_ABC& agent = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
        if( dotationCategory_.IsIED() )
            MIL_Report::PostEvent( agent, report::eRC_IEDHeard );
    }
    MIL_EffectManager::GetEffectManager().Register( *new MIL_Effect_DetectionRange( unitsIds, dotationCategory_.GetMosID(), fireEffectsIds ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire_ABC::GetSmokeDuration
// Created: LDC 2013-08-30
// -----------------------------------------------------------------------------
double PHY_DotationCategory_IndirectFire_ABC::GetSmokeDuration() const
{
    return 0.;
}
