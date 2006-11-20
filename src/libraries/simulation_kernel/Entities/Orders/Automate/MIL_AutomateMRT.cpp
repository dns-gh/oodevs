// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AutomateMRT.h"

#include "Network/NET_ASN_Messages.h"
#include "Entities/Orders/Pion/MIL_PionMission_ABC.h"
#include "Entities/Orders/Automate/MIL_AutomateMission_ABC.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/MIL_AgentPion.h"

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT constructor
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
MIL_AutomateMRT::MIL_AutomateMRT()
    : bActivated_      ( false )
    , pAutomateMission_( 0 )
{
    
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT destructor
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
MIL_AutomateMRT::~MIL_AutomateMRT()
{
    if( !bActivated_ ) 
    {
        for( IT_MissionPionMap itMissionPion = missionPionMap_.begin(); itMissionPion != missionPionMap_.end(); ++itMissionPion )
        {
            MIL_PionMission_ABC& mission = *itMissionPion->second;
            delete &mission;
        }

        for( IT_FuseauPionMap itFuseauPion = fuseauPionMap_.begin(); itFuseauPion != fuseauPionMap_.end(); ++itFuseauPion )
            delete itFuseauPion->second;
    }
    missionPionMap_.clear();
    fuseauPionMap_ .clear();
    pAutomateMission_ = 0;
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::Initialize
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
void MIL_AutomateMRT::Initialize( MIL_AutomateMission_ABC& automateMission )
{
    assert( missionPionMap_.empty() );
    assert( fuseauPionMap_.empty() );
    assert( !pAutomateMission_ );

    pAutomateMission_ = &automateMission;
}

//=============================================================================
// MISC
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::GetFuseauForPion
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
const MIL_Fuseau& MIL_AutomateMRT::GetFuseauForPion( const MIL_AgentPion& pion ) const
{
    // Fuseau
    CIT_FuseauPionMap itFuseauPion = fuseauPionMap_.find( &const_cast< MIL_AgentPion& >( pion ) );
    if( itFuseauPion != fuseauPionMap_.end() )
        return *itFuseauPion->second;
    else
        return pAutomateMission_->GetAutomate().GetFuseau();
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::SetMissionForPion
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
void MIL_AutomateMRT::SetMissionForPion( MIL_AgentPion& pion, MIL_PionMission_ABC& mission )
{
    MIL_PionMission_ABC*& pMission = missionPionMap_[ &pion ];
    if( pMission )
        delete pMission;
    pMission = &mission;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::SetFuseauForPion
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
void MIL_AutomateMRT::SetFuseauForPion( MIL_AgentPion& pion, MIL_Fuseau& fuseau )
{
    MIL_Fuseau*& pFuseau = fuseauPionMap_[ &pion ];
    if( pFuseau )
        delete pFuseau;
    pFuseau = &fuseau;
}


//=============================================================================
// ACTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::Validate
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
void MIL_AutomateMRT::Validate()
{  
    assert( pAutomateMission_ );
    
    // Affectation des fuseaux/limas à chaque mission pion
    for( CIT_MissionPionMap itMissionPion = missionPionMap_.begin(); itMissionPion != missionPionMap_.end(); ++itMissionPion )
    {
        MIL_AgentPion&      pion     = *itMissionPion->first;
        MIL_PionMission_ABC& mission = *itMissionPion->second;

        mission.SetFuseau( GetFuseauForPion( pion ) ); //$$$ NAZE
    }

    // Envoi MRT à MOS    
    NET_ASN_MsgAutomateMRT asnMsg;
    asnMsg.GetAsnMsg().automate_id   = pAutomateMission_->GetAutomate().GetID();
    asnMsg.GetAsnMsg().missions.n    = missionPionMap_.size();

    if( missionPionMap_.empty() )
    {
        asnMsg.Send();
        return;
    }

    ASN1T_MsgPionOrder* pASNPionOrders = new ASN1T_MsgPionOrder[ missionPionMap_.size() ];
    uint i = 0;
    for( CIT_MissionPionMap itMissionPion = missionPionMap_.begin(); itMissionPion != missionPionMap_.end(); ++itMissionPion )
        itMissionPion->second->Serialize( pASNPionOrders[i++] );
    asnMsg.GetAsnMsg().missions.elem = pASNPionOrders;
    
    asnMsg.Send();

    // Nettoyage
    i = 0;
    for( itMissionPion = missionPionMap_.begin(); itMissionPion != missionPionMap_.end(); ++itMissionPion )
            itMissionPion->second->CleanAfterSerialization( pASNPionOrders[i++] );

    if( asnMsg.GetAsnMsg().missions.n > 0 )
        delete [] asnMsg.GetAsnMsg().missions.elem;
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::Activate
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
void MIL_AutomateMRT::Activate()
{
    assert( pAutomateMission_ );
    if( bActivated_ )
    {
        MT_LOG_WARNING( "MRT has already been activated", 1, "MIL_AutomateMRT::Activate" );
        return;
    }
    bActivated_ = true;

    // Activation des missions pion
    for( IT_MissionPionMap itMissionPion = missionPionMap_.begin(); itMissionPion != missionPionMap_.end(); ++itMissionPion )
    {
        MIL_AgentPion&       pion    = *itMissionPion->first;
        MIL_PionMission_ABC& mission = *itMissionPion->second;
        pion.GetOrderManager().OnReceivePionOrder( mission );
    }

    for( IT_FuseauPionMap itFuseauPion = fuseauPionMap_.begin(); itFuseauPion != fuseauPionMap_.end(); ++itFuseauPion )
        delete itFuseauPion->second;
    fuseauPionMap_.clear();
}


