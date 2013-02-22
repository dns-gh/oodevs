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
#include "Entities/Orders/MIL_Mission_ABC.h"

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT constructor
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
MIL_AutomateMRT::MIL_AutomateMRT()
    : bActivated_( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT destructor
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
MIL_AutomateMRT::~MIL_AutomateMRT()
{
    Cancel();
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::SetMissionForPion
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
void MIL_AutomateMRT::SetMissionForPion( MIL_AgentPion& pion, const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    missionsPion_.insert( std::make_pair( &pion, mission ) );
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::SetFuseauForPion
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
void MIL_AutomateMRT::SetFuseauForPion( MIL_AgentPion& pion, MIL_Fuseau& fuseau )
{
    if( IsActivated() )
        return;
    fuseauxPion_[ &pion ].reset( &fuseau );
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::GetFuseauForPion
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
const MIL_Fuseau* MIL_AutomateMRT::GetFuseauForPion( MIL_AgentPion& pion ) const
{
    if( IsActivated() )
        return 0;
    auto it = fuseauxPion_.find( &pion );
    if( it != fuseauxPion_.end() )
        return it->second.get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::Activate
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_AutomateMRT::Activate()
{
    assert( !bActivated_ );
    // Affectation des fuseaux à chaque mission pion
    for( auto it = missionsPion_.begin(); it != missionsPion_.end(); ++it )
    {
        MIL_AgentPion& pion = *it->first;
        const MIL_Fuseau* pFuseau = GetFuseauForPion( pion );
        if( pFuseau )
            it->second->AffectFuseau( *pFuseau );
        pion.GetOrderManager().ReplaceMission( it->second );
    }
    missionsPion_.clear();
    fuseauxPion_.clear();
    bActivated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::Cancel
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_AutomateMRT::Cancel()
{
    bActivated_ = false;
    for( auto it = missionsPion_.begin(); it != missionsPion_.end(); ++it )
        it->second->Stop( it->second );
    missionsPion_.clear();
    fuseauxPion_.clear();
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::IsActivated
// Created: NLD 2003-04-16
//-----------------------------------------------------------------------------
bool MIL_AutomateMRT::IsActivated() const
{
    return bActivated_;
}
