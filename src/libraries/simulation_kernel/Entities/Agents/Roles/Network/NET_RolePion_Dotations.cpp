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

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/DEC_AutomateDecision.h"


#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( network::NET_RolePion_Dotations, "NET_RolePion_Dotations" )

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
void NET_RolePion_Dotations::serialize( Archive& file, const uint )
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
void NET_RolePion_Dotations::SendMsg( NET_ASN_MsgUnitAttributes& asnMsg ) const
{
    asnMsg.Send();

    if( asnMsg().m.pions_renforcantPresent && asnMsg().pions_renforcant.n > 0 )
        delete [] asnMsg().pions_renforcant.elem;

    if( asnMsg().m.contamine_par_agents_nbcPresent && asnMsg().contamine_par_agents_nbc.n > 0 )
        delete [] asnMsg().contamine_par_agents_nbc.elem;

    if( asnMsg().m.pions_transportesPresent && asnMsg().pions_transportes.n > 0 )
        delete [] asnMsg().pions_transportes.elem;

    if( asnMsg().m.dotation_eff_ressourcePresent && asnMsg().dotation_eff_ressource.n > 0 )
        delete [] asnMsg().dotation_eff_ressource.elem;

    if( asnMsg().m.dotation_eff_materielPresent && asnMsg().dotation_eff_materiel.n > 0 )
        delete [] asnMsg().dotation_eff_materiel.elem;

    if( asnMsg().m.dotation_eff_personnelPresent && asnMsg().dotation_eff_personnel.n > 0 )
        delete [] asnMsg().dotation_eff_personnel.elem;

    if( asnMsg().m.equipements_pretesPresent && asnMsg().equipements_pretes.n > 0 )
        delete [] asnMsg().equipements_pretes.elem;

    if( asnMsg().m.equipements_empruntesPresent && asnMsg().equipements_empruntes.n > 0 )
        delete [] asnMsg().equipements_empruntes.elem;
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::SendChangedState() const
{
    if( !DataUpdated() )
        return;

    NET_ASN_MsgUnitAttributes msg;

    msg().oid = pion_.GetID();

    pion_.Apply( &network::NetworkUnitMessageNotificationHandler_ABC::SendChangedState, msg );
 
    bool bIsDead = pion_.IsDead();
    if( bLastStateDead_ != bIsDead )
    {
        msg().m.mortPresent = 1;
        msg().mort          = bIsDead;
        bLastStateDead_               = bIsDead;        
    }

    bool bIsNeutralized = pion_.IsNeutralized();
    if( bLastStateNeutralized_ != bIsNeutralized )
    {
        msg().m.neutralisePresent = 1;
        msg().neutralise          = bIsNeutralized;
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
    NET_ASN_MsgUnitAttributes msg;
    msg().oid                 = pion_.GetID();
    msg().m.mortPresent       = 1;
    msg().mort                = bLastStateDead_ = pion_.IsDead();
    msg().m.neutralisePresent = 1;
    msg().neutralise          = bLastStateNeutralized_ = pion_.IsNeutralized();

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