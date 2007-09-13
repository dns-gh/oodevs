// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterDateTime.h"
#include "Simulation.h"
#include "clients_kernel/GlTools_ABC.h"
#include "xeumeuleu/xml.h"
#include <qregexp.h>

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime constructor
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
ActionParameterDateTime::ActionParameterDateTime( const kernel::OrderParameter& parameter, xml::xistream& xis, const Simulation& simulation )
    : ActionParameter< QString >( parameter )
    , simulation_( simulation )
{
    std::string value;
    xis >> attribute( "value", value );
    QRegExp regexp( tools::translate( "ActionParameter", "Day ([0-9]+) ([0-9:]+)" ) );
    regexp.match( value.c_str() );
    QStringList list = regexp.capturedTexts();
    if( list.size() < 3 )
        throw std::runtime_error( tools::translate( "ActionParameter", "Invalid date format" ).ascii() ); // $$$$ SBO 2007-06-25: 
    day_ = list[1].toUInt();
    time_ = QTime::fromString( list[2] );
    SetValue( tools::translate( "ActionParameter", "Day %1 %2" ).arg( day_ ).arg( time_.toString() ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime constructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
ActionParameterDateTime::ActionParameterDateTime( const OrderParameter& parameter, const Simulation& simulation, unsigned int ticks )
    : ActionParameter< QString >( parameter )
    , simulation_( simulation )
    , day_( simulation.ComputeDay( ticks ) )
    , time_( simulation.ComputeTime( ticks ) )
{
    SetValue( tools::translate( "ActionParameter", "Day %1 %2" ).arg( day_ ).arg( time_.toString() ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime destructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
ActionParameterDateTime::~ActionParameterDateTime()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime::Draw
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterDateTime::Draw( const geometry::Point2f&, const kernel::Viewport_ABC&, const kernel::GlTools_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime::DisplayInToolTip
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
void ActionParameterDateTime::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetValue() );
    ActionParameter< QString >::DisplayInToolTip( displayer );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime::Serialize
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterDateTime::Serialize( xml::xostream& xos ) const
{
    ActionParameter< QString >::Serialize( xos );
    xos << attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDateTime::CommitTo
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterDateTime::CommitTo( ASN1INT& asn ) const
{
    asn = simulation_.ComputeTick( day_, time_ );
}
