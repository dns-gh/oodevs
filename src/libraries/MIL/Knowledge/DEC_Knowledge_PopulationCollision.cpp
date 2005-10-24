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

#include "DEC_KnowledgeBlackBoard.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_PopulationCollision, "DEC_Knowledge_PopulationCollision" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision::DEC_Knowledge_PopulationCollision( const MIL_AgentPion& agentColliding, MIL_Population& population )
    : DEC_Knowledge_ABC()
    , pAgentColliding_ ( &agentColliding )
    , pPopulation_     ( &population )
    , flows_           ()
    , concentrations_  () 
    , bIsValid_        ( false )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision constructor
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision::DEC_Knowledge_PopulationCollision()
    : DEC_Knowledge_ABC()
    , pAgentColliding_ ( 0 )
    , pPopulation_     ( 0 )
    , flows_           ()
    , concentrations_  () 
    , bIsValid_        ( false )
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
    assert( false );
    file & boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         & const_cast< MIL_AgentPion*& >( pAgentColliding_ )
         & pPopulation_
         & flows_
         & concentrations_
         & bIsValid_;
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
    bIsValid_ = true;
    flows_.insert( &flow );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::Update
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationCollision::Update( MIL_PopulationConcentration& concentration )
{
    bIsValid_ = true;
    concentrations_.insert( &concentration );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::GetMaxSpeed
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_PopulationCollision::GetMaxSpeed() const
{
    assert( pAgentColliding_ );

    T_ComposanteVolumeSet volumes_;
    pAgentColliding_->GetRole< PHY_RolePion_Composantes >().GetVisibleVolumes( volumes_ );

    MT_Float rMaxSpeed = std::numeric_limits< MT_Float >::max();
    for( CIT_PopulationFlowSet it = flows_.begin(); it != flows_.end(); ++it )
    {
        const MIL_PopulationFlow& flow = **it;
        for( CIT_ComposanteVolumeSet itVolume = volumes_.begin(); itVolume != volumes_.end(); ++itVolume )
            rMaxSpeed = std::min( rMaxSpeed, flow.GetMaxSpeed( **itVolume ) );
    }

    for( CIT_PopulationConcentrationSet it = concentrations_.begin(); it != concentrations_.end(); ++it )
    {
        const MIL_PopulationConcentration& concentration = **it;
        for( CIT_ComposanteVolumeSet itVolume = volumes_.begin(); itVolume != volumes_.end(); ++itVolume )
            rMaxSpeed = std::min( rMaxSpeed, concentration.GetMaxSpeed( **itVolume ) );
    }
    return rMaxSpeed;
}
