// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Agent.h"
#include "Automat.h"
#include "Listener_ABC.h"
#include "Model.h"
#include "Scheduler_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/StaticModel.h"
#include "protocol/Protocol.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Model::Model( const kernel::StaticModel& staticModel, Scheduler_ABC& scheduler )
    : staticModel_( staticModel )
    , scheduler_  ( scheduler )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Model::~Model()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model::OnReceiveMessage
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Model::OnReceiveMessage( const sword::SimToClient& message )
{
    if( message.message().has_unit_creation() )
         CreateAgent( message.message().unit_creation() );
    if( message.message().has_automat_creation() )
         CreateAutomat( message.message().automat_creation() );

    if( message.message().has_frag_order_ack() )
    {
        if( message.message().frag_order_ack().error_code() )
            BOOST_FOREACH( const Listener_ABC* listener, listeners_ )
                listener->FragOrderErrorAck( message.message().frag_order_ack().tasker() );
        else
            BOOST_FOREACH( Listener_ABC* listener, listeners_ )
                listener->FragOrderAcknowledged( message.message().frag_order_ack().tasker() );
    }
    if( message.message().has_order_ack() )
    {
        if( message.message().order_ack().error_code() )
            BOOST_FOREACH( const Listener_ABC* listener, listeners_ )
                listener->MissionErrorAck( message.message().order_ack().tasker() );
        else 
            BOOST_FOREACH( Listener_ABC* listener, listeners_ )
                listener->MissionAcknowledged( message.message().order_ack().tasker() );
    }
}

// -----------------------------------------------------------------------------
// Name: Model::OnReceiveMessage
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Model::OnReceiveMessage( const sword::MessengerToClient& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model::OnReceiveMessage
// Created: LGY 2011-05-20
// -----------------------------------------------------------------------------
void Model::OnReceiveMessage( const sword::AuthenticationToClient& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model::Register
// Created: PHC 2011-04-08
// -----------------------------------------------------------------------------
void Model::Register( Listener_ABC& listener )
{
    listeners_.push_back( &listener );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateAgent
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Model::CreateAgent( const sword::UnitCreation& message )
{
    boost::shared_ptr< Agent > agent( new Agent( message, staticModel_.types_ ) );
    agents_[ message.unit().id() ] = agent;
    scheduler_.Schedule( agent );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateAutomat
// Created: PHC 2011-05-12
// -----------------------------------------------------------------------------
void Model::CreateAutomat( const sword::AutomatCreation& message )
{
    boost::shared_ptr< Automat > automat( new Automat( message, staticModel_.types_ ) );
    automats_[ message.automat().id() ] = automat;
    scheduler_.Schedule( automat );
}

// -----------------------------------------------------------------------------
// Name: Model::FindAgent
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Agent_ABC* Model::FindAgent( unsigned int id ) const
{
    T_Agents::const_iterator it = agents_.find( id );
    if( it != agents_.end() )
        return it->second.get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Model::GetAgent
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Agent_ABC& Model::GetAgent( unsigned int id ) const
{
    T_Agents::const_iterator it = agents_.find( id );
    if( it != agents_.end() )
        return *it->second;
    throw std::runtime_error( __FUNCTION__ ": unknown agent '" + boost::lexical_cast< std::string >( id ) + "'." );
}

// -----------------------------------------------------------------------------
// Name: Model::FindAutomat
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Automat_ABC* Model::FindAutomat( unsigned int id ) const
{
    T_Automats::const_iterator it = automats_.find( id );
    if( it != automats_.end() )
        return it->second.get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Model::GetAutomat
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Automat_ABC& Model::GetAutomat( unsigned int id ) const
{
    T_Automats::const_iterator it = automats_.find( id );
    if( it != automats_.end() )
        return *it->second;
    throw std::runtime_error( __FUNCTION__ ": unknown automat '" + boost::lexical_cast< std::string >( id ) + "'." );
}

// -----------------------------------------------------------------------------
// Name: Model::FindKnowledgeGroup
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* Model::FindKnowledgeGroup( unsigned int /*id*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Model::GetKnowledgeGroup
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC& Model::GetKnowledgeGroup( unsigned int /*id*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Model::FindObject
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Object_ABC* Model::FindObject( unsigned int /*id*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Model::GetObject
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Object_ABC& Model::GetObject( unsigned int /*id*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Model::FindUrbanObject
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Object_ABC* Model::FindUrbanObject( unsigned int /*id*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Model::GetUrbanObject
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Object_ABC& Model::GetUrbanObject( unsigned int /*id*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Model::FindPopulation
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Population_ABC* Model::FindPopulation( unsigned int /*id*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Model::GetPopulation
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Population_ABC& Model::GetPopulation( unsigned int /*id*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Model::FindFormation
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Formation_ABC* Model::FindFormation( unsigned int /*id*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Model::GetFormation
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Formation_ABC& Model::GetFormation( unsigned int /*id*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Model::FindTeam
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Team_ABC* Model::FindTeam( unsigned int /*id*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Model::GetTeam
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
kernel::Team_ABC& Model::GetTeam( unsigned int /*id*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not implemented" );
}
