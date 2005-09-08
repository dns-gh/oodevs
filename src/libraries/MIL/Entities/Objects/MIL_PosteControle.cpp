//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_PosteControle.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:20 $
// $Revision: 4 $
// $Workfile: MIL_PosteControle.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_PosteControle.h"
#include "MIL_RealObjectType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"

BOOST_CLASS_EXPORT_GUID( MIL_PosteControle, "MIL_PosteControle" )

//-----------------------------------------------------------------------------
// Name: MIL_PosteControle constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_PosteControle::MIL_PosteControle()
    : MIL_RealObject_ABC( MIL_RealObjectType::posteControle_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_PosteControle destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_PosteControle::~MIL_PosteControle()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_PosteControle::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_PosteControle::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PosteControle::ProcessAgentInside
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void MIL_PosteControle::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentInside( agent );

    const T_AgentSet& animators = GetAnimators();
    if( !CanInteractWithAgent( agent ) || GetConstructionPercentage() < 1. || animators.empty() )
        return;
    
    //$$$$$ NotifyAgentExternalPerception() A GICLER  - remplacer par perception::AddPerceptionLocalisationBlabla()
    for( CIT_AgentSet itAnimator = animators.begin(); itAnimator != animators.end(); ++itAnimator )
        const_cast< MIL_AgentPion& >(**itAnimator).GetRole< PHY_RolePion_Perceiver >().NotifyAgentExternalPerception( agent, PHY_PerceptionLevel::identified_ );
}

