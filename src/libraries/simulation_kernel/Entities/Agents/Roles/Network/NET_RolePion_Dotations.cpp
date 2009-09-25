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
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Humans/PHY_RoleInterface_Humans.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/DEC_AutomateDecision.h"

#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( NET_RolePion_Dotations, "NET_RolePion_Dotations" )

template< typename Archive >
void save_construct_data( Archive& archive, const NET_RolePion_Dotations* role, const unsigned int /*version*/ )
{
    archive << role->pPion_;
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
    : pPion_                     ( &pion )
    , bLastStateDead_            ( false )
    , bLastStateNeutralized_     ( false )
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
    assert( pPion_ );

    if(    pPion_->GetRole< PHY_RoleInterface_Posture           >().HasChanged()
        || pPion_->GetRole< PHY_RoleInterface_Composantes       >().HasChanged()
        || pPion_->GetRole< PHY_RoleInterface_Location          >().HasLocationChanged()
        || pPion_->GetRole< PHY_RoleInterface_Location          >().HasSpeedChanged()
        || pPion_->GetRole< PHY_RoleInterface_Reinforcement     >().HasChanged()
        || pPion_->GetRole< nbc::PHY_RoleInterface_NBC          >().HasChanged()
        || pPion_->GetRole< PHY_RoleInterface_Communications    >().HasChanged()
        || pPion_->GetRole< transport::PHY_RoleInterface_Transported >().HasChanged()
        || pPion_->GetRole< transport::PHY_RoleAction_Transport  >().HasChanged()
        || pPion_->GetRole< PHY_RoleInterface_HumanFactors      >().HasChanged()
        || pPion_->GetRole< transport::PHY_RoleAction_Loading              >().HasChanged()
        || pPion_->GetRole< PHY_RoleInterface_Surrender         >().HasChanged()
        || pPion_->GetRole< PHY_RoleInterface_Refugee           >().HasChanged()
        || pPion_->GetRole< PHY_RoleInterface_Perceiver         >().HasRadarStateChanged() 
        || pPion_->GetRole< DEC_RolePion_Decision               >().HasStateChanged()
        || pPion_->GetRole< dotation::PHY_RoleInterface_Dotations         >().HasChanged()
        || pPion_->GetRole< human::PHY_RoleInterface_Humans            >().HasChanged()
        || pPion_->IsDead()        != bLastStateDead_
        || pPion_->IsNeutralized() != bLastStateNeutralized_ )
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
    assert( pPion_ );

    // Debug - Cones de vision
    if( MIL_AgentServer::GetWorkspace().GetAgentServer().MustSendUnitVisionCones() )
    {
        if(    pPion_->GetRole< PHY_RoleInterface_Location      >().HasLocationChanged()
            || pPion_->GetRole< PHY_RoleInterface_Composantes   >().HasChanged()
            || pPion_->GetRole< PHY_RoleInterface_Perceiver     >().HasChanged()
            || pPion_->GetRole< transport::PHY_RoleAction_Loading          >().HasChanged()
            || pPion_->GetRole< transport::PHY_RoleInterface_Transported   >().HasChanged()
            || pPion_->GetRole< PHY_RoleInterface_Surrender     >().HasChanged()
            || MIL_AgentServer::GetWorkspace().GetAgentServer().MustInitUnitVisionCones() )
            pPion_->GetRole< PHY_RoleInterface_Perceiver        >().SendDebugState(); //$$ BOF
    }

    if( !DataUpdated() )
        return;

    NET_ASN_MsgUnitAttributes msg;
    assert( pPion_ );
    msg().oid = pPion_->GetID();

    pPion_->GetRole< human::PHY_RoleInterface_Humans           >().SendChangedState( msg );
    pPion_->GetRole< dotation::PHY_RoleInterface_Dotations        >().SendChangedState( msg );
    pPion_->GetRole< PHY_RoleInterface_Posture          >().SendChangedState( msg ); // Current, old, pourcentage
    pPion_->GetRole< PHY_RoleInterface_Location         >().SendChangedState( msg ); // Direction, speed, altitude, position
    pPion_->GetRole< PHY_RoleInterface_Composantes      >().SendChangedState( msg ); // Etat ops
    pPion_->GetRole< PHY_RoleInterface_Reinforcement    >().SendChangedState( msg ); // Reinforcement
    pPion_->GetRole< transport::PHY_RoleAction_Loading             >().SendChangedState( msg ); // Loading state
    pPion_->GetRole< nbc::PHY_RoleInterface_NBC         >().SendChangedState( msg ); // NBC state
    pPion_->GetRole< PHY_RoleInterface_Communications   >().SendChangedState( msg ); // Brouillage / silence radio
    pPion_->GetRole< PHY_RoleInterface_HumanFactors     >().SendChangedState( msg );
    pPion_->GetRole< transport::PHY_RoleInterface_Transported      >().SendChangedState( msg );
    pPion_->GetRole< transport::PHY_RoleAction_Transport           >().SendChangedState( msg );
    pPion_->GetRole< PHY_RoleInterface_Perceiver        >().SendChangedState( msg );
    pPion_->GetRole< PHY_RoleInterface_Surrender        >().SendChangedState( msg );
    pPion_->GetRole< PHY_RoleInterface_Refugee          >().SendChangedState( msg );
    pPion_->GetRole< DEC_RolePion_Decision              >().SendChangedState( msg ); // Dec states

    bool bIsDead = pPion_->IsDead();
    if( bLastStateDead_ != bIsDead )
    {
        msg().m.mortPresent = 1;
        msg().mort          = bIsDead;
        bLastStateDead_               = bIsDead;        
    }

    bool bIsNeutralized = pPion_->IsNeutralized();
    if( bLastStateNeutralized_ != bIsNeutralized )
    {
        msg().m.neutralisePresent = 1;
        msg().neutralise          = bIsNeutralized;
        bLastStateNeutralized_              = bIsNeutralized;        
    }       

    SendMsg( msg );

    // Other messages
    pPion_->GetRole< PHY_RoleAction_Moving          >().SendChangedState        (); // Itineraire
    pPion_->GetRole< PHY_RoleInterface_Composantes  >().SendLogisticChangedState(); // Consignes logistiques maintenance
    pPion_->GetRole< human::PHY_RoleInterface_Humans       >().SendLogisticChangedState(); // Consignes logistiques santé
    
    PHY_RoleInterface_Maintenance* roleMaintenance = pPion_->Retrieve< PHY_RoleInterface_Maintenance >();
    if( roleMaintenance )
        roleMaintenance->SendChangedState();
    PHY_RoleInterface_Medical* roleMedical = pPion_->Retrieve< PHY_RoleInterface_Medical >();
    if( roleMedical )
        roleMedical->SendChangedState();
    PHY_RoleInterface_Supply* roleSupply = pPion_->Retrieve< PHY_RoleInterface_Supply >();
    if( roleSupply )
        roleSupply->SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::SendFullState() const
{
    // MsgUnitAttributes
    assert( pPion_ );

    NET_ASN_MsgUnitAttributes msg;
    msg().oid                 = pPion_->GetID();
    msg().m.mortPresent       = 1;
    msg().mort                = bLastStateDead_ = pPion_->IsDead();
    msg().m.neutralisePresent = 1;
    msg().neutralise          = bLastStateNeutralized_ = pPion_->IsNeutralized();
    pPion_->GetRole< human::PHY_RoleInterface_Humans       >().SendFullState( msg );
    pPion_->GetRole< dotation::PHY_RoleInterface_Dotations >().SendFullState( msg );
    pPion_->GetRole< PHY_RoleInterface_Posture          >().SendFullState( msg ); // Current, old, pourcentage
    pPion_->GetRole< PHY_RoleInterface_Location         >().SendFullState( msg ); // Direction, speed, altitude, position
    pPion_->GetRole< PHY_RoleInterface_Composantes      >().SendFullState( msg ); // Etat ops
    pPion_->GetRole< PHY_RoleInterface_Reinforcement    >().SendFullState( msg ); // Reinforcement
    pPion_->GetRole< transport::PHY_RoleAction_Loading             >().SendFullState( msg ); // Loading state
    pPion_->GetRole< DEC_RolePion_Decision              >().SendFullState( msg ); // Dec states
    pPion_->GetRole< nbc::PHY_RoleInterface_NBC         >().SendFullState( msg ); // NBC state
    pPion_->GetRole< PHY_RoleInterface_Communications   >().SendFullState( msg ); // Brouillage
    pPion_->GetRole< PHY_RoleInterface_HumanFactors     >().SendFullState( msg );
    pPion_->GetRole< transport::PHY_RoleInterface_Transported      >().SendFullState( msg );
    pPion_->GetRole< transport::PHY_RoleAction_Transport           >().SendFullState( msg );
    pPion_->GetRole< PHY_RoleInterface_Perceiver        >().SendFullState( msg );
    pPion_->GetRole< PHY_RoleInterface_Surrender        >().SendFullState( msg );
    pPion_->GetRole< PHY_RoleInterface_Refugee          >().SendFullState( msg );
    SendMsg( msg );

    // Other messages
    pPion_->GetRole< PHY_RoleAction_Moving          >().SendFullState        (); // Itineraire
    pPion_->GetRole< PHY_RoleInterface_Composantes  >().SendLogisticFullState(); // Consignes logistiques maintenance
    pPion_->GetRole< human::PHY_RoleInterface_Humans       >().SendLogisticFullState(); // Consignes logistiques santé
    PHY_RoleInterface_Maintenance* roleMaintenance = pPion_->Retrieve< PHY_RoleInterface_Maintenance >();
    if( roleMaintenance )
        roleMaintenance->SendFullState();
    PHY_RoleInterface_Medical* roleMedical = pPion_->Retrieve< PHY_RoleInterface_Medical >();
    if( roleMedical )
        roleMedical->SendFullState();
    PHY_RoleInterface_Supply* roleSupply = pPion_->Retrieve< PHY_RoleInterface_Supply >();
    if( roleSupply )
        roleSupply->SendFullState();
}
