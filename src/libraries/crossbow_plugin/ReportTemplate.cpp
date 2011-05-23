// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "protocol/Protocol.h"
#include "ReportTemplate.h"
#include "ReportFactory.h"
#include <xeumeuleu/xml.hpp>

using namespace sword;

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: ReportTemplate constructor
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
ReportTemplate::ReportTemplate( xml::xistream& xis, const ReportFactory& factory )
    : factory_( factory )
{
    std::string message;
    xis >> xml::attribute( "id", (int&)id_ )
        >> xml::attribute( "message", message )
        >> xml::list( "parameter", *this, &ReportTemplate::ReadParameter );
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
// Name: ReportTemplate::Message
// Created: JCR 2010-11-19
// -----------------------------------------------------------------------------
const std::string& ReportTemplate::GetMessage() const
{
    return message_;
}

namespace
{
    void SetParameter( std::string& message, unsigned int index, const std::string& value )
    {
        std::stringstream pattern;
        pattern << "%" << index;
        std::size_t pos = message.find_first_of( pattern.str() );
        if( pos == std::string::npos )
            return;
        message.erase( pos, pattern.str().length() );
        message.insert( pos, value );
    }
}

// -----------------------------------------------------------------------------
// Name: ReportTemplate::RenderMessage
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
std::string ReportTemplate::RenderMessage( const MissionParameters& message ) const
{
    std::string content( GetMessage() );
    unsigned int enums = 0;
    for( int i = 0; i < message.elem_size(); ++i )
        if( message.elem(i).value().Get( 0 ).has_enumeration() )
            SetParameter( content, i + 1, enumerations_[enums++][message.elem(i).value().Get( 0 ).enumeration()] );
        else
            SetParameter( content, i + 1, factory_.RenderParameter( message.elem(i) ) );
    return content;
}

// -----------------------------------------------------------------------------
// Name: ReportTemplate::ReadParameter
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
void ReportTemplate::ReadParameter( xml::xistream& xis )
{
    std::string type;
    xis >> xml::attribute( "type", type );
    std::string lstring;
    lstring.resize( type.size() );
    std::transform( type.begin(), type.end(), lstring.begin(), tolower );
    if( lstring == "enumeration" )
    {
        enumerations_.push_back( T_EnumerationValues() );
        xis >> xml::list( "value", *this, &ReportTemplate::ReadEnumeration );
    }
}

// -----------------------------------------------------------------------------
// Name: ReportTemplate::ReadEnumeration
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
void ReportTemplate::ReadEnumeration( xml::xistream& xis )
{
    std::string name;
    xis >> xml::attribute( "name", name );
    enumerations_.back().push_back( name.c_str() );
}
