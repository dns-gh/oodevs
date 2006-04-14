// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationCollision.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:32 $
// $Revision: 5 $
// $Workfile: DEC_Knowledge_PopulationCollision.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Knowledge_PopulationCollision.h"

#include "DEC_Knowledge_Population.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"

using namespace DIN;

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_PopulationCollision, "DEC_Knowledge_PopulationCollision" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision::DEC_Knowledge_PopulationCollision( const MIL_AgentPion& agentColliding, MIL_Population& population )
    : DEC_Knowledge_ABC      ()
    , pAgentColliding_       ( &agentColliding )
    , pPopulation_           ( &population )
    , flows_                 ()
    , concentrations_        () 
    , previousFlows_         ()
    , previousConcentrations_()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision constructor
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision::DEC_Knowledge_PopulationCollision()
    : DEC_Knowledge_ABC      ()
    , pAgentColliding_       ( 0 )
    , pPopulation_           ( 0 )
    , flows_                 ()
    , concentrations_        () 
    , previousFlows_         ()
    , previousConcentrations_()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision::~DEC_Knowledge_PopulationCollision()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_Knowledge_PopulationCollision::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         & const_cast< MIL_AgentPion*& >( pAgentColliding_ )
         & pPopulation_
         & flows_
         & concentrations_
         & previousFlows_;
         & previousConcentrations_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::Update
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationCollision::Update( MIL_PopulationFlow& flow )
{
    flows_.insert( &flow ).second;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::Update
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationCollision::Update( MIL_PopulationConcentration& concentration )
{
    concentrations_.insert( &concentration ).second;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::PublishKnowledges
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationCollision::PublishKnowledges( DEC_Knowledge_Population& knowledge ) const
{
    for( CIT_PopulationConcentrationSet it = concentrations_.begin(); it != concentrations_.end(); ++it )
        knowledge.Update( *this, **it  );

    for( CIT_PopulationFlowSet it = flows_.begin(); it != flows_.end(); ++it )
        knowledge.Update( *this, **it );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::GetPionMaxSpeed
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_PopulationCollision::GetPionMaxSpeed() const
{
    assert( pAgentColliding_ );

    T_ComposanteVolumeSet volumes_;
    pAgentColliding_->GetRole< PHY_RolePion_Composantes >().GetVisibleVolumes( volumes_ ); /// $$$ BOF

    MT_Float rMaxSpeed = std::numeric_limits< MT_Float >::max();
    for( CIT_PopulationFlowSet it = flows_.begin(); it != flows_.end(); ++it )
    {
        const MIL_PopulationFlow& flow = **it;
        for( CIT_ComposanteVolumeSet itVolume = volumes_.begin(); itVolume != volumes_.end(); ++itVolume )
            rMaxSpeed = std::min( rMaxSpeed, flow.GetPionMaxSpeed( **itVolume ) );
    }

    for( CIT_PopulationConcentrationSet it = concentrations_.begin(); it != concentrations_.end(); ++it )
    {
        const MIL_PopulationConcentration& concentration = **it;
        for( CIT_ComposanteVolumeSet itVolume = volumes_.begin(); itVolume != volumes_.end(); ++itVolume )
            rMaxSpeed = std::min( rMaxSpeed, concentration.GetPionMaxSpeed( **itVolume ) );
    }
    return rMaxSpeed;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::GetPionReloadingTimeFactor
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_PopulationCollision::GetPionReloadingTimeFactor() const
{
    assert( pAgentColliding_ );

    T_ComposanteVolumeSet volumes_;

    MT_Float rFactor = 1.;
    for( CIT_PopulationFlowSet it = flows_.begin(); it != flows_.end(); ++it )
        rFactor = std::max( rFactor, (**it).GetPionReloadingTimeFactor() );

    for( CIT_PopulationConcentrationSet it = concentrations_.begin(); it != concentrations_.end(); ++it )
        rFactor = std::max( rFactor, (**it).GetPionReloadingTimeFactor() );

    return rFactor;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::GetMaxPopulationDensity
// Created: NLD 2005-10-27
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_PopulationCollision::GetMaxPopulationDensity() const
{
    MT_Float rMaxDensity = 0.;
    for( CIT_PopulationFlowSet it = flows_.begin(); it != flows_.end(); ++it )
        rMaxDensity = std::max( rMaxDensity, (**it).GetDensity() );

    for( CIT_PopulationConcentrationSet it = concentrations_.begin(); it != concentrations_.end(); ++it )
        rMaxDensity = std::max( rMaxDensity, (**it).GetDensity() );
    return rMaxDensity;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::GetPosition
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Knowledge_PopulationCollision::GetPosition() const
{
    assert( pAgentColliding_ );
    return pAgentColliding_->GetRole< PHY_RolePion_Location >().GetPosition(); //$$$
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationCollision::UpdateOnNetwork() const
{
    if( previousFlows_ != flows_ || previousConcentrations_ != concentrations_ )
        SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationCollision::SendStateToNewClient() const
{
    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN_BufferedMessage msg = msgMgr.BuildMessage();

    assert( pAgentColliding_ );
    assert( pPopulation_ );

    msg << (uint32)pAgentColliding_->GetID();
    msg << (uint32)pPopulation_    ->GetID();

    msg << (uint32)concentrations_.size();
    for( CIT_PopulationConcentrationSet it = concentrations_.begin(); it != concentrations_.end(); ++it )
        msg << (uint32)(**it).GetID();

    msg << (uint32)flows_.size();
    for( CIT_PopulationFlowSet it = flows_.begin(); it != flows_.end(); ++it )
        msg << (uint32)(**it).GetID();

    msgMgr.SendMsgPopulationCollision( msg );
}
