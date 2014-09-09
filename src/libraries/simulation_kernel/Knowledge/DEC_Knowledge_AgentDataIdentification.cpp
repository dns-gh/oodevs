// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentDataIdentification.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:48 $
// $Revision: 6 $
// $Workfile: DEC_Knowledge_AgentDataIdentification.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_AgentDataIdentification.h"
#include "DEC_Knowledge_AgentPerceptionDataIdentification.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_AgentDataIdentification )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification constructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentDataIdentification::DEC_Knowledge_AgentDataIdentification()
    : nTimeLastUpdate_  ( 0 )
    , pAgentType_       ( 0 )
    , bAgentTypeUpdated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification destructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentDataIdentification::~DEC_Knowledge_AgentDataIdentification()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataIdentification::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> nTimeLastUpdate_;
    unsigned int nID;
    file >> nID;
    pAgentType_ = MIL_AgentTypePion::Find( nID );
    file >> bAgentTypeUpdated_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataIdentification::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned agentType = ( pAgentType_ ? pAgentType_->GetID() : (unsigned int)-1 );
    file << nTimeLastUpdate_;
    file << agentType;
    file << bAgentTypeUpdated_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::Prepare
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataIdentification::Prepare()
{
    bAgentTypeUpdated_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::DoUpdate
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Knowledge_AgentDataIdentification::DoUpdate( const T& data )
{
    if( data.GetTimeLastUpdate() <= nTimeLastUpdate_ )
        return;
    if( !pAgentType_ )
    {
        pAgentType_ = data.GetAgentType();
        assert( pAgentType_ );
        bAgentTypeUpdated_ = true;
    }
    nTimeLastUpdate_ = data.GetTimeLastUpdate();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::Update
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataIdentification::Update( const DEC_Knowledge_AgentPerceptionDataIdentification& data )
{
    DoUpdate( data );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::Update
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataIdentification::Update( const DEC_Knowledge_AgentDataIdentification& data )
{
    DoUpdate( data );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::SendChangedState
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataIdentification::SendChangedState( sword::UnitKnowledgeUpdate& /*asnMsg*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::SendFullState
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataIdentification::SendFullState( sword::UnitKnowledgeUpdate& /*asnMsg*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::HasChanged
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataIdentification::HasChanged() const
{
    return bAgentTypeUpdated_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::GetTimeLastUpdate
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_AgentDataIdentification::GetTimeLastUpdate() const
{
    return nTimeLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::GetAgentType
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
const MIL_AgentType_ABC* DEC_Knowledge_AgentDataIdentification::GetAgentType() const
{
    return pAgentType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::Extrapolate
// Created: JVT 2004-11-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataIdentification::Extrapolate( const MIL_Agent_ABC& /*agentKnown*/ )
{
    // NOTHING
}
