// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Action_ABC.h"
#include "ActionParameter_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Entity_ABC.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;

unsigned long Action_ABC::idManager_ = 0; // $$$$ SBO 2007-03-12: real id manager maybe

// -----------------------------------------------------------------------------
// Name: Action_ABC constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Action_ABC::Action_ABC( kernel::Controller& controller, const kernel::OrderType& type, const kernel::Entity_ABC& target )
    : controller_( controller )
    , id_( ++idManager_ )
    , target_( target )
    , type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Action_ABC destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Action_ABC::~Action_ABC()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetId
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
unsigned long Action_ABC::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetName
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
QString Action_ABC::GetName() const
{
    return type_.GetName();
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetType
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
const kernel::OrderType& Action_ABC::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetEntity
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& Action_ABC::GetEntity() const
{
    return target_;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::AddParameter
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void Action_ABC::AddParameter( ActionParameter_ABC& parameter )
{
    Register( parameter.GetId(), parameter );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Draw
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
void Action_ABC::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Display
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
void Action_ABC::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "Action", "Action" ) ).Display( GetName() )
             .Item( tools::translate( "Action", "Value" ) ).Display( "" );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void Action_ABC::Serialize( xml::xostream& xos ) const
{
    xos << attribute( "id", type_.GetId() )
        << attribute( "name", type_.GetName() ) // $$$$ SBO 2007-04-24: not required
        << attribute( "target", target_.GetId() );
    xos << start( "parameters" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->SerializeParameter( xos );
    xos << end();
    xos << start( "context" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->SerializeContext( xos );
    xos << end();
}
