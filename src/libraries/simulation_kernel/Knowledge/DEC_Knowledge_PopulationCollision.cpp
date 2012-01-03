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
#include <boost/serialization/vector.hpp>
#include <boost/serialization/set.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_PopulationCollision )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision::DEC_Knowledge_PopulationCollision( const MIL_Agent_ABC& agentColliding, MIL_Population& population )
    : DEC_Knowledge_ABC()
    , pAgentColliding_( &agentColliding )
    , pPopulation_    ( &population )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision constructor
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision::DEC_Knowledge_PopulationCollision()
    : DEC_Knowledge_ABC()
    , pAgentColliding_( 0 )
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
         & concentrations_
         & previousFlows_;
         & previousConcentrations_;
}

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
    concentrations_.insert( &concentration );
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
double DEC_Knowledge_PopulationCollision::GetPionMaxSpeed() const
{
    assert( pAgentColliding_ );
    T_ComposanteVolumeSet volumes_;
    pAgentColliding_->GetRole< PHY_RolePion_Composantes >().GetVisibleVolumes( volumes_ );
    double rMaxSpeed = std::numeric_limits< double >::max();
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
double DEC_Knowledge_PopulationCollision::GetPionReloadingTimeFactor() const
{
    assert( pAgentColliding_ );
    T_ComposanteVolumeSet volumes_;
    double rFactor = 1.;
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
double DEC_Knowledge_PopulationCollision::GetMaxPopulationDensity() const
{
    double rMaxDensity = 0.;
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
    return pAgentColliding_->GetRole< PHY_RoleInterface_Location >().GetPosition();
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
    assert( pAgentColliding_ );
    return *pAgentColliding_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationCollision::Prepare()
{
    previousFlows_.clear();
    previousConcentrations_.clear();
    std::swap( previousFlows_, flows_ );
    std::swap( previousConcentrations_, concentrations_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationCollision::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationCollision::Clean()
{
    return flows_.empty() && concentrations_.empty();
}
