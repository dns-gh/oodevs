// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLima.h"
#include "ActionParameterLocation.h"
#include "ActionParameterVisitor_ABC.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterLima constructor
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
ActionParameterLima::ActionParameterLima( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : ActionParameter< QString >( parameter )
    , location_( new ActionParameterLocation( OrderParameter( tools::translate( "ActionParameter", "Location" ), "location", false ), converter, location ) )
{
    AddParameter( *location_ );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterLima::ActionParameterLima( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_LimaOrder& asn )
    : ActionParameter< QString >( parameter )
    , location_( new ActionParameterLocation( OrderParameter( tools::translate( "ActionParameter", "Location" ), "location", false ), converter, asn.lima ) )
{
    QStringList functions;
    for( unsigned int i = 0; i < asn.fonctions.n; ++i )
        functions.append( tools::ToString( (E_FuncLimaType)asn.fonctions.elem[i] ) );
    SetValue( functions.join( ", " ) );
    AddParameter( *location_ );
}

namespace
{
    QString ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        return name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterLima::ActionParameterLima( const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( OrderParameter( ReadName( xis ), "lima", false ) )
{
    std::string value;
    xis >> attribute( "value", value )
        >> start( "parameter" );
    location_ = new ActionParameterLocation( converter, xis );
    AddParameter( *location_ );
    xis >> end();
    SetValue( value.c_str() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima destructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterLima::~ActionParameterLima()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima::IsContext
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
bool ActionParameterLima::IsContext() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima::AddFunction
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void ActionParameterLima::AddFunction( unsigned int i )
{
    QStringList functions = QStringList::split( ", ", GetValue() );
    functions.append( tools::ToString( (E_FuncLimaType)i ) );
    SetValue( functions.join( ", " ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima::Serialize
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ActionParameterLima::Serialize( xml::xostream& xos ) const
{
    ActionParameter< QString >::Serialize( xos );
    xos << attribute( "type", "lima" )
        << attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima::DisplayInToolTip
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
void ActionParameterLima::DisplayInToolTip( Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLima::CommitTo( ASN1T_LimaOrder& asn ) const
{
    QStringList functions = QStringList::split( ", ", GetValue() );
    asn.fonctions.n = functions.count();
    asn.fonctions.elem = new ASN1T_EnumLimaType[asn.fonctions.n];
    for( unsigned int i = 0; i < asn.fonctions.n; ++i )
        asn.fonctions.elem[i] = ASN1T_EnumLimaType( tools::LimaTypeFromString( functions[i] ) );
    location_->CommitTo( asn.lima );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLima::Clean( ASN1T_LimaOrder& asn ) const
{
    delete[] asn.fonctions.elem;
    location_->Clean( asn.lima );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLima::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
