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
#include "protocol/Simulation.h"
#include <xeumeuleu/xml.hpp>

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
// Name: ReportTemplate::RenderMessage
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
QString ReportTemplate::RenderMessage( const kernel::Entity_ABC* entity, const sword::Report& message ) const
{
    QString messageStr = message_;
    unsigned int enums = 0;
    if( message.has_parameters() )
    {
        const sword::MissionParameters& parameters = message.parameters();
        for( int i = 0; i < parameters.elem_size(); ++i )
        {
            const sword::MissionParameter& parameter = parameters.elem( i );
            if( !parameter.null_value() && parameter.value_size() == 1 )//report doesn't manage list
            {
                const sword::MissionParameter_Value& param = *parameter.value().begin();
                if( param.has_enumeration() )
                    messageStr = messageStr.arg( enumerations_[ enums++ ][ param.enumeration() ] );
                else
                    messageStr = messageStr.arg( factory_.RenderParameter( entity, param ) );
            }
        }
    }
    return messageStr;
}

// -----------------------------------------------------------------------------
// Name: ReportTemplate::ReadParameter
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
void ReportTemplate::ReadParameter( xml::xistream& xis )
{
    std::string type;
    xis >> xml::attribute( "type", type );
    if( QString::fromStdString( type ).toLower() == "enumeration" )
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
