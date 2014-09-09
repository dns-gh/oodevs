// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentPerceptionDataIdentification.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:57 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_AgentPerceptionDataIdentification.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_AgentPerceptionDataIdentification.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "MIL_Time_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_AgentPerceptionDataIdentification )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataIdentification constructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerceptionDataIdentification::DEC_Knowledge_AgentPerceptionDataIdentification()
    : nTimeLastUpdate_( 0 )
    , pAgentType_     ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataIdentification destructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerceptionDataIdentification::~DEC_Knowledge_AgentPerceptionDataIdentification()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataIdentification::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerceptionDataIdentification::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int nID;
    file >> nTimeLastUpdate_;
    file >> nID;
    pAgentType_ = MIL_AgentTypePion::Find( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataIdentification::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerceptionDataIdentification::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned agentType = ( pAgentType_ ? pAgentType_->GetID() : (unsigned int)-1 );
    file << nTimeLastUpdate_;
    file << agentType;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataIdentification::Update
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerceptionDataIdentification::Update( const MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& perceptionLevel )
{
    if( perceptionLevel < PHY_PerceptionLevel::identified_ )
        return;
    const unsigned int nCurrentTimeStep = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    if( nTimeLastUpdate_ >= nCurrentTimeStep )
        return;
    nTimeLastUpdate_ = nCurrentTimeStep;
    pAgentType_ = &agentPerceived.GetType();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataIdentification::GetTimeLastUpdate
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_AgentPerceptionDataIdentification::GetTimeLastUpdate() const
{
    return nTimeLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataIdentification::GetAgentType
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
const MIL_AgentType_ABC* DEC_Knowledge_AgentPerceptionDataIdentification::GetAgentType() const
{
    return pAgentType_;
}
