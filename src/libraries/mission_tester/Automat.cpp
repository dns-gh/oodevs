// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Automat.h"
#include "Exercise.h"
#include "Filter_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "protocol/Protocol.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/LogisticLevel.h"

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
            , count_   ( Count( resolver_ ) )
            , current_ ( resolver_.CreateIterator() )
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
// Name: Automat constructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Automat::Automat( const sword::AutomatCreation& message, const tools::Resolver_ABC< kernel::AutomatType >& resolver )
    : id_           ( message.automat().id() )
    , name_         ( message.name().c_str() )
    , type_         ( resolver.Get( message.type().id() ) )
    , logisticLevel_( kernel::LogisticLevel::Resolve( message.logistic_level() ) )
    , current_      ( new InfiniteIterator< kernel::Mission >( GetType().GetDecisionalModel() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat::GetName
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
QString Automat::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Automat::GetId
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
unsigned long Automat::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Automat::GetType
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
const kernel::AutomatType& Automat::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Automat::Select
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Automat::Select( kernel::ActionController& /*controller*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not to be called" );
}

// -----------------------------------------------------------------------------
// Name: Automat::ContextMenu
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Automat::ContextMenu( kernel::ActionController& /*controller*/, const QPoint& /*where*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not to be called" );
}

// -----------------------------------------------------------------------------
// Name: Automat::Activate
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Automat::Activate( kernel::ActionController& /*controller*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not to be called" );
}

// -----------------------------------------------------------------------------
// Name: Automat::Matches
// Created: PHC 2011-03-31
// -----------------------------------------------------------------------------
bool Automat::Matches( const Filter_ABC& filter ) const
{
    return filter.Accepts( "automat" );
}

// -----------------------------------------------------------------------------
// Name: Automat::Trigger
// Created: PHC 2011-04-04
// -----------------------------------------------------------------------------
bool Automat::Trigger( State_ABC& /*state*/ )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Automat::DoSomething
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
bool Automat::StartMission( Exercise& exercise )
{
    if( current_->HasMoreElements() )
    {
        const kernel::Mission& mission( current_->NextElement() );
        const kernel::MissionType& missionType( mission.GetType() );
        return exercise.CreateAction( *this, missionType );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Automat::GetLogisticLevel
// Created: PHC 2011-05-12
// -----------------------------------------------------------------------------
const kernel::LogisticLevel& Automat::GetLogisticLevel( void ) const
{
    return logisticLevel_;
}
