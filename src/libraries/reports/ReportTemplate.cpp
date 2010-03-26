// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ReportTemplate.h"
#include "ReportFactory.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.h>

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ReportTemplate constructor
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
ReportTemplate::ReportTemplate( xml::xistream& xis, const ReportFactory& factory )
    : factory_( factory )
{
    std::string message;
    xis >> attribute( "id", (int&)id_ )
        >> attribute( "message", message )
        >> list( "parameter", *this, &ReportTemplate::ReadParameter );
    message_ = message.c_str();
}

// -----------------------------------------------------------------------------
// Name: ReportTemplate destructor
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
ReportTemplate::~ReportTemplate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportTemplate::GetId
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
unsigned long ReportTemplate::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: ReportTemplate::RenderMessage
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
QString ReportTemplate::RenderMessage( const Common::MsgMissionParameters& message ) const
{
    QString messageStr = message_;
    unsigned int enums = 0;
    for( int i = 0; i < message.elem_size(); ++i )
        if( message.elem(i).value().has_enumeration() )
            messageStr = messageStr.arg( enumerations_[enums++][message.elem(i).value().enumeration()] );
        else
            messageStr = messageStr.arg( factory_.RenderParameter( message.elem(i) ) );
    return messageStr;
}

// -----------------------------------------------------------------------------
// Name: ReportTemplate::ReadParameter
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
void ReportTemplate::ReadParameter( xml::xistream& xis )
{
    std::string type;
    xis >> attribute( "type", type );
    if( QString( type.c_str() ).lower() == "enumeration" )
    {
        enumerations_.push_back( T_EnumerationValues() );
        xis >> list( "value", *this, &ReportTemplate::ReadEnumeration );
    }
}

// -----------------------------------------------------------------------------
// Name: ReportTemplate::ReadEnumeration
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
void ReportTemplate::ReadEnumeration( xml::xistream& xis )
{
    std::string name;
    xis >> attribute( "name", name );
    enumerations_.back().push_back( name.c_str() );
}
