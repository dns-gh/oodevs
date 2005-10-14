// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationFlow.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 12:59 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_PopulationFlow.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Knowledge_PopulationFlow.h"

#include "DEC_Knowledge_Population.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"
#include "Entities/Agents/s/PHY_Level.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "MIL_AgentServer.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_PopulationFlow, "DEC_Knowledge_PopulationFlow" )

using namespace DIN;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlow::DEC_Knowledge_PopulationFlow( DEC_Knowledge_Population& populationKnowledge, MIL_PopulationFlow& flowPerceived )
    : pPopulationKnowledge_    ( &populationKnowledge    )
    , pPopulationFlowPerceived_( &flowPerceived )
    , shape_                   ()
    , previousShape_           ()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlow::DEC_Knowledge_PopulationFlow()
    : pPopulationKnowledge_    ( 0 )
    , pPopulationFlowPerceived_( 0 )
    , shape_                   ()
    , previousShape_           ()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlow::~DEC_Knowledge_PopulationFlow()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::load( MIL_CheckPointInArchive& file, const uint )
{
    assert( false );
//    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
//
//    file >> const_cast< MIL_AgentPion*& >( pAgentPerceiving_ ) 
//         >> pPopulationPerceived_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( false );
//    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
//         << const_cast< MIL_AgentPion*& >( pAgentPerceiving_ ) 
//         << pPopulationPerceived_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::Prepare()
{
    previousShape_.clear();
    shape_.swap( previousShape_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::Update
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::Update( const PHY_Level& Level, const T_PointVector& shape )
{
    assert( Level != PHY_Level::notSeen_ );
    shape_ = shape;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationFlow::Clean()
{
    return shape_.empty();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::UpdateOnNetwork() const
{
    if( shape_ != previousShape_ )
        SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlow::SendStateToNewClient() const
{
    assert( pPopulationKnowledge_ );
    assert( pPopulationFlowPerceived_ );

    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN_BufferedMessage msg = msgMgr.BuildMessage();
    msg << (uint32)pPopulationKnowledge_->GetAgentPerceiving().GetID();
    msg << (uint32)pPopulationKnowledge_->GetPopulationPerceived().GetID();
    msg << (uint32)pPopulationFlowPerceived_->GetID();

    msg << (uint32)shape_.size();
    for( CIT_PointVector it = shape_.begin(); it != shape_.end(); ++it )
        msg << *it;
    msgMgr.SendMsgPopulationFlowInterVisibility( msg );
}
