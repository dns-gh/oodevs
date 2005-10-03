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

#include "MIL_pch.h"

#include "MIL_Effect_IndirectFire.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponDataType_IndirectFire.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_IndirectFireResults.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KS_AgentQuerier.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire constructor
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
MIL_Effect_IndirectFire::MIL_Effect_IndirectFire( const MIL_AgentPion& firer, uint nTargetKnowledgeID, const PHY_IndirectFireDotationClass& indirectWeaponClass, MT_Float rInterventionTypeToFire )
    : nNbrRefs_               ( 0 )
    , firer_                  ( firer )
    , indirectWeaponClass_    ( indirectWeaponClass )
    , rInterventionTypeToFire_( rInterventionTypeToFire )
    , pWeaponDotationCategory_( )
    , vSourcePosition_        ( firer.GetRole< PHY_RolePion_Location >().GetPosition() )
    , vTargetPosition_        ( -1., -1. )
    , nTargetKnowledgeID_     ( nTargetKnowledgeID )
    , nNbrAmmoFired_          ( 0 )
    , bIsFlying_              ( false )
    , rImpactTimeStep_        ( 0. )
    , pFireResult_            ( 0 )    
{
    UpdateTargetPositionFromKnowledge(); /// Update vTargetPosition_
    IncRef();
    MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().RegisterFlyingShell( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire constructor
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
MIL_Effect_IndirectFire::MIL_Effect_IndirectFire( const MIL_AgentPion& firer, const MT_Vector2D& vTargetPosition, const PHY_IndirectFireDotationClass& indirectWeaponClass, MT_Float rInterventionTypeToFire )
    : nNbrRefs_               ( 0 )
    , firer_                  ( firer )
    , indirectWeaponClass_ ( indirectWeaponClass )
    , rInterventionTypeToFire_( rInterventionTypeToFire )
    , pWeaponDotationCategory_( )
    , vSourcePosition_        ( firer.GetRole< PHY_RolePion_Location >().GetPosition() )
    , vTargetPosition_        ( vTargetPosition )
    , nTargetKnowledgeID_     ( 0 )
    , nNbrAmmoFired_          ( 0 )
    , bIsFlying_              ( false )
    , rImpactTimeStep_        ( 0. )
    , pFireResult_            ( 0 )
{
    IncRef();
    MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().RegisterFlyingShell( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire destructor
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
MIL_Effect_IndirectFire::~MIL_Effect_IndirectFire()
{
    delete pFireResult_;
    MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().UnregisterFlyingShell( *this );
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::UpdateTargetPositionFromKnowledge
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void MIL_Effect_IndirectFire::UpdateTargetPositionFromKnowledge()
{
    if( nTargetKnowledgeID_ == 0 )
        return;
    const DEC_Knowledge_Agent* pTargetKnowledge = firer_.GetKSQuerier().GetKnowledgeAgentFromDiaID( nTargetKnowledgeID_ );
    if( !pTargetKnowledge )
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
    if( pWeaponDotationCategory_ )
    {
        return weapon.GetDotationCategory() == *pWeaponDotationCategory_;
    }
    else
    {
        const PHY_DotationCategory_IndirectFire_ABC* pIndirectFireData = weapon.GetDotationCategory().GetIndirectFireData();
        return pIndirectFireData && pIndirectFireData->GetIndirectFireDotationCategory() == indirectWeaponClass_;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::NotifyAmmoFired
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void MIL_Effect_IndirectFire::NotifyAmmoFired( const PHY_WeaponDataType_IndirectFire& weaponType, uint nNbrAmmoReserved )
{
    assert( pWeaponDotationCategory_ && pWeaponDotationCategory_->GetIndirectFireData() );

    if( !pFireResult_ )
        pFireResult_ = new PHY_IndirectFireResults( firer_, vTargetPosition_, *pWeaponDotationCategory_ );

    assert( !bIsFlying_ );

    UpdateTargetPositionFromKnowledge();

    const MT_Float rNewTimeBeforeImpact = vSourcePosition_.Distance( vTargetPosition_ ) / weaponType.GetAverageSpeed();
    rImpactTimeStep_ = std::max( rImpactTimeStep_, rNewTimeBeforeImpact + MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() );
    
    nNbrAmmoFired_ += nNbrAmmoReserved;
    if( pWeaponDotationCategory_->GetIndirectFireData()->ConvertToInterventionType( nNbrAmmoFired_ ) >= rInterventionTypeToFire_ )
        bIsFlying_ = true;        
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::Execute
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool MIL_Effect_IndirectFire::Execute()
{
    if( !IsTargetValid() || rInterventionTypeToFire_ == 0. )
    {
        if( pFireResult_ )
        {
            delete pFireResult_;
            pFireResult_ = 0;
        }
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
        assert( pWeaponDotationCategory_ );
        assert( pFireResult_ );
        pWeaponDotationCategory_->IndirectFire( firer_, vSourcePosition_, vTargetPosition_, nNbrAmmoFired_, *pFireResult_ );
    }
    delete pFireResult_;
    pFireResult_ = 0;
    DecRef();
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::GetNbrAmmoToCompleteInterventionType
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
uint MIL_Effect_IndirectFire::GetNbrAmmoToCompleteInterventionType() const
{
    return (uint)( pWeaponDotationCategory_->GetIndirectFireData()->ConvertToNbrAmmo( rInterventionTypeToFire_ ) - nNbrAmmoFired_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_IndirectFire::FlyThroughLocalisation
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
bool MIL_Effect_IndirectFire::FlyThroughLocalisation( const TER_Localisation& localisation ) const
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
