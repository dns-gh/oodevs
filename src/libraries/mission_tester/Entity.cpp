// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Entity.h"
#include "Exercise.h"
#include "Filter_ABC.h"
#include "clients_kernel/AgentType.h"
#include "protocol/Protocol.h"
#include "clients_kernel/FragOrder.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/MissionType.h"
#include <stdlib.h>

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
// Name: Entity constructor
// Created: PHC 2011-05-17
// -----------------------------------------------------------------------------
Entity::Entity( const unsigned long id, const QString& name, const kernel::DecisionalModel& decisionalModel )
    : id_              ( id )
    , name_            ( name )
    , currentFragOrder_( new InfiniteIterator< kernel::FragOrder >( decisionalModel ) )
    , currentMission_  ( new InfiniteIterator< kernel::Mission >( decisionalModel ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Entity destructor
// Created: PHC 2011-05-17
// -----------------------------------------------------------------------------
Entity::~Entity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent::GetName
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
QString Entity::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetId
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
unsigned long Entity::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Agent::Select
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Entity::Select( kernel::ActionController& /*controller*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not to be called" );
}

// -----------------------------------------------------------------------------
// Name: Agent::ContextMenu
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Entity::ContextMenu( kernel::ActionController& /*controller*/, const QPoint& /*where*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not to be called" );
}

// -----------------------------------------------------------------------------
// Name: Agent::Activate
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Entity::Activate( kernel::ActionController& /*controller*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not to be called" );
}

// -----------------------------------------------------------------------------
// Name: Agent::Start
// Created: PHC 2011-05-17
// -----------------------------------------------------------------------------
bool Entity::Start( Exercise& exercise, bool withFragOrders )
{
    if( withFragOrders && ( rand() % 10 ) > 5 )
        return StartFragOrder( exercise );
    return StartMission( exercise );
}

// -----------------------------------------------------------------------------
// Name: Agent::StartMission
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
bool Entity::StartMission( Exercise& exercise )
{
    if( currentMission_->HasMoreElements() )
    {
        const kernel::Mission& mission( currentMission_->NextElement() );
        const kernel::MissionType& missionType( mission.GetType() );
        return exercise.CreateMission( *this, missionType );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Agent::StartFragOrder
// Created: PHC 2011-05-17
// -----------------------------------------------------------------------------
bool Entity::StartFragOrder( Exercise& exercise )
{
    if( currentFragOrder_->HasMoreElements() )
    {
        const kernel::FragOrder& fragOrder( currentFragOrder_->NextElement() );
        const kernel::FragOrderType& fragOrderType( fragOrder.GetType() );
        return exercise.CreateFragOrder( *this, fragOrderType );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Entity::Matches
// Created: PHC 2011-05-17
// -----------------------------------------------------------------------------
bool Entity::Matches( const Filter_ABC& /*filter*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " should be reimplemented in child class." );
}

// -----------------------------------------------------------------------------
// Name: Entity::GetTypeName
// Created: PHC 2011-05-17
// -----------------------------------------------------------------------------
QString Entity::GetTypeName() const
{
    return kernel::Entity_ABC::GetTypeName();
}
