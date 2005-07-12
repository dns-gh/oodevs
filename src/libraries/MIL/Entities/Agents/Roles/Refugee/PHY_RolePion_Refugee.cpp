// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Refugee/PHY_RolePion_Refugee.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 2 $
// $Workfile: PHY_RolePion_Refugee.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_RolePion_Refugee.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Refugee, "PHY_RolePion_Refugee" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Refugee::PHY_RolePion_Refugee( MT_RoleContainer& role, MIL_AgentPion& pion )
    : PHY_RoleInterface_Refugee( role )
    , pPion_                   ( &pion )
    , bManaged_                ( false )
    , bHasChanged_             ( true )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RolePion_Refugee::PHY_RolePion_Refugee()
    : PHY_RoleInterface_Refugee()
    , pPion_                   ( 0 )
    , bManaged_                ( false )
    , bHasChanged_             ( true )    
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Refugee::~PHY_RolePion_Refugee()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Refugee::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Refugee >( *this )
         & pPion_;
}

// =============================================================================
// OPERATIONS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::OrientateRefugee
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
bool PHY_RolePion_Refugee::OrientateRefugee( const MIL_CampRefugies& camp )
{
    assert( pPion_ );
    if ( !pPion_->GetType().IsRefugee() )
        return false;
    return pPion_->GetAutomate().OrientateRefugee( camp );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::NotifyManaged
// Created: NLD 2005-02-28
// -----------------------------------------------------------------------------
bool PHY_RolePion_Refugee::NotifyManaged()
{
    assert( pPion_ );
    if( bManaged_ || !pPion_->GetType().IsRefugee() )
        return false;

    bManaged_    = true;
    bHasChanged_ = true;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::NotifyUnmanaged
// Created: NLD 2005-02-28
// -----------------------------------------------------------------------------
bool PHY_RolePion_Refugee::NotifyUnmanaged()
{
    assert( pPion_ );
    
    if ( !bManaged_ || !pPion_->GetType().IsRefugee() )
        return false;

    bManaged_    = false;
    bHasChanged_ = true;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::NotifyInsideRefugeeCamp
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::NotifyInsideRefugeeCamp( const MIL_CampRefugies& camp )
{
    assert( pPion_ );
    if ( pPion_->GetType().IsRefugee() )
        pPion_->GetAutomate().NotifyInsideRefugeeCamp( camp );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::NotifyOutsideRefugeeCamp
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::NotifyOutsideRefugeeCamp( const MIL_CampRefugies& camp )
{
    assert( pPion_ );
    
    if ( pPion_->GetType().IsRefugee() )
        pPion_->GetAutomate().NotifyOutsideRefugeeCamp( camp );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg.GetAsnMsg().m.refugie_pris_en_comptePresent = 1;
    msg.GetAsnMsg().refugie_pris_en_compte          = bManaged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}
