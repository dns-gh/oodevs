// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentsModel.h"
#include "AgentFactory_ABC.h"
#include "Agent.h"
#include "Automat.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/AutomatComposition.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: AgentsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
AgentsModel::AgentsModel( Controllers& controllers, AgentFactory_ABC& agentFactory )
    : agentFactory_( agentFactory )
    , controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
AgentsModel::~AgentsModel()
{
    Purge();
    controllers_.Remove( *this );
}


// -----------------------------------------------------------------------------
// Name: AgentsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void AgentsModel::Purge()
{
    Resolver< Agent_ABC >::DeleteAll();
    Resolver< Population_ABC >::DeleteAll();
}

namespace
{
    geometry::Point2f CartesianFromPolar( double radius, double angle )
    {
        static const double pi = 3.1415926539 / 180.0;
        const double radianAngle = pi * angle;
        return geometry::Point2f( radius * std::sin( radianAngle ), radius * std::cos( radianAngle ) );
    }

    class DiamondFormation
    {
    public:
        explicit DiamondFormation( const geometry::Point2f& center )
            : center_( geometry::Vector2f( center.X(), center.Y() ) )
            , radius_( 1 )
            , angle_ ( 0 )
            , onLevel_( 0 )
            , totalOnLevel_( 4 )
        {}

        geometry::Point2f NextPosition()
        {
            static const double spacing = 500.;
            geometry::Point2f position = CartesianFromPolar( radius_ * spacing, angle_ ) + center_;
            ++onLevel_;

            // if level is full, start a new one
            if( onLevel_ == totalOnLevel_ )
            {
                radius_ *= 1.414; // sqrt(2)
                angle_  = 45.;
                totalOnLevel_ += 4;
                onLevel_ = 0;
            }
            else
                angle_ += 360.0 / totalOnLevel_;
            return position;
        }

    private:
        DiamondFormation( const DiamondFormation& );
        DiamondFormation& operator=( const DiamondFormation& );

    private:
        const geometry::Vector2f center_;
        double angle_, radius_;
        unsigned int onLevel_, totalOnLevel_;
    };
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAutomat
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsModel::CreateAutomat( kernel::Formation_ABC& parent, const kernel::AutomatType& type, const geometry::Point2f& position )
{
    Automat_ABC* agent = agentFactory_.Create( parent, type, position );
    Resolver< Automat_ABC >::Register( agent->GetId(), *agent );

    DiamondFormation formation( position );
    CreateAgent( *agent, *type.GetTypePC(), formation.NextPosition(), true );
    Iterator< const AutomatComposition& > it = type.CreateIterator();
    while( it.HasMoreElements() )
    {
        const AutomatComposition& composition = it.NextElement();
        for( unsigned toAdd = composition.GetSensibleNumber(); toAdd > 0; --toAdd )
            CreateAgent( *agent, composition.GetType(), formation.NextPosition() );
    }
}
    
// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAutomat
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsModel::CreateAutomat( xml::xistream& xis, kernel::Formation_ABC& parent )
{
    Automat_ABC* agent = agentFactory_.Create( xis, parent );
    Resolver< Automat_ABC >::Register( agent->GetId(), *agent );
}
    
// -----------------------------------------------------------------------------
// Name: AgentsModel::GetAutomat
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
kernel::Automat_ABC& AgentsModel::GetAutomat( unsigned long id )
{
    return Resolver< Automat_ABC >::Get( id );
}
    
// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAutomat
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentsModel::FindAutomat( unsigned long id )
{
    return Resolver< Automat_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void AgentsModel::CreateAgent( Automat_ABC& parent, const AgentType& type, const geometry::Point2f& position, bool commandPost )
{
    Agent_ABC* agent = agentFactory_.Create( parent, type, position, commandPost );
    Resolver< Agent_ABC >::Register( agent->GetId(), *agent );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void AgentsModel::CreateAgent( xml::xistream& xis, Automat_ABC& parent )
{
    Agent_ABC* agent = agentFactory_.Create( xis, parent );
    Resolver< Agent_ABC >::Register( agent->GetId(), *agent );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Agent_ABC& AgentsModel::GetAgent( unsigned long id ) const
{
    return Resolver< Agent_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAgent
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent_ABC* AgentsModel::FindAgent( unsigned long id ) const
{
    return Resolver< Agent_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAllAgent
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Entity_ABC* AgentsModel::FindAllAgent( unsigned long id ) const
{
    Entity_ABC* agent = Resolver< Automat_ABC >::Find( id );
    if( agent )
        return agent;
    if( agent = Resolver< Agent_ABC >::Find( id ) )
        return agent;
    return Resolver< Population_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreatePopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void AgentsModel::CreatePopulation( Team_ABC& parent, const PopulationType& type, const geometry::Point2f& position )
{
    Population_ABC* popu = agentFactory_.Create( parent, type, position );
    Resolver< Population_ABC >::Register( popu->GetId(), *popu );
}
//
// -----------------------------------------------------------------------------
// Name: AgentsModel::GetPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population_ABC& AgentsModel::GetPopulation( unsigned long id )
{
    return Resolver< Population_ABC >::Get( id );
}
//
// -----------------------------------------------------------------------------
// Name: AgentsModel::FindPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population_ABC* AgentsModel::FindPopulation( unsigned long id )
{
    return Resolver< Population_ABC>::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::NotifyDeleted
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void AgentsModel::NotifyDeleted( const Agent_ABC& agent )
{
    Resolver< Agent_ABC >::Remove( agent.GetId() );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::NotifyDeleted
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsModel::NotifyDeleted( const kernel::Automat_ABC& agent )
{
    Resolver< Automat_ABC >::Remove( agent.GetId() );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::NotifyDeleted
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsModel::NotifyDeleted( const kernel::Population_ABC& agent )
{
    Resolver< Population_ABC >::Remove( agent.GetId() );
}
