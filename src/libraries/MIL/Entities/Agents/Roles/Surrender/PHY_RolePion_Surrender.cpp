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

#include "MIL_Pch.h"

#include "PHY_RolePion_Surrender.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Automates/MIL_Automate.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Surrender, "PHY_RolePion_Surrender" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Surrender::PHY_RolePion_Surrender( MT_RoleContainer& role, MIL_AgentPion& pion )
    : PHY_RoleInterface_Surrender( role )
    , pPion_                     ( &pion )
    , bSurrendered_              ( false )
    , bPrisoner_                 ( false )
    , bHasChanged_               ( true )
{
}
  
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RolePion_Surrender::PHY_RolePion_Surrender()
    : PHY_RoleInterface_Surrender()
    , pPion_                     ()
    , bSurrendered_              ( false )
    , bPrisoner_                 ( false )
    , bHasChanged_               ( true )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Surrender::~PHY_RolePion_Surrender()
{
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
    file & boost::serialization::base_object< PHY_RoleInterface_Surrender >( *this )
         & pPion_
         & bSurrendered_
         & bPrisoner_;
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::Surrender
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::Surrender()
{
    if( bSurrendered_ )
        return false;
    bSurrendered_ = true;
    bHasChanged_  = true;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::TakePrisoner
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::TakePrisoner( const MIL_AgentPion& pionTakingPrisoner, const MIL_CampPrisonniers& camp )
{
    assert( pPion_ );
    return pPion_->GetAutomate().TakePrisoner( pionTakingPrisoner, camp );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::NotifyTakenPrisoner
// Created: NLD 2005-02-28
// -----------------------------------------------------------------------------
bool PHY_RolePion_Surrender::NotifyTakenPrisoner()
{
    if( !bSurrendered_ || bPrisoner_ )
        return false;

    bPrisoner_   = true;
    bHasChanged_ = true;
    GetRole< PHY_RolePion_Dotations   >().NotifyTakenPrisoner();
    GetRole< PHY_RolePion_Composantes >().NotifyTakenPrisoner();
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::NotifyInsidePrisonerCamp
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::NotifyInsidePrisonerCamp( const MIL_CampPrisonniers& camp )
{
    if ( bPrisoner_ )
    {
        assert( pPion_ );
        pPion_->GetAutomate().NotifyInsidePrisonerCamp( camp );
    }
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::NotifyOutsidePrisonerCamp
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::NotifyOutsidePrisonerCamp( const MIL_CampPrisonniers& camp )
{
    if ( bPrisoner_ )
    {
        assert( pPion_ );
        pPion_->GetAutomate().NotifyOutsidePrisonerCamp( camp );
    }
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Surrender::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg.GetAsnMsg().m.prisonnierPresent = 1;
    msg.GetAsnMsg().prisonnier          = bPrisoner_;

    msg.GetAsnMsg().m.renduPresent  = 1;
    msg.GetAsnMsg().rendu           = bSurrendered_;
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
