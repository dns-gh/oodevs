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
#include "simulation_kernel/Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"

#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/TransportNotificationHandler_ABC.h"

BOOST_CLASS_EXPORT_GUID( refugee::PHY_RolePion_Refugee, "PHY_RolePion_Refugee" )

namespace refugee
{

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_Refugee* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_Refugee* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RolePion_Refugee( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Refugee::PHY_RolePion_Refugee( MIL_AgentPion& pion )
    : pion_                    ( pion )
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Refugee::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Refugee >( *this )
         & bManaged_
         & const_cast< MIL_Object_ABC*& >( pCamp_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::Update( bool /*bIsDead*/ )
{
    if( pCamp_ && pCamp_->IsMarkedForDestruction() )
        pCamp_ = 0;

    if( HasChanged() )
    {
        pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Orientate
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::Orientate( MIL_AgentPion& pionManaging )
{
    if( !pion_.GetType().IsRefugee() )
        return;

    pCamp_       = 0;
    bManaged_    = true;
    bHasChanged_ = true;
    pion_.GetAutomate().NotifyRefugeeOriented( pionManaging );

    bool bTransportOnlyLoadable = false;
    pionManaging.Apply( &transport::TransportNotificationHandler_ABC::MagicLoadPion, pion_, bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Release
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::Release( MIL_AgentPion& callerAgent )
{

    if( !pion_.GetType().IsRefugee() || !bManaged_ )
        return;

    pCamp_       = 0;
    bManaged_    = false;
    bHasChanged_ = true;
    pion_.GetAutomate().NotifyRefugeeReleased();

    callerAgent.Apply( &transport::TransportNotificationHandler_ABC::MagicUnloadPion, pion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Release
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::ReleaseCamp( MIL_AgentPion& callerAgent, const MIL_Object_ABC& camp )
{

    if( !pion_.GetType().IsRefugee() || !bManaged_ )
        return;
    
    pCamp_       = &camp;
    bManaged_    = true;
    bHasChanged_ = true;
    pion_.GetAutomate().NotifyRefugeeReleased( camp );

    callerAgent.Apply( &transport::TransportNotificationHandler_ABC::MagicUnloadPion, pion_ );
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Refugee::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::IsManaged
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool PHY_RolePion_Refugee::IsManaged() const
{
    return bManaged_;
}

} // namespace refugee
