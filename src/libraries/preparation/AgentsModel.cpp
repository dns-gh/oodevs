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
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
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

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void AgentsModel::CreateAgent( const Agent_ABC& parent, const AgentType& type, const geometry::Point2f& position )
{
    Agent_ABC* agent = agentFactory_.Create( parent, type, position );
    Resolver< Agent_ABC >::Register( agent->GetId(), *agent );
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
            static const double spacing = 100.;
            geometry::Point2f position = CartesianFromPolar( radius_ * spacing, angle_ ) + center_;
            ++onLevel_;

            // if level is full, start a new one
            if( onLevel_ == totalOnLevel_ )
            {
                radius_ *= 1.414; // sqrt(2)
                angle_  = 0.;
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
// Name: AgentsModel::CreateAgent
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
void AgentsModel::CreateAgent( const KnowledgeGroup_ABC& parent, const AutomatType& type, const geometry::Point2f& position )
{
    Agent_ABC* agent = agentFactory_.Create( parent, type, position );
    Resolver< Agent_ABC >::Register( agent->GetId(), *agent );

    DiamondFormation formation( position );
    CreateAgent( *agent, *type.GetTypePC(), formation.NextPosition() );
    Iterator< const AgentType& > it = type.CreateIterator();
    while( it.HasMoreElements() )
    {
        const AgentType& agentType = it.NextElement();
        CreateAgent( *agent, agentType, formation.NextPosition() );
    }
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
    Entity_ABC* agent = Resolver< Agent_ABC >::Find( id );
    if( ! agent )
        agent = Resolver< Population_ABC >::Find( id );
    return agent;
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreatePopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void AgentsModel::CreatePopulation( const Team_ABC& parent, const PopulationType& type, const geometry::Point2f& position )
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
// Name: AgentsModel::Serialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void AgentsModel::Serialize( xml::xostream& xos ) const
{
    xos << start( "Automates" );
    for( Resolver< Agent_ABC >::CIT_Elements it = Resolver< Agent_ABC >::elements_.begin(); it != Resolver< Agent_ABC >::elements_.end(); ++it )
        if( it->second->GetAutomatType() )
            static_cast< const Agent* >( it->second )->Serialize( xos ); // $$$$ SBO 2006-09-06: 
    xos << end();

    xos << start( "Pions" );
    for( Resolver< Agent_ABC >::CIT_Elements it = Resolver< Agent_ABC >::elements_.begin(); it != Resolver< Agent_ABC >::elements_.end(); ++it )
        if( !it->second->GetAutomatType() )
            static_cast< const Agent* >( it->second )->Serialize( xos ); // $$$$ SBO 2006-09-06: 
    xos << end();

    xos << start( "Populations" );
//    for( Resolver< Population_ABC >::CIT_Elements it = Resolver< Population_ABC >::elements_.begin(); it != Resolver< Population_ABC >::elements_.end(); ++it )
//        static_cast< const Population* >( it->second )->Serialize( xos );
    xos << end();
}
