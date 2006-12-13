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

#include "Entities/Agents/MIL_AgentPion.h"
#include "MIL_PionMission.h"

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT constructor
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
MIL_AutomateMRT::MIL_AutomateMRT()
    : bActivated_  ( false )
    , missionsPion_()
    , fuseauxPion_ ()
{
    
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT destructor
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
MIL_AutomateMRT::~MIL_AutomateMRT()
{
    Cancel();
}

//=============================================================================
// MISC
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::SetMissionForPion
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
void MIL_AutomateMRT::SetMissionForPion( MIL_AgentPion& pion, MIL_PionMission& mission )
{
    MIL_PionMission*& pMission = missionsPion_[ &pion ];
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
    if( IsActivated() )
        return;

    MIL_Fuseau*& pFuseau = fuseauxPion_[ &pion ];
    if( pFuseau )
        delete pFuseau;
    pFuseau = &fuseau;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::GetFuseauForPion
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
const MIL_Fuseau* MIL_AutomateMRT::GetFuseauForPion( MIL_AgentPion& pion ) const
{
    if( IsActivated() )
        return 0;

    CIT_FuseauPionMap it = fuseauxPion_.find( &pion );
    if( it != fuseauxPion_.end() )
        return it->second;
    return 0;
}

//=============================================================================
// ACTIONS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::Activate
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_AutomateMRT::Activate()
{  
    assert( !bActivated_ );

    // Affectation des fuseaux à chaque mission pion
    for( CIT_MissionPionMap it = missionsPion_.begin(); it != missionsPion_.end(); ++it )
    {
        MIL_AgentPion&   pion    = *it->first;
        MIL_PionMission& mission = *it->second;

        const MIL_Fuseau* pFuseau = GetFuseauForPion( pion );
        if( pFuseau )
            mission.AffectFuseau( *pFuseau );
        pion.GetOrderManager().ReplaceMission( &mission );
    }
    missionsPion_.clear();
    bActivated_ = true;
    for( CIT_FuseauPionMap it = fuseauxPion_.begin(); it != fuseauxPion_.end(); ++it )
        delete it->second;
    fuseauxPion_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::Cancel
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_AutomateMRT::Cancel()
{
    bActivated_ = false;
    for( CIT_MissionPionMap it = missionsPion_.begin(); it != missionsPion_.end(); ++it )
        delete it->second;
    missionsPion_.clear();

    for( CIT_FuseauPionMap it = fuseauxPion_.begin(); it != fuseauxPion_.end(); ++it )
        delete it->second;
    fuseauxPion_ .clear();
}
