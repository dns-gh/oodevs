// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DecontaminationCapacity.h"
#include "MIL_Object_ABC.h"
#include "AnimatorAttribute.h"
#include "ConstructionAttribute.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Populations/MIL_Population.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DecontaminationCapacity )

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
DecontaminationCapacity::DecontaminationCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
DecontaminationCapacity::DecontaminationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity constructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
DecontaminationCapacity::DecontaminationCapacity( const DecontaminationCapacity& /*rhs*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity destructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
DecontaminationCapacity::~DecontaminationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity::serialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void DecontaminationCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void DecontaminationCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void DecontaminationCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    DecontaminationCapacity* capacity = new DecontaminationCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity::ProcessAgentInside
// Created: LGY 2011-11-18
// -----------------------------------------------------------------------------
void DecontaminationCapacity::ProcessAgentInside( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    if( std::find( agentsInside_.begin(), agentsInside_.end(), &agent ) == agentsInside_.end() )
    {
        agent.GetRole< nbc::PHY_RoleInterface_NBC >().TemporaryImmunizeAgent( true );
        agentsInside_.push_back( &agent );
    }
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity::ProcessPopulationInside
// Created: LGY 2011-11-18
// -----------------------------------------------------------------------------
void DecontaminationCapacity::ProcessPopulationInside( MIL_Object_ABC& /*object*/, MIL_PopulationElement_ABC& population )
{
    if( std::find( populationsInside_.begin(), populationsInside_.end(), &population ) == populationsInside_.end() )
        populationsInside_.push_back( &population );
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity::Update
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void DecontaminationCapacity::Update( MIL_Object_ABC& object, unsigned int /*time*/ )
{
    if( object.IsMarkedForDestruction() || object.GetAttribute< ConstructionAttribute >().GetState() < 1. )
        return;
    if( !agents_.empty() )
    {
        MIL_Agent_ABC* agent = agents_.front();
        if( agent && std::find( agentsInside_.begin(), agentsInside_.end(), agent ) != agentsInside_.end() )
            if( Decontaminate( object, *agent ) )
                agents_.pop_front();
    }
    else
    {
        if( !populations_.empty() )
        {
            MIL_Population* population = populations_.front();
            MIL_PopulationElement_ABC* element = GetElement( population );
            if( population && element )
            {
                Decontaminate( object, *element );
                if( population->GetContaminatedHumans() == 0u )
                    populations_.pop_front();
            }
        }
    }
    populationsInside_.clear();
    for( IT_Agents it = agentsInside_.begin(); it != agentsInside_.end(); ++it )
        (*it)->GetRole< nbc::PHY_RoleInterface_NBC >().TemporaryImmunizeAgent( false );
    agentsInside_.clear();
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity::Decontaminated
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
bool DecontaminationCapacity::Decontaminate( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    nbc::PHY_RoleInterface_NBC& roleNBC = agent.GetRole< nbc::PHY_RoleInterface_NBC >();
    AnimatorAttribute& animators = object.GetAttribute< AnimatorAttribute >();
    const double rRatioWorkers = animators.GetAnimatorsRatio();
    if( rRatioWorkers > 0 )
        roleNBC.Decontaminate( rRatioWorkers );
    return !roleNBC.IsContaminated() && roleNBC.GetDecontaminationState() == 1.;
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity::Decontaminate
// Created: LGY 2011-11-09
// -----------------------------------------------------------------------------
void DecontaminationCapacity::Decontaminate( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population )
{
    AnimatorAttribute& animators = object.GetAttribute< AnimatorAttribute >();
    const double rRatioWorkers = animators.GetAnimatorsRatio();
    if( rRatioWorkers > 0 )
        population.ApplyDecontamination( rRatioWorkers );
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity::QueueForDecontamination
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void DecontaminationCapacity::QueueForDecontamination( MIL_Agent_ABC& agent )
{
    if( std::find( agents_.begin(), agents_.end(), &agent ) == agents_.end() )
    {
        agent.GetRole< nbc::PHY_RoleInterface_NBC >().StartDecontamination();
        agents_.push_back( &agent );
    }
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity::QueueForDecontamination
// Created: LGY 2011-11-09
// -----------------------------------------------------------------------------
void DecontaminationCapacity::QueueForDecontamination( MIL_Population& population )
{
    if( std::find( populations_.begin(), populations_.end(), &population ) == populations_.end() )
        populations_.push_back( &population );
}

// -----------------------------------------------------------------------------
// Name: DecontaminationCapacity::GetElement
// Created: LGY 2011-11-18
// -----------------------------------------------------------------------------
MIL_PopulationElement_ABC* DecontaminationCapacity::GetElement( MIL_Population* population )
{
    if( !population )
        return 0;
    for( IT_Populations it = populationsInside_.begin(); it != populationsInside_.end(); ++it )
        if( (*it)->GetPopulation().GetID() == population->GetID() && (*it)->GetContaminatedHumans() > 0 )
            return *it;
    return 0;
}
