// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Effects/MIL_Effect_IndirectFire.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 14:58 $
// $Revision: 7 $
// $Workfile: MIL_Effect_IndirectFire.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Effect_IndirectFire.h"
#include "MIL_AgentServer.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponDataType_IndirectFire.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire constructor
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
MIL_Effect_IndirectFire::MIL_Effect_IndirectFire( const MIL_AgentPion& firer, uint nTargetKnowledgeID, const PHY_DotationCategory_IndirectFire_ABC& indirectDotationCategory, MT_Float rInterventionTypeToFire )
    : nNbrRefs_                ( 0 )
    , firer_                   ( firer )
    , rInterventionTypeToFire_ ( rInterventionTypeToFire )
    , indirectDotationCategory_( indirectDotationCategory )
    , vSourcePosition_         ( firer.GetRole< PHY_RoleInterface_Location >().GetPosition() )
    , vTargetPosition_         ( -1., -1. )
    , nTargetKnowledgeID_      ( nTargetKnowledgeID )
    , nNbrAmmoFired_           ( 0 )
    , bIsFlying_               ( false )
    , bFired_                  ( false )
    , rImpactTimeStep_         ( 0. )
    , pFireResult_             ( 0 )    
{
    UpdateTargetPositionFromKnowledge(); /// Update vTargetPosition_
    IncRef();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire constructor
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
MIL_Effect_IndirectFire::MIL_Effect_IndirectFire( const MIL_AgentPion& firer, const MT_Vector2D& vTargetPosition, const PHY_DotationCategory_IndirectFire_ABC& indirectDotationCategory, MT_Float rInterventionTypeToFire )
    : nNbrRefs_                ( 0 )
    , firer_                   ( firer )
    , rInterventionTypeToFire_ ( rInterventionTypeToFire )
    , indirectDotationCategory_( indirectDotationCategory )
    , vSourcePosition_         ( firer.GetRole< PHY_RoleInterface_Location >().GetPosition() )
    , vTargetPosition_         ( vTargetPosition )
    , nTargetKnowledgeID_      ( 0 )
    , nNbrAmmoFired_           ( 0 )
    , bIsFlying_               ( false )
    , bFired_                  ( false )
    , rImpactTimeStep_         ( 0. )
    , pFireResult_             ( 0 )
{
    IncRef();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire destructor
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
MIL_Effect_IndirectFire::~MIL_Effect_IndirectFire()
{
    StopFlying();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::UpdateTargetPositionFromKnowledge
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void MIL_Effect_IndirectFire::UpdateTargetPositionFromKnowledge()
{
    if( nTargetKnowledgeID_ == 0 )
        return;
    boost::shared_ptr< DEC_Knowledge_Agent > pTargetKnowledge = firer_.GetKnowledgeGroup().GetKnowledge().GetKnowledgeAgentFromID( nTargetKnowledgeID_ );
    if( !pTargetKnowledge || !pTargetKnowledge->IsValid() )
    {
        nTargetKnowledgeID_ = 0;
        return;
    }
    vTargetPosition_ = pTargetKnowledge->GetPosition();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::CanWeaponBeUsed
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool MIL_Effect_IndirectFire::CanWeaponBeUsed( const PHY_Weapon& weapon ) const
{
    assert( !bIsFlying_ );

    if( !weapon.CanIndirectFire() )
        return false;

    if( weapon.GetDotationCategory() != indirectDotationCategory_.GetDotationCategory() )
        return false;

    const MT_Float rFlyingDist = vSourcePosition_.Distance( vTargetPosition_ );
    if( rFlyingDist < weapon.GetMinRangeToIndirectFire() || rFlyingDist > weapon.GetMaxRangeToIndirectFire() )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::NotifyAmmoFired
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void MIL_Effect_IndirectFire::NotifyAmmoFired( const PHY_WeaponDataType_IndirectFire& weaponType, uint nNbrAmmoReserved )
{
    assert( !bIsFlying_ );
      
    UpdateTargetPositionFromKnowledge();

    const MT_Float rNewTimeBeforeImpact = vSourcePosition_.Distance( vTargetPosition_ ) / weaponType.GetAverageSpeed();
    rImpactTimeStep_ = std::max( rImpactTimeStep_, rNewTimeBeforeImpact + MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() );
    
    nNbrAmmoFired_ += nNbrAmmoReserved;
    if( indirectDotationCategory_.ConvertToInterventionType( nNbrAmmoFired_ ) >= rInterventionTypeToFire_ )
        StartFlying();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::Execute
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool MIL_Effect_IndirectFire::Execute()
{
    if( !IsTargetValid() || rInterventionTypeToFire_ == 0. )
    {
        StopFlying();
        DecRef();
        return false;
    }

    if( !bIsFlying_ )
        return true;

    if( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() < rImpactTimeStep_ )
        return true;

    if( nNbrAmmoFired_ > 0 )
    {
        UpdateTargetPositionFromKnowledge();
        assert( pFireResult_ );
        indirectDotationCategory_.GetDotationCategory().ApplyIndirectFireEffect( firer_, vSourcePosition_, vTargetPosition_, nNbrAmmoFired_, *pFireResult_ );
    }
    StopFlying();
    DecRef();
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::StartFlying
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void MIL_Effect_IndirectFire::StartFlying()
{
    bFired_ = true;
    if( !pFireResult_ )
        pFireResult_ = new PHY_FireResults_Pion( firer_, vTargetPosition_, indirectDotationCategory_.GetDotationCategory() );

    if( !bIsFlying_ )
    {
        bIsFlying_ = true;
        MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().RegisterFlyingShell( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::StopFlying
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void MIL_Effect_IndirectFire::StopFlying() 
{
    if( bIsFlying_ )
    {
        bIsFlying_ = false;
        MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().UnregisterFlyingShell( *this );
    }

    if( pFireResult_ )
    {
        delete pFireResult_;
        pFireResult_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::GetNbrAmmoToCompleteInterventionType
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
uint MIL_Effect_IndirectFire::GetNbrAmmoToCompleteInterventionType() const
{
    if( bFired_ )
        return 0;

    MT_Float rTmp = indirectDotationCategory_.ConvertToNbrAmmo( rInterventionTypeToFire_ ) - nNbrAmmoFired_;
    if( rTmp <= 0. )
        return 0;
    return std::max( (uint)1, (uint)rTmp );
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::IsFlyingThroughLocalisation
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
bool MIL_Effect_IndirectFire::IsFlyingThroughLocalisation( const TER_Localisation& localisation ) const
{
    if( !bIsFlying_ )
        return false;
    assert( pFireResult_ );
    MT_Line lineTmp( vSourcePosition_, vTargetPosition_ );
    return localisation.Intersect2D( lineTmp );
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::GetFireID
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
uint MIL_Effect_IndirectFire::GetFireID() const
{
    assert( pFireResult_ );
    return pFireResult_->GetID();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::GetIndirectDotationCategory
// Created: NLD 2006-08-07
// -----------------------------------------------------------------------------
const PHY_DotationCategory_IndirectFire_ABC& MIL_Effect_IndirectFire::GetIndirectDotationCategory() const
{
    return indirectDotationCategory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::IsTargetValid
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool MIL_Effect_IndirectFire::IsTargetValid() const
{
    return vTargetPosition_.rX_ != -1. && vTargetPosition_.rY_ != -1.;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::IsInterventionTypeFired
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool MIL_Effect_IndirectFire::IsInterventionTypeFired() const
{
    return bFired_ || GetNbrAmmoToCompleteInterventionType() == 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::IncRef
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void MIL_Effect_IndirectFire::IncRef()
{
    ++nNbrRefs_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::DecRef
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void MIL_Effect_IndirectFire::DecRef()
{
    assert( nNbrRefs_ > 0 );
    if( --nNbrRefs_ == 0 )
        delete this;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::ForceFlying
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void MIL_Effect_IndirectFire::ForceFlying()
{
    StartFlying();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::GetNbrAmmoFired
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
uint MIL_Effect_IndirectFire::GetNbrAmmoFired() const
{
    return nNbrAmmoFired_;
}

