// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Location.h"
#include "ParameterVisitor_ABC.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Location constructor
// Created: ABR 2011-11-17
// -----------------------------------------------------------------------------
Location::Location( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter )
    : Parameter< QString >( parameter )
    , LocationBase( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Location constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
Location::Location( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : Parameter< QString >( parameter )
    , LocationBase( converter, location )
{
    Set( IsValid() );
}

// -----------------------------------------------------------------------------
// Name: Location constructor
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
Location::Location( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const sword::Location& message )
    : Parameter< QString >( parameter )
    , LocationBase( converter, message )
{
    Set( IsValid() );
}

// -----------------------------------------------------------------------------
// Name: Location destructor
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
Location::~Location()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Location::Draw
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
void Location::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    LocationBase::Draw( where, viewport, tools );
    Parameter< QString >::Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Location::DisplayInToolTip
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void Location::DisplayInToolTip( Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetName() );
}

// -----------------------------------------------------------------------------
// Name: Location::GetPosition
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
geometry::Point2f Location::GetPosition() const
{
    return LocationBase::GetPosition();
}

// -----------------------------------------------------------------------------
// Name: Location::Serialize
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Location::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    if( IsSet() )
        LocationBase::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: Location::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Location::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value ( !IsSet() );
    if( IsSet() )
        LocationBase::CommitTo( *message.add_value()->mutable_location() );
}
// -----------------------------------------------------------------------------
// Name: Location::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Location::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        LocationBase::CommitTo( *message.mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: Location::CommitTo
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void Location::CommitTo( std::string& content ) const
{
    LocationBase::CommitTo( content );
}

// -----------------------------------------------------------------------------
// Name: Location::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Location::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Location::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool Location::IsSet() const
{
    return IsValid();
}

// -----------------------------------------------------------------------------
// Name: Location::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Location::SerializeType() const
{
    return "location";
}
