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
#include "MIL_Singletons.h"
#include "MIL_Time_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponDataType_IndirectFire.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Illumination/PHY_RoleInterface_Illumination.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_terrain/TER_Localisation.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire constructor
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
MIL_Effect_IndirectFire::MIL_Effect_IndirectFire( const MIL_Agent_ABC& firer, unsigned int nTargetKnowledgeID, const PHY_DotationCategory_IndirectFire_ABC& indirectDotationCategory, double rInterventionTypeToFire )
    : nNbrRefs_( 0 )
    , firer_( firer )
    , rInterventionTypeToFire_( rInterventionTypeToFire )
    , indirectDotationCategory_( indirectDotationCategory )
    , vSourcePosition_( firer.GetRole< PHY_RoleInterface_Location >().GetPosition() )
    , vTargetPosition_( -1., -1. )
    , nTargetKnowledgeID_( nTargetKnowledgeID )
    , nNbrAmmoFired_( 0 )
    , fireResultId_( 0 )
    , bIsFlying_( false )
    , bFired_( false )
    , bArrived_( false )
    , rImpactTimeStep_( 0. )
    , pFireResult_( 0 )
{
    UpdateTargetPositionFromKnowledge(); /// Update vTargetPosition_
    IncRef();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire constructor
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
MIL_Effect_IndirectFire::MIL_Effect_IndirectFire( const MIL_Agent_ABC& firer, const MT_Vector2D& vTargetPosition, const PHY_DotationCategory_IndirectFire_ABC& indirectDotationCategory, double rInterventionTypeToFire )
    : nNbrRefs_( 0 )
    , firer_( firer )
    , rInterventionTypeToFire_( rInterventionTypeToFire )
    , indirectDotationCategory_( indirectDotationCategory )
    , vSourcePosition_( firer.GetRole< PHY_RoleInterface_Location >().GetPosition() )
    , vTargetPosition_( vTargetPosition )
    , nTargetKnowledgeID_( 0 )
    , nNbrAmmoFired_( 0 )
    , bIsFlying_( false )
    , bFired_( false )
    , bArrived_( false )
    , rImpactTimeStep_( 0. )
    , pFireResult_( 0 )
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
    boost::shared_ptr< DEC_Knowledge_Agent > pTargetKnowledge = firer_.GetKnowledgeGroup()->GetKnowledge().GetKnowledgeAgentFromID( nTargetKnowledgeID_ );
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

    const double rFlyingDist = vSourcePosition_.Distance( vTargetPosition_ );
    if( rFlyingDist < weapon.GetMinRangeToIndirectFire() || rFlyingDist > weapon.GetMaxRangeToIndirectFire() )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::NotifyAmmoFired
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void MIL_Effect_IndirectFire::NotifyAmmoFired( const PHY_WeaponDataType_IndirectFire& weaponType, unsigned int nNbrAmmoReserved )
{
    assert( !bIsFlying_ );

    UpdateTargetPositionFromKnowledge();

    const double rNewTimeBeforeImpact = vSourcePosition_.Distance( vTargetPosition_ ) / weaponType.GetAverageSpeed();
    rImpactTimeStep_ = std::max( rImpactTimeStep_, rNewTimeBeforeImpact + MIL_Singletons::GetTime().GetCurrentTick() );

    nNbrAmmoFired_ += nNbrAmmoReserved;
    if( indirectDotationCategory_.GetDotationCategory().ConvertToInterventionType( nNbrAmmoFired_ ) >= rInterventionTypeToFire_ )
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

    if( MIL_Singletons::GetTime().GetCurrentTick() < rImpactTimeStep_ )
        return true;

    if( nNbrAmmoFired_ > 0 )
    {
        UpdateTargetPositionFromKnowledge();
        assert( pFireResult_ );
        //LTO begin
        if( indirectDotationCategory_.GetDotationCategory().IsGuided() )
        {
            boost::shared_ptr< DEC_Knowledge_Agent > pTargetKnowledge = firer_.GetKnowledgeGroup()->GetKnowledge().GetKnowledgeAgentFromID( nTargetKnowledgeID_ );
            if( pTargetKnowledge && pTargetKnowledge->IsValid() )
            {
                indirectDotationCategory_.GetDotationCategory().ApplyIndirectFireEffect( firer_, pTargetKnowledge->GetAgentKnown(), nNbrAmmoFired_, *pFireResult_ );
            }
        }
        //LTO end
        else
        {
            indirectDotationCategory_.GetDotationCategory().ApplyIndirectFireEffect( firer_, vSourcePosition_, vTargetPosition_, nNbrAmmoFired_, *pFireResult_ );
        }
    }

    bArrived_ = true;
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
    fireResultId_ = pFireResult_->GetID();

    if( !bIsFlying_ )
    {
        bIsFlying_ = true;
        MIL_EffectManager::GetEffectManager().RegisterFlyingShell( *this );
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
        MIL_EffectManager::GetEffectManager().UnregisterFlyingShell( *this );
    }

    delete pFireResult_;
    pFireResult_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::GetNbrAmmoToCompleteInterventionType
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
unsigned int MIL_Effect_IndirectFire::GetNbrAmmoToCompleteInterventionType() const
{
    if( bFired_ )
        return 0;

    double rTmp = indirectDotationCategory_.GetDotationCategory().ConvertToNbrAmmo( rInterventionTypeToFire_ ) - nNbrAmmoFired_;
    if( rTmp <= 0. )
        return 0;
    return std::max( (unsigned int)1, (unsigned int)rTmp );
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::IsFlyingThroughLocalisation
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
bool MIL_Effect_IndirectFire::IsFlyingThroughLocalisation( const TER_Localisation& localisation ) const
{
    if( !bIsFlying_ )
        return false;
    MT_Line lineTmp( vSourcePosition_, vTargetPosition_ );
    return localisation.Intersect2D( lineTmp );
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::GetFireID
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
unsigned int MIL_Effect_IndirectFire::GetFireID() const
{
    return fireResultId_;
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
    if( indirectDotationCategory_.GetDotationCategory().IsGuided() && nTargetKnowledgeID_ != 0 )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > pTargetKnowledge = firer_.GetKnowledgeGroup()->GetKnowledge().GetKnowledgeAgentFromID( nTargetKnowledgeID_ );
        if( pTargetKnowledge && pTargetKnowledge->IsValid() )
        {
            if( pTargetKnowledge->GetPosition().Distance( firer_.GetRole< PHY_RoleInterface_Location >().GetPosition() ) > indirectDotationCategory_.GetDotationCategory().GetGuidanceRange()
            || !pTargetKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Illumination >().IsIlluminated() )
            {
                return false;
            }
        }
    }
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
// Name: MIL_Effect_IndirectFire::MustWaitImpact
// Created: MGD 2010-02-17
// -----------------------------------------------------------------------------
bool MIL_Effect_IndirectFire::MustWaitImpact() const
{
    if( indirectDotationCategory_.GetDotationCategory().IsGuided() )
        return bFired_ && !bArrived_;
    return false;
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
// Name: MIL_Effect_IndirectFire::GetNbrAmmoFired
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
unsigned int MIL_Effect_IndirectFire::GetNbrAmmoFired() const
{
    return nNbrAmmoFired_;
}
