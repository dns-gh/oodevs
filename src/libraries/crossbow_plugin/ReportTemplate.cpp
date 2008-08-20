// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ReportTemplate.h"
#include "ReportFactory.h"
#include "game_asn/Simulation.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace crossbow;

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

namespace
{
    void SetParameter( std::string& message, unsigned int index, const std::string& value )
    {
        std::stringstream pattern;
        pattern << "%" << index;
        unsigned int pos = message.find_first_of( pattern.str() );
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
std::string ReportTemplate::RenderMessage( const ASN1T_MissionParameters& asn ) const
{
    std::string message = message_;
    unsigned int enums = 0;
    for( unsigned int i = 0; i < asn.n; ++i )
        if( asn.elem[i].value.t == T_MissionParameter_value_enumeration )
            SetParameter( message, i + 1, enumerations_[enums++][asn.elem[i].value.u.enumeration] );
        else
            SetParameter( message, i + 1, factory_.RenderParameter( asn.elem[i] ) );
    return message;
}

// -----------------------------------------------------------------------------
// Name: ReportTemplate::ReadParameter
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
void ReportTemplate::ReadParameter( xml::xistream& xis )
{
    std::string type;
    xis >> attribute( "type", type );
    std::string lstring;
    lstring.resize( type.size() );
    std::transform( type.begin(), type.end(), lstring.begin(), tolower );
    if( lstring == "enumeration" )
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
