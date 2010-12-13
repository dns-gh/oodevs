// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Location.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Location constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
Location::Location( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : Parameter< QString >( parameter )
    , LocationBase( converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Location constructor
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
Location::Location( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const sword::MsgLocation& message )
    : Parameter< QString >( parameter )
    , LocationBase( converter, message )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Location constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
Location::Location( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Parameter< QString >( parameter )
    , LocationBase( converter, xis )
{
    // NOTHING
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
void Location::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
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
    LocationBase::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: Location::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Location::CommitTo( sword::MsgMissionParameter& message ) const
{
    message.set_null_value ( !IsSet() );
    if( IsSet() )
        LocationBase::CommitTo( *message.mutable_value()->Add()->mutable_location() );
}
// -----------------------------------------------------------------------------
// Name: Location::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Location::CommitTo( sword::MsgMissionParameter_Value& message ) const
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
