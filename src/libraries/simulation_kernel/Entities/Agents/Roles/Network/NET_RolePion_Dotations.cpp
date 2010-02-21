// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Network/NET_RolePion_Dotations.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 17 $
// $Workfile: NET_RolePion_Dotations.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "NET_RolePion_Dotations.h"
#include "NetworkUnitMessageNotificationHandler_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( network::NET_RolePion_Dotations )

namespace network
{

template< typename Archive >
void save_construct_data( Archive& archive, const NET_RolePion_Dotations* role, const unsigned int /*version*/ )
{
    const MIL_AgentPion* pPion = &role->pion_;
    archive << pPion;
}

template< typename Archive >
void load_construct_data( Archive& archive, NET_RolePion_Dotations* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )NET_RolePion_Dotations( *pion );
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations constructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
NET_RolePion_Dotations::NET_RolePion_Dotations( MIL_AgentPion& pion )
    : pion_                     ( pion )
    , bLastStateDead_            ( false )
    , bLastStateNeutralized_     ( false )
    , bExternalMustUpdateData_    ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
NET_RolePion_Dotations::~NET_RolePion_Dotations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void NET_RolePion_Dotations::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< NET_RoleInterface_Dotations >( *this )
         & bLastStateDead_
         & bLastStateNeutralized_;
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::DataUpdated
// Created: NLD 2006-10-04
// -----------------------------------------------------------------------------
bool NET_RolePion_Dotations::DataUpdated() const
{
//@TODO Remove HasChanged in interface and hla
    if( bExternalMustUpdateData_
        || pion_.IsDead()        != bLastStateDead_
        || pion_.IsNeutralized() != bLastStateNeutralized_ )
        return true;

    return false;
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::SendMsg
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::SendMsg( client::UnitAttributes& asnMsg ) const
{
    asnMsg.Send( NET_Publisher_ABC::Publisher() );

    if( asnMsg().has_pions_renforcant()  && asnMsg().pions_renforcant().elem_size() > 0 )
        asnMsg().mutable_pions_renforcant()->Clear();

    if( asnMsg().has_contamine_par_agents_nbc()  && asnMsg().contamine_par_agents_nbc().elem_size() > 0 )
        asnMsg().mutable_contamine_par_agents_nbc()->Clear();

    if( asnMsg().has_pions_transportes()  && asnMsg().pions_transportes().elem_size() > 0 )
        asnMsg().mutable_pions_transportes()->Clear();

    if( asnMsg().has_dotation_eff_ressource()  && asnMsg().dotation_eff_ressource().elem_size() > 0 )
        asnMsg().mutable_dotation_eff_ressource()->Clear();

    if( asnMsg().has_dotation_eff_materiel()  && asnMsg().dotation_eff_materiel().elem_size() > 0 )
        asnMsg().mutable_dotation_eff_materiel()->Clear();

    if( asnMsg().has_dotation_eff_personnel()  && asnMsg().dotation_eff_personnel().elem_size() > 0 )
        asnMsg().mutable_dotation_eff_personnel()->Clear();

    if( asnMsg().has_equipements_pretes()  && asnMsg().equipements_pretes().elem_size() > 0 )
        asnMsg().mutable_equipements_pretes()->Clear();

    if( asnMsg().has_equipements_empruntes()  && asnMsg().equipements_empruntes().elem_size() > 0 )
        asnMsg().mutable_equipements_empruntes()->Clear();
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::SendChangedState() const
{
    if( !DataUpdated() )
        return;

    client::UnitAttributes msg;
    msg().set_oid( pion_.GetID() );
    pion_.Apply( &network::NetworkUnitMessageNotificationHandler_ABC::SendChangedState, msg );
 
    bool bIsDead = pion_.IsDead();
    if( bLastStateDead_ != bIsDead )
    {
        msg().set_mort( bIsDead );
        bLastStateDead_ = bIsDead;        
    }

    bool bIsNeutralized = pion_.IsNeutralized();
    if( bLastStateNeutralized_ != bIsNeutralized )
    {
        msg().set_neutralise( bIsNeutralized );
        bLastStateNeutralized_              = bIsNeutralized;        
    }       

    SendMsg( msg );
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::SendFullState() const
{
    // MsgUnitAttributes

    client::UnitAttributes msg;
    msg().set_oid       ( pion_.GetID() );
    msg().set_mort      ( bLastStateDead_ = pion_.IsDead() );
    msg().set_neutralise( bLastStateNeutralized_ = pion_.IsNeutralized() );

//    pPion_->GetRole< PHY_RoleInterface_Humans           >().SendFullState( msg );
//    pPion_->GetRole< PHY_RoleInterface_Dotations        >().SendFullState( msg );
//    pPion_->GetRole< PHY_RoleInterface_Posture          >().SendFullState( msg ); // Current, old, pourcentage
//    pPion_->GetRole< PHY_RoleInterface_Location         >().SendFullState( msg ); // Direction, speed, altitude, position
//    pPion_->GetRole< PHY_RoleInterface_Composantes      >().SendFullState( msg ); // Etat ops
//    pPion_->GetRole< PHY_RoleInterface_Reinforcement    >().SendFullState( msg ); // Reinforcement
//    pPion_->GetRole< PHY_RoleAction_Loading             >().SendFullState( msg ); // Loading state
//    pPion_->GetRole< DEC_RolePion_Decision              >().SendFullState( msg ); // Dec states
//    pPion_->GetRole< PHY_RoleInterface_NBC              >().SendFullState( msg ); // NBC state
//    pPion_->GetRole< PHY_RoleInterface_Communications   >().SendFullState( msg ); // Brouillage
//    pPion_->GetRole< PHY_RoleInterface_HumanFactors     >().SendFullState( msg );
//    pPion_->GetRole< PHY_RoleInterface_Transported      >().SendFullState( msg );
//    pPion_->GetRole< PHY_RoleAction_Transport           >().SendFullState( msg );
//    pPion_->GetRole< PHY_RoleInterface_Perceiver        >().SendFullState( msg );
//    pPion_->GetRole< PHY_RoleInterface_Surrender        >().SendFullState( msg );
//    pPion_->GetRole< PHY_RoleInterface_Refugee          >().SendFullState( msg );

    pion_.Apply( &network::NetworkUnitMessageNotificationHandler_ABC::SendFullState, msg );

    SendMsg( msg );
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::Clean
// Created: MGD 2009-09-29
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::Clean()
{
    bExternalMustUpdateData_ = false;// Reinitialize for next step
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::NotifyComponentHasChanged
// Created: MGD 2009-09-29
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::NotifyDataHasChanged()
{
    bExternalMustUpdateData_ = true;
}

} // namespace network