// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "UnitMagicAction.h"
#include "protocol/simulationsenders.h"
#include "protocol/publisher_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ModelVisitor_ABC.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::MagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
UnitMagicAction::UnitMagicAction( const kernel::Entity_ABC& entity, const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered /*= true*/ )
    : ActionWithTarget_ABC ( controller, magic, entity )
    , controller_( controller )
    , registered_( registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::MagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
UnitMagicAction::UnitMagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic, const kernel::Entity_ABC& entity )
    : ActionWithTarget_ABC( xis, controller, magic, entity )
    , controller_         ( controller )
    , registered_         ( true )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: UnitMagicAction::~MagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
UnitMagicAction::~UnitMagicAction()
{
    if( registered_ )
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::Polish
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void UnitMagicAction::Polish()
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::Accept
// Created: JSR 2010-04-06
// -----------------------------------------------------------------------------
void UnitMagicAction::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::Serialize
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void UnitMagicAction::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", GetType().GetName() );
    xos << xml::attribute( "type", "magicunit" );
    ActionWithTarget_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::Publish
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
void UnitMagicAction::Publish( Publisher_ABC& publisher ) const
{
    MsgsClientToSim::MsgUnitMagicAction_Type type = 
        ( MsgsClientToSim::MsgUnitMagicAction_Type ) GetType().GetId();
    simulation::UnitMagicAction message;
    message().set_oid( GetEntity().GetId() );
    message().set_type( type );
    CommitTo( *message().mutable_parametres() );
    message.Send( publisher );
    // $$$$ JSR 2010-04-13: TODO Rajouter dans le type un indicateur pour l'update pour rendre la ligne ci-dessous g�n�rique
    if( type == MsgsClientToSim::MsgUnitMagicAction_Type_move_to )
        const_cast< kernel::Entity_ABC& >( GetEntity() ).Update( message() );
    message().Clear();
}

