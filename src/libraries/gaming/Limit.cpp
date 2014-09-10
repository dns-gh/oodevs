// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Limit.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "protocol/MessengerSenders.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
Limit::Limit( kernel::Controller& controller,
              Publisher_ABC& publisher,
              const kernel::CoordinateConverter_ABC& converter,
              const T_CanBeRenamedFunctor& canBeRenamedFunctor )
    : TacticalLine_ABC( controller, tools::translate( "Limit", "Limit" ), 0, publisher, converter, canBeRenamedFunctor )
    , controller_( controller )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
Limit::Limit( kernel::Controller& controller,
              Publisher_ABC& publisher,
              const kernel::CoordinateConverter_ABC& converter,
              const sword::LimitCreation& message,
              const T_CanBeRenamedFunctor& canBeRenamedFunctor )
    : TacticalLine_ABC( controller, QString( message.tactical_line().name().c_str() ), message.id().id(), publisher, converter, canBeRenamedFunctor )
    , controller_( controller )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Limit destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
Limit::~Limit()
{
    controller_.Delete( *(kernel::TacticalLine_ABC*)this );
    controller_.Delete( *(kernel::Entity_ABC*)this );
}

//-----------------------------------------------------------------------------
// Name: Limit::UpdateToSim
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void Limit::UpdateToSim( E_State state )
{
    switch( state )
    {
    case eStateCreated:
        {
            plugins::messenger::LimitCreationRequest message;
            message().mutable_tactical_line()->set_name( GetName() );
            WriteGeometry ( *message().mutable_tactical_line()->mutable_geometry() );
            WriteDiffusion( *message().mutable_tactical_line()->mutable_diffusion() );
            Send( message );
        }
        break;
    case eStateModified:
        {
            plugins::messenger::LimitUpdateRequest message;
            message().mutable_id()->set_id( GetId() );
            message().mutable_tactical_line()->set_name( GetName() );
            WriteGeometry ( *message().mutable_tactical_line()->mutable_geometry() );
            WriteDiffusion( *message().mutable_tactical_line()->mutable_diffusion() );
            Send( message );
        }
        break;
    case eStateDeleted:
        plugins::messenger::LimitDestructionRequest message;
        message().mutable_id()->set_id( GetId() );
        Send( message );
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: Limit::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void Limit::MultipleSelect( kernel::ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const Limit* > first;
    std::vector< const kernel::TacticalLine_ABC* > second;
    std::vector< const Entity_ABC* > third;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const Limit* >( elements[ i ] ) );
        second.push_back( static_cast< const kernel::TacticalLine_ABC* >( elements[ i ] ) );
        third.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( third, second, first );
}

// -----------------------------------------------------------------------------
// Name: Limit::IsLimit
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
bool Limit::IsLimit() const
{
    return true;
}
