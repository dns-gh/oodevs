// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Lima.h"
#include "Location.h"
#include "DateTime.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: ABR 2011-11-17
// -----------------------------------------------------------------------------
Lima::Lima( const kernel::OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
Lima::Lima( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : Parameter< QString >( parameter )
{
    AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location" ).toStdString(), "location", false ), converter, location ) );
    Set( true );
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
Lima::Lima( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const sword::PhaseLineOrder& message )
    : Parameter< QString >( parameter )
{
    QStringList functions;
    for( int i = 0; i < message.fonctions_size(); ++i )
        functions.append( tools::ToShortString( (E_FuncLimaType)message.fonctions(i) ) );
    SetValue( functions.join( ", " ) );
    AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location" ).toStdString(), "location", false ), converter, message.line().location() ) );
    AddParameter( *new DateTime( OrderParameter( tools::translate( "Parameter", "Schedule" ).toStdString(), "datetime", true ), message.time() ) );
}

// -----------------------------------------------------------------------------
// Name: Lima destructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
Lima::~Lima()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima::AddFunction
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void Lima::AddFunction( unsigned int i )
{
    QStringList functions = QStringList::split( ", ", GetValue() );
    functions.append( tools::ToShortString( (E_FuncLimaType)i ) );
    SetValue( functions.join( ", " ) );
}

// -----------------------------------------------------------------------------
// Name: Lima::Serialize
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void Lima::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    if( IsSet() )
    {
        QStringList functions = QStringList::split( ", ", GetValue() );
        for( int i = 0; i < functions.size(); ++i )
            functions[i] = tools::LimaTypeShortToXmlString( functions[i] );
        xos << xml::attribute( "value", functions.join( ", " ).toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: Lima::DisplayTooltip
// Created: SBO 2008-04-16
// -----------------------------------------------------------------------------
void Lima::DisplayTooltip( const gui::Viewport_ABC& viewport, const gui::GlTools_ABC& tools ) const
{
    Parameter< QString >::DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Lima::DisplayInToolTip
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
void Lima::DisplayInToolTip( Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetValue() );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->GetType() != "location" )
            it->second->DisplayInToolTip( displayer );
}

// -----------------------------------------------------------------------------
// Name: Lima::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Lima::CommitTo( sword::PhaseLineOrder& message ) const
{
    QStringList functions = QStringList::split( ", ", GetValue() );
    for( int i = 0; i < functions.count(); ++i )
        message.add_fonctions( sword::PhaseLineOrder::Function( tools::LimaTypeFromShortString( functions[i] ) ) );

    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "location" )
            static_cast< const Location* >( it->second )->CommitTo( *message.mutable_line()->mutable_location() );
        else if( type == "datetime" )
            static_cast< const DateTime* >( it->second )->CommitTo( *message.mutable_time() );
    }
}

// -----------------------------------------------------------------------------
// Name: Lima::CommitTo
// Created: MGD 2010-10-27
// -----------------------------------------------------------------------------
void Lima::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        CommitTo( *message.add_value()->mutable_phaseline()->add_elem() );
}
// -----------------------------------------------------------------------------
// Name: Lima::CommitTo
// Created: MGD 2010-10-27
// -----------------------------------------------------------------------------
void Lima::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        CommitTo( *message.mutable_phaseline()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: Lima::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Lima::Clean( sword::PhaseLineOrder& message ) const
{
    message.Clear();
}

// -----------------------------------------------------------------------------
// Name: Lima::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Lima::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second != 0 )
            it->second->Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: Lima::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Lima::SerializeType() const
{
    return "phaseline";
}
