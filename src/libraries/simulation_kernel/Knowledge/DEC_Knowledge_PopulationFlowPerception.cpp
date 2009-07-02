// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationFlowPerception.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 12:59 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_PopulationFlowPerception.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_PopulationFlowPerception.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "MIL_AgentServer.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_PopulationFlowPerception, "DEC_Knowledge_PopulationFlowPerception" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlowPerception::DEC_Knowledge_PopulationFlowPerception( DEC_Knowledge_PopulationPerception& populationKnowledge, MIL_PopulationFlow& flowPerceived )
    : pPopulationKnowledge_    ( &populationKnowledge    )
    , pPopulationFlowPerceived_( &flowPerceived )
    , shape_                   ()
    , previousShape_           ()
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlowPerception::DEC_Knowledge_PopulationFlowPerception()
    : pPopulationKnowledge_    ( 0 )
    , pPopulationFlowPerceived_( 0 )
    , shape_                   ()
    , previousShape_           ()
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlowPerception::~DEC_Knowledge_PopulationFlowPerception()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPerception::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> const_cast< DEC_Knowledge_PopulationPerception*& >( pPopulationKnowledge_ )
         >> pPopulationFlowPerceived_
         >> shape_
         >> previousShape_;

    uint nID;
    file >> nID;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );

    file >> nID;
    pPreviousPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPerception::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned current  = pCurrentPerceptionLevel_->GetID(),
             previous = pPreviousPerceptionLevel_->GetID();
    file << pPopulationKnowledge_
         << pPopulationFlowPerceived_
         << shape_
         << previousShape_
         << current
         << previous;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPerception::Prepare()
{
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_  = &PHY_PerceptionLevel::notSeen_;

    previousShape_.clear();
    shape_.swap( previousShape_ );    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::Update
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPerception::Update( const PHY_PerceptionLevel& perceptionLevel, const T_PointVector& shape )
{
    assert( perceptionLevel != PHY_PerceptionLevel::notSeen_ );
    if( perceptionLevel > *pCurrentPerceptionLevel_ )
        pCurrentPerceptionLevel_ = &perceptionLevel;   

    shape_                   = shape;
    pCurrentPerceptionLevel_ = &perceptionLevel;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationFlowPerception::Clean()
{
    return shape_.empty();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::IsPerceived
// Created: SBO 2006-01-18
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationFlowPerception::IsPerceived() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetNbrAliveHumans
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_PopulationFlowPerception::GetNbrAliveHumans() const
{
    assert( pPopulationFlowPerceived_ );
    return pPopulationFlowPerceived_->GetNbrAliveHumans();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetNbrDeadHumans
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_PopulationFlowPerception::GetNbrDeadHumans() const
{
    assert( pPopulationFlowPerceived_ );
    return pPopulationFlowPerceived_->GetNbrDeadHumans();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetAttitude
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
const MIL_PopulationAttitude& DEC_Knowledge_PopulationFlowPerception::GetAttitude() const
{
    assert( pPopulationFlowPerceived_ );
    return pPopulationFlowPerceived_->GetAttitude();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetShape
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
const T_PointVector& DEC_Knowledge_PopulationFlowPerception::GetShape() const
{
    return shape_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetDirection
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Knowledge_PopulationFlowPerception::GetDirection() const
{
    assert( pPopulationFlowPerceived_ );
    return pPopulationFlowPerceived_->GetDirection();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetSpeed
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_PopulationFlowPerception::GetSpeed() const
{
    assert( pPopulationFlowPerceived_ );
    return pPopulationFlowPerceived_->GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPerception::UpdateOnNetwork() const
{
    if( shape_ != previousShape_ )
        SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPerception::SendStateToNewClient() const
{
    assert( pPopulationKnowledge_ );
    assert( pPopulationFlowPerceived_ );
    NET_ASN_MsgPopulationFlowDetection asn;
    asn().oid            = pPopulationKnowledge_->GetAgentPerceiving().GetID();
    asn().population_oid = pPopulationKnowledge_->GetPopulationPerceived().GetID();
    asn().flow_oid       = pPopulationFlowPerceived_->GetID();
    NET_ASN_Tools::WritePath( shape_, asn().visible_flow );
    asn.Send();
    NET_ASN_Tools::Delete( asn().visible_flow );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetFlowPerceived
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
const MIL_PopulationFlow& DEC_Knowledge_PopulationFlowPerception::GetFlowPerceived() const
{
    assert( pPopulationFlowPerceived_ );
    return *pPopulationFlowPerceived_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetCurrentPerceptionLevel
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_PopulationFlowPerception::GetCurrentPerceptionLevel() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetKnowledge
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
const DEC_Knowledge_PopulationPerception& DEC_Knowledge_PopulationFlowPerception::GetKnowledge() const
{
    assert( pPopulationKnowledge_ );
    return *pPopulationKnowledge_;
}
