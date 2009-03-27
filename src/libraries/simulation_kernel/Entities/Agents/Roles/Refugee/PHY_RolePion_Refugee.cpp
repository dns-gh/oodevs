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

#include "simulation_kernel_pch.h"

#include "PHY_RolePion_Refugee.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
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
    , pCamp_                   ( 0 )
    , bHasChanged_             ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RolePion_Refugee::PHY_RolePion_Refugee()
    : PHY_RoleInterface_Refugee()
    , pPion_                   ( 0 )
    , bManaged_                ( false )
    , pCamp_                   ( 0 )
    , bHasChanged_             ( true )    
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Refugee::~PHY_RolePion_Refugee()
{
    // NOTHING
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
         & pPion_
         & bManaged_
         & const_cast< MIL_Object_ABC*& >( pCamp_ );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::Update( bool /*bIsDead*/ )
{
    if( pCamp_ && pCamp_->IsMarkedForDestruction() )
        pCamp_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Orientate
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
bool PHY_RolePion_Refugee::Orientate( const MIL_AgentPion& pionManaging )
{
    assert( pPion_ );
    if( !pPion_->GetType().IsRefugee() )
        return false;

    pCamp_       = 0;
    bManaged_    = true;
    bHasChanged_ = true;
    pPion_->GetAutomate().NotifyRefugeeOriented( pionManaging );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Release
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
bool PHY_RolePion_Refugee::Release()
{
    assert( pPion_ );
    if( !pPion_->GetType().IsRefugee() || !bManaged_ )
        return false;

    pCamp_       = 0;
    bManaged_    = false;
    bHasChanged_ = true;
    pPion_->GetAutomate().NotifyRefugeeReleased();
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Release
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
bool PHY_RolePion_Refugee::Release( const MIL_Object_ABC& camp )
{
    assert( pPion_ );
    if( !pPion_->GetType().IsRefugee() || !bManaged_ )
        return false;
    
    pCamp_       = &camp;
    bManaged_    = true;
    bHasChanged_ = true;
    pPion_->GetAutomate().NotifyRefugeeReleased( camp );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::IsManaged
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool PHY_RolePion_Refugee::IsManaged( const MIL_Object_ABC& camp ) const
{
    if( !pCamp_ )
        return false;
    return pCamp_ == &camp; //$$$$
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
    msg().m.refugie_pris_en_comptePresent = 1;
    msg().refugie_pris_en_compte          = bManaged_;
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
