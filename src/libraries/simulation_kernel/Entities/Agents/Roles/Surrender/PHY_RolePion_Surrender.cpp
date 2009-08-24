// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 17:40 $
// $Revision: 6 $
// $Workfile: PHY_RolePion_Surrender.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Surrender.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/MIL_Army.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Surrender, "PHY_RolePion_Surrender" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_Surrender* role, const unsigned int /*version*/ )
{
    archive << role->pPion_;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_Surrender* role, const unsigned int /*version*/ )
{
	MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RolePion_Surrender( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Surrender::PHY_RolePion_Surrender( MIL_AgentPion& pion )
    : pPion_                     ( &pion )
    , bPrisoner_                 ( false )
    , pPrison_                   ( 0 )
    , bHasChanged_               ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Surrender::~PHY_RolePion_Surrender()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Surrender::serialize( Archive& file, const uint )
{
    file & bPrisoner_
         & const_cast< MIL_Object_ABC*& >( pPrison_ );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::Update( bool /*bIsDead*/ )
{
    if( pPrison_ && pPrison_->IsMarkedForDestruction() )
        pPrison_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::NotifySurrendered
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::NotifySurrendered()
{
    assert( pPion_ );
    MIL_Report::PostEvent( *pPion_, MIL_Report::eReport_Surrendered );
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::NotifySurrenderCanceled
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::NotifySurrenderCanceled()
{
    assert( pPion_ );
    Release();
    MIL_Report::PostEvent( *pPion_, MIL_Report::eReport_CancelSurrender );
    bHasChanged_ = true;
}
            
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::Capture
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::Capture( const MIL_AgentPion& pionTakingPrisoner )
{
    assert( pPion_ );
    if( !IsSurrendered() )
        return false;

    pPrison_     = 0;
    bPrisoner_   = true;
    bHasChanged_ = true;
    pPion_->GetRole< PHY_RoleInterface_Dotations   >().NotifyCaptured();
    pPion_->GetRole< PHY_RoleInterface_Composantes >().NotifyCaptured();
    return pPion_->GetAutomate().NotifyCaptured( pionTakingPrisoner );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::Release
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::Release()
{
    if( !IsSurrendered() || !bPrisoner_ )
        return false;

    pPrison_     = 0;
    bPrisoner_   = false;
    bHasChanged_ = true;
    pPion_->GetRole< PHY_RoleInterface_Dotations   >().NotifyReleased();
    pPion_->GetRole< PHY_RoleInterface_Composantes >().NotifyReleased();
    return pPion_->GetAutomate().NotifyReleased();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::Imprison
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::Imprison( const MIL_Object_ABC& camp )
{
    if( !IsSurrendered() || !bPrisoner_ )
        return false;
    pPrison_ = &camp;
    pPion_->GetAutomate().NotifyImprisoned( camp );    
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::IsImprisoned
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::IsImprisoned( const MIL_Object_ABC& camp )
{
    if( !pPrison_ )
        return false;
    return pPrison_ == &camp; //$$$$
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::IsSurrendered
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::IsSurrendered() const
{
    assert( pPion_ );
    return pPion_->GetAutomate().IsSurrendered();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::GetArmySurrenderedTo
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
const MIL_Army* PHY_RolePion_Surrender::GetArmySurrenderedTo() const
{
    assert( pPion_ );
    return pPion_->GetAutomate().GetArmySurrenderedTo();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    assert( pPion_ );
    msg().m.prisonnierPresent = 1;
    msg().prisonnier          = IsPrisoner();

    const MIL_Army* pArmySurrenderedTo = GetArmySurrenderedTo();
    msg().m.renduPresent  = 1;
    msg().rendu           = pArmySurrenderedTo ? pArmySurrenderedTo->GetID() : 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::IsPrisoner
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::IsPrisoner() const
{
    return bPrisoner_;
}
