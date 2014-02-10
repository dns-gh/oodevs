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

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_PopulationCollision.h"
#include "DEC_Knowledge_Population.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_PopulationCollision )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision::DEC_Knowledge_PopulationCollision( const MIL_Agent_ABC& agentColliding, MIL_Population& population )
    : pAgentColliding_( &agentColliding )
    , pPopulation_    ( &population )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision constructor
// Created: LDC 2013-10-04
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision::DEC_Knowledge_PopulationCollision( MIL_Population& population )
    : pAgentColliding_( 0 )
    , pPopulation_    ( &population )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision constructor
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision::DEC_Knowledge_PopulationCollision()
    : pAgentColliding_( 0 )
    , pPopulation_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision::~DEC_Knowledge_PopulationCollision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_PopulationCollision::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         & const_cast< MIL_Agent_ABC*& >( pAgentColliding_ )
         & pPopulation_
         & flows_
         & concentrations_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::Update
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationCollision::Update( MIL_PopulationFlow& flow )
{
    flows_.insert( &flow );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::Update
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationCollision::Update( MIL_PopulationConcentration& concentration )
{
    concentrations_.insert( &concentration );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::PublishKnowledges
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationCollision::PublishKnowledges( DEC_Knowledge_Population& knowledge ) const
{
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        knowledge.Update( *this, **it  );
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        knowledge.Update( *this, **it );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::GetPionMaxSpeed
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
double DEC_Knowledge_PopulationCollision::GetPionMaxSpeed() const
{
    if( !pAgentColliding_ )
        throw std::runtime_error( "Collision has no agent" );
    double rMaxSpeed = std::numeric_limits< double >::max();
    auto volumes = pAgentColliding_->GetRole< PHY_RolePion_Composantes >().GetVisibleVolumes();
    for( auto itVolume = volumes.begin(); itVolume != volumes.end(); ++itVolume )
    {
        for( auto it = flows_.begin(); it != flows_.end(); ++it )
            rMaxSpeed = std::min( rMaxSpeed, (*it)->GetPionMaxSpeed( **itVolume ) );
        for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
            rMaxSpeed = std::min( rMaxSpeed, (*it)->GetPionMaxSpeed( **itVolume ) );
    }
    return rMaxSpeed;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::GetPionReloadingTimeFactor
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
double DEC_Knowledge_PopulationCollision::GetPionReloadingTimeFactor() const
{
    if( !pAgentColliding_ )
        throw std::runtime_error( "Collision has no agent" );
    double rFactor = 1.;
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        rFactor = std::max( rFactor, (*it)->GetPionReloadingTimeFactor() );
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        rFactor = std::max( rFactor, (*it)->GetPionReloadingTimeFactor() );
    return rFactor;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::GetMaxPopulationDensity
// Created: NLD 2005-10-27
// -----------------------------------------------------------------------------
double DEC_Knowledge_PopulationCollision::GetMaxPopulationDensity() const
{
    double rMaxDensity = 0.;
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        rMaxDensity = std::max( rMaxDensity, (*it)->GetDensity() );
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        rMaxDensity = std::max( rMaxDensity, (*it)->GetDensity() );
    return rMaxDensity;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::GetPositions
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationCollision::GetPositions( std::vector< MT_Vector2D >& positions ) const
{
    if( pAgentColliding_ )
        positions.push_back( pAgentColliding_->GetRole< PHY_RoleInterface_Location >().GetPosition() );
    else
    {
        for ( auto it = flows_.begin(); it != flows_.end(); ++it )
        {
            const T_PointList& flowShape = (*it)->GetFlowShape();
            positions.insert( positions.end(), flowShape.begin(), flowShape.end() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::GetPopulation
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
MIL_Population& DEC_Knowledge_PopulationCollision::GetPopulation() const
{
    assert( pPopulation_ );
    return *pPopulation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::GetAgentColliding
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
const MIL_Agent_ABC& DEC_Knowledge_PopulationCollision::GetAgentColliding() const
{
    if( !pAgentColliding_ )
        throw std::runtime_error( "Collision has no agent" );
    return *pAgentColliding_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationCollision::Prepare()
{
    flows_.clear();
    concentrations_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationCollision::Clean()
{
    return flows_.empty() && concentrations_.empty();
}
