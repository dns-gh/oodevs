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
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
Lima::Lima( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : Parameter< QString >( parameter )
{
    AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location" ).ascii(), "location", false ), converter, location ) );
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
Lima::Lima( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_LimaOrder& asn )
    : Parameter< QString >( parameter )
{
    QStringList functions;
    for( unsigned int i = 0; i < asn.fonctions.n; ++i )
        functions.append( tools::ToShortString( (E_FuncLimaType)asn.fonctions.elem[i] ) );
    SetValue( functions.join( ", " ) );
    AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location" ).ascii(), "location", false ), converter, asn.lima ) );
    AddParameter( *new DateTime( OrderParameter( tools::translate( "Parameter", "Schedule" ).ascii(), "datetime", false ), asn.horaire ) );
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
Lima::Lima( const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Parameter< QString >( OrderParameter( attribute< std::string >( xis, "name" ), "lima", false ) )
{
    std::string value;
    xis >> attribute( "value", value )
        >> list( "parameter", *this, &Lima::ReadParameter, converter );
    QStringList functions = QStringList::split( ", ", value.c_str() );
    for( unsigned int i = 0; i < functions.size(); ++i )
        functions[i] = tools::ToShortString( tools::LimaTypeFromXmlString( functions[i] ) );
    SetValue( functions.join( ", " ) );
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
    QStringList functions = QStringList::split( ", ", GetValue() );
    for( unsigned int i = 0; i < functions.size(); ++i )
        functions[i] = tools::LimaTypeShortToXmlString( functions[i] );
    Parameter< QString >::Serialize( xos );
    xos << attribute( "type", "lima" )
        << attribute( "value", functions.join( ", " ) );
}

// -----------------------------------------------------------------------------
// Name: Lima::ReadParameter
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void Lima::ReadParameter( xml::xistream& xis, const CoordinateConverter_ABC& converter )
{
    std::string type;
    xis >> attribute( "type", type );
    if( type == "location" )
        AddParameter( *new Location( OrderParameter( tools::translate( "Parameter", "Location" ).ascii(), "location", false ), converter, xis ) );
    else if( type == "datetime" )
        AddParameter( *new DateTime( OrderParameter( tools::translate( "Parameter", "Schedule" ).ascii(), "datetime", false ), xis ) );
}

// -----------------------------------------------------------------------------
// Name: Lima::DisplayTooltip
// Created: SBO 2008-04-16
// -----------------------------------------------------------------------------
void Lima::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
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
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->GetType() != "location" )
            it->second->DisplayInToolTip( displayer );
}

// -----------------------------------------------------------------------------
// Name: Lima::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Lima::CommitTo( ASN1T_LimaOrder& asn ) const
{
    QStringList functions = QStringList::split( ", ", GetValue() );
    asn.fonctions.n = functions.count();
    asn.fonctions.elem = new ASN1T_EnumLimaType[asn.fonctions.n];
    for( unsigned int i = 0; i < asn.fonctions.n; ++i )
        asn.fonctions.elem[i] = ASN1T_EnumLimaType( tools::LimaTypeFromShortString( functions[i] ) );

    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "location" )
            static_cast< const Location* >( it->second )->CommitTo( asn.lima );
        else if( type == "datetime" )
            static_cast< const DateTime* >( it->second )->CommitTo( asn.horaire );
    }
}

// -----------------------------------------------------------------------------
// Name: Lima::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Lima::Clean( ASN1T_LimaOrder& asn ) const
{
    delete[] asn.fonctions.elem;
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const std::string type = it->second->GetType();
        if( type == "location" )
            static_cast< const Location* >( it->second )->Clean( asn.lima );
    }
}

// -----------------------------------------------------------------------------
// Name: Lima::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Lima::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
