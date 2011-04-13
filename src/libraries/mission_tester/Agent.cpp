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
#include "Exercise.h"
#include "Filter_ABC.h"
#include "clients_kernel/AgentType.h"
#include "protocol/Protocol.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/MissionType.h"

using namespace mission_tester;

namespace
{
    template< typename T >
    unsigned long Count( const tools::Resolver_ABC< T >& resolver )
    {
        unsigned long result = 0;
        for( tools::Iterator< const T& > it( resolver.CreateIterator() ); it.HasMoreElements(); it.NextElement(), ++result );
        return result;
    }
    template< typename T >
    class InfiniteIterator : private boost::noncopyable
                           , public tools::Iterator_ABC< const T& >
    {
    public:
        explicit InfiniteIterator( const tools::Resolver_ABC< T >& resolver )
            : resolver_( resolver )
            , count_( Count( resolver_ ) )
            , current_( resolver_.CreateIterator() )
        {}
        virtual ~InfiniteIterator() {}
        virtual bool HasMoreElements() const
        {
            return count_ > 0;
        }
        virtual const T& NextElement()
        {
            if( !current_.HasMoreElements() )
                current_ = resolver_.CreateIterator();
            if( current_.HasMoreElements() )
                return current_.NextElement();
            throw std::runtime_error( __FUNCTION__ ": trying to access empty resolver!" );
        }

    private:
        const tools::Resolver_ABC< T >& resolver_;
        const unsigned long count_;
        tools::Iterator< const T& > current_;
    };
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Agent::Agent( const sword::UnitCreation& message, const tools::Resolver_ABC< kernel::AgentType >& resolver )
    : id_         ( message.unit().id() )
    , name_       ( message.name().c_str() )
    , type_       ( resolver.Get( message.type().id() ) )
    , commandPost_( message.pc() )
    , current_    ( new InfiniteIterator< kernel::Mission >( GetType().GetDecisionalModel() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent::GetName
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
QString Agent::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetId
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
unsigned long Agent::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetType
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
const kernel::AgentType& Agent::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Agent::IsCommandPost
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
bool Agent::IsCommandPost() const
{
    return commandPost_;
}

// -----------------------------------------------------------------------------
// Name: Agent::Select
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Agent::Select( kernel::ActionController& /*controller*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not to be called" );
}

// -----------------------------------------------------------------------------
// Name: Agent::ContextMenu
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Agent::ContextMenu( kernel::ActionController& /*controller*/, const QPoint& /*where*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not to be called" );
}

// -----------------------------------------------------------------------------
// Name: Agent::Activate
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Agent::Activate( kernel::ActionController& /*controller*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not to be called" );
}

// -----------------------------------------------------------------------------
// Name: Agent::Matches
// Created: PHC 2011-03-31
// -----------------------------------------------------------------------------
bool Agent::Matches( const Filter_ABC& filter ) const
{
    return filter.Accepts( "agent" );
}

// -----------------------------------------------------------------------------
// Name: Agent::Trigger
// Created: PHC 2011-04-04
// -----------------------------------------------------------------------------
bool Agent::Trigger( State_ABC& /*state*/ )
{
    return true; // $$$$ PHC 2011-04-04: TODO
}

// -----------------------------------------------------------------------------
// Name: Agent::DoSomething
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
bool Agent::StartMission( Exercise& exercise )
{
    if( current_->HasMoreElements() )
    {
        const kernel::Mission& mission( current_->NextElement() );
        const kernel::MissionType& missionType( mission.GetType() );
        return exercise.CreateAction( *this, missionType );
    }
    return false;
}
