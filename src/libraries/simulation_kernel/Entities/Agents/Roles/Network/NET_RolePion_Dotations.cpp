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
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Humans/PHY_RolePion_Humans.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Agents/Roles/NBC/PHY_RolePion_NBC.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePion_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_RolePion_Medical.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Entities/Agents/Roles/Refugee/PHY_RolePion_Refugee.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/DEC_AutomateDecision.h"

#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( NET_RolePion_Dotations, "NET_RolePion_Dotations" )

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
// Name: NET_RolePion_Dotations constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
NET_RolePion_Dotations::NET_RolePion_Dotations()
    : pPion_                     ( 0 )
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
    file & const_cast< MIL_AgentPion*& >( pPion_ )
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

    if(    pPion_->GetRole< PHY_RolePion_Posture          >().HasChanged()
        || pPion_->GetRole< PHY_RolePion_Composantes      >().HasChanged()
        || pPion_->GetRole< PHY_RolePion_Location         >().HasLocationChanged()
        || pPion_->GetRole< PHY_RolePion_Location         >().HasSpeedChanged()
        || pPion_->GetRole< PHY_RolePion_Reinforcement    >().HasChanged()
        || pPion_->GetRole< PHY_RolePion_NBC              >().HasChanged()
        || pPion_->GetRole< PHY_RolePion_Communications   >().HasChanged()
        || pPion_->GetRole< PHY_RolePion_Transported      >().HasChanged()
        || pPion_->GetRole< PHY_RoleAction_Transport      >().HasChanged()
        || pPion_->GetRole< PHY_RolePion_HumanFactors     >().HasChanged()
        || pPion_->GetRole< PHY_RoleAction_Loading        >().HasChanged()
        || pPion_->GetRole< PHY_RolePion_Surrender        >().HasChanged()
        || pPion_->GetRole< PHY_RolePion_Refugee          >().HasChanged()
        || pPion_->GetRole< PHY_RolePion_Perceiver        >().HasRadarStateChanged() 
        || pPion_->GetRole< DEC_RolePion_Decision         >().HasStateChanged()
        || pPion_->GetRole< PHY_RolePion_Dotations        >().HasChanged()
        || pPion_->GetRole< PHY_RolePion_Humans           >().HasChanged()
        || pPion_->GetRole< PHY_RolePion_Composantes      >().HasChanged()
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
        if(    pPion_->GetRole< PHY_RolePion_Location    >().HasLocationChanged()
            || pPion_->GetRole< PHY_RolePion_Composantes >().HasChanged()
            || pPion_->GetRole< PHY_RolePion_Perceiver   >().HasChanged()
            || pPion_->GetRole< PHY_RoleAction_Loading   >().HasChanged()
            || pPion_->GetRole< PHY_RolePion_Transported >().HasChanged()
            || pPion_->GetRole< PHY_RolePion_Surrender   >().HasChanged()
            || MIL_AgentServer::GetWorkspace().GetAgentServer().MustInitUnitVisionCones() )
            pPion_->GetRole< PHY_RolePion_Perceiver >().SendDebugState(); //$$ BOF
    }

    if( !DataUpdated() )
        return;

    NET_ASN_MsgUnitAttributes msg;
    assert( pPion_ );
    msg().oid = pPion_->GetID();

    pPion_->GetRole< PHY_RolePion_Humans         >().SendChangedState( msg );
    pPion_->GetRole< PHY_RolePion_Dotations      >().SendChangedState( msg );
    pPion_->GetRole< PHY_RolePion_Posture        >().SendChangedState( msg ); // Current, old, pourcentage
    pPion_->GetRole< PHY_RolePion_Location       >().SendChangedState( msg ); // Direction, speed, altitude, position
    pPion_->GetRole< PHY_RolePion_Composantes    >().SendChangedState( msg ); // Etat ops
    pPion_->GetRole< PHY_RolePion_Reinforcement  >().SendChangedState( msg ); // Reinforcement
    pPion_->GetRole< PHY_RoleAction_Loading      >().SendChangedState( msg ); // Loading state
    pPion_->GetRole< PHY_RolePion_NBC            >().SendChangedState( msg ); // NBC state
    pPion_->GetRole< PHY_RolePion_Communications >().SendChangedState( msg ); // Brouillage / silence radio
    pPion_->GetRole< PHY_RolePion_HumanFactors   >().SendChangedState( msg );
    pPion_->GetRole< PHY_RolePion_Transported    >().SendChangedState( msg );
    pPion_->GetRole< PHY_RoleAction_Transport    >().SendChangedState( msg );
    pPion_->GetRole< PHY_RolePion_Perceiver      >().SendChangedState( msg );
    pPion_->GetRole< PHY_RolePion_Surrender      >().SendChangedState( msg );
    pPion_->GetRole< PHY_RolePion_Refugee        >().SendChangedState( msg );
    pPion_->GetRole< DEC_RolePion_Decision       >().SendChangedState( msg ); // Dec states

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
    pPion_->GetRole< PHY_RoleAction_Moving    >().SendChangedState        (); // Itineraire
    pPion_->GetRole< PHY_RolePion_Composantes >().SendLogisticChangedState(); // Consignes logistiques maintenance
    pPion_->GetRole< PHY_RolePion_Humans      >().SendLogisticChangedState(); // Consignes logistiques santé
    pPion_->GetRole< PHY_RolePion_Maintenance >().SendChangedState        ();
    pPion_->GetRole< PHY_RolePion_Medical     >().SendChangedState        ();
    pPion_->GetRole< PHY_RolePion_Supply      >().SendChangedState        ();
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
    pPion_->GetRole< PHY_RolePion_Humans         >().SendFullState( msg );
    pPion_->GetRole< PHY_RolePion_Dotations      >().SendFullState( msg );
    pPion_->GetRole< PHY_RolePion_Posture        >().SendFullState( msg ); // Current, old, pourcentage
    pPion_->GetRole< PHY_RolePion_Location       >().SendFullState( msg ); // Direction, speed, altitude, position
    pPion_->GetRole< PHY_RolePion_Composantes    >().SendFullState( msg ); // Etat ops
    pPion_->GetRole< PHY_RolePion_Reinforcement  >().SendFullState( msg ); // Reinforcement
    pPion_->GetRole< PHY_RoleAction_Loading      >().SendFullState( msg ); // Loading state
    pPion_->GetRole< DEC_RolePion_Decision       >().SendFullState( msg ); // Dec states
    pPion_->GetRole< PHY_RolePion_NBC            >().SendFullState( msg ); // NBC state
    pPion_->GetRole< PHY_RolePion_Communications >().SendFullState( msg ); // Brouillage
    pPion_->GetRole< PHY_RolePion_HumanFactors   >().SendFullState( msg );
    pPion_->GetRole< PHY_RolePion_Transported    >().SendFullState( msg );
    pPion_->GetRole< PHY_RoleAction_Transport    >().SendFullState( msg );
    pPion_->GetRole< PHY_RolePion_Perceiver      >().SendFullState( msg );
    pPion_->GetRole< PHY_RolePion_Surrender      >().SendFullState( msg );
    pPion_->GetRole< PHY_RolePion_Refugee        >().SendFullState( msg );
    SendMsg( msg );

    // Other messages
    pPion_->GetRole< PHY_RoleAction_Moving    >().SendFullState        (); // Itineraire
    pPion_->GetRole< PHY_RolePion_Composantes >().SendLogisticFullState(); // Consignes logistiques maintenance
    pPion_->GetRole< PHY_RolePion_Humans      >().SendLogisticFullState(); // Consignes logistiques santé
    pPion_->GetRole< PHY_RolePion_Maintenance >().SendFullState        ();
    pPion_->GetRole< PHY_RolePion_Medical     >().SendFullState        ();
    pPion_->GetRole< PHY_RolePion_Supply      >().SendFullState        ();
}
