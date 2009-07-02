// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationFlowPart.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 12:59 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_PopulationFlowPart.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_PopulationFlowPart.h"
#include "DEC_Knowledge_Population.h"
#include "DEC_Knowledge_PopulationFlowPerception.h"
#include "DEC_Knowledge_PopulationCollision.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_PopulationFlowPart, "DEC_Knowledge_PopulationFlowPart" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPart constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlowPart::DEC_Knowledge_PopulationFlowPart()
    : shape_             ()
    , rRelevance_        ( 1. )
    , rLastRelevanceSent_( 1. )
    , bPerceived_        ( true )
    , nTimeLastUpdate_   ( 0 )
{
    
}
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPart destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlowPart::~DEC_Knowledge_PopulationFlowPart()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPart::load
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPart::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> shape_
         >> rRelevance_
         >> rLastRelevanceSent_
         >> nTimeLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPart::save
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPart::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << shape_
         << rRelevance_
         << rLastRelevanceSent_
         << nTimeLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPart::Prepare
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPart::Prepare()
{
    bPerceived_ = false;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPart::Update
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationFlowPart::Update( const DEC_Knowledge_PopulationFlowPerception& perception )
{
    nTimeLastUpdate_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();    

    if( perception.GetCurrentPerceptionLevel() != PHY_PerceptionLevel::notSeen_ )
    {
        bPerceived_ = true;
        if( shape_ != perception.GetShape() )
        {
            shape_ = perception.GetShape();
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPart::Update
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationFlowPart::Update( const DEC_Knowledge_PopulationCollision& collision  )
{
    nTimeLastUpdate_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();    
    if( bPerceived_ )
        return false;
    
    T_PointVector shape( 1, collision.GetPosition() );
    if( shape_ != shape )
    {
        shape_ = shape;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPart::UpdateRelevance
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationFlowPart::UpdateRelevance( const MT_Float rMaxLifeTime )
{
    assert( rRelevance_ > 0. );

    if( bPerceived_ )
        return ChangeRelevance( 1. );

    if( shape_.empty() )
        return ChangeRelevance( 0. );

    assert( rRelevance_ >= 0. && rRelevance_ <= 1. );

    nTimeLastUpdate_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();    
    if( rMaxLifeTime == 0. )
        return ChangeRelevance( 0. );
    else
    {
        // Degradation : effacement au bout de X minutes
        const MT_Float rTimeRelevanceDegradation = ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() - nTimeLastUpdate_ ) / rMaxLifeTime;
        const MT_Float rRelevance                = std::max( 0., rRelevance_ - rTimeRelevanceDegradation );       
        return ChangeRelevance( rRelevance );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPart::Serialize
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPart::Serialize( ASN1T_FlowPart& asn ) 
{
    NET_ASN_Tools::WritePath( shape_, asn.forme );
    asn.pertinence      = (uint)( rRelevance_ * 100. );
    rLastRelevanceSent_ = rRelevance_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPart::Clean
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationFlowPart::Clean()
{
    return rRelevance_ <= 0. || shape_.empty();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPart::ChangeRelevance
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationFlowPart::ChangeRelevance( MT_Float rNewRelevance )
{
    if( rRelevance_ == rNewRelevance )
        return false;

    static const MT_Float rDeltaForNetwork = 0.05;
    if( fabs( rLastRelevanceSent_ - rNewRelevance ) > rDeltaForNetwork || rNewRelevance == 0. || rNewRelevance == 1. )
    {
        rRelevance_ = rNewRelevance;
        return true;
    }
    rRelevance_ = rNewRelevance;
    return false;
}
