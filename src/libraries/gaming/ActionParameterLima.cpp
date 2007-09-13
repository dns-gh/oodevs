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
#include "ActionParameterDateTime.h"
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
{
    AddParameter( *new ActionParameterLocation( OrderParameter( tools::translate( "ActionParameter", "Location" ), "location", false ), converter, location ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterLima::ActionParameterLima( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Simulation& simulation, const ASN1T_LimaOrder& asn )
    : ActionParameter< QString >( parameter )
{
    QStringList functions;
    for( unsigned int i = 0; i < asn.fonctions.n; ++i )
        functions.append( tools::ToString( (E_FuncLimaType)asn.fonctions.elem[i] ) );
    SetValue( functions.join( ", " ) );
    AddParameter( *new ActionParameterLocation( OrderParameter( tools::translate( "ActionParameter", "Location" ), "location", false ), converter, asn.lima ) );
    AddParameter( *new ActionParameterDateTime( OrderParameter( tools::translate( "ActionParameter", "Schedule" ), "datetime", false ), simulation, asn.horaire ) );
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
ActionParameterLima::ActionParameterLima( const CoordinateConverter_ABC& converter, const Simulation& simulation, xml::xistream& xis )
    : ActionParameter< QString >( OrderParameter( ReadName( xis ), "lima", false ) )
{
    std::string value;
    xis >> attribute( "value", value )
        >> list( "parameter", *this, &ActionParameterLima::ReadParameter, converter, simulation );
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
// Name: ActionParameterLima::ReadParameter
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameterLima::ReadParameter( xml::xistream& xis, const CoordinateConverter_ABC& converter, const Simulation& simulation )
{
    std::string type;
    xis >> attribute( "type", type );
    if( type == "location" )
        AddParameter( *new ActionParameterLocation( converter, xis ) );
    else if( type == "datetime" )
        AddParameter( *new ActionParameterDateTime( OrderParameter( tools::translate( "ActionParameter", "Schedule" ), "datetime", false ), xis, simulation ) );
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

    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const QString& type = it->second->GetType();
        if( type == "location" )
            static_cast< const ActionParameterLocation* >( it->second )->CommitTo( asn.lima );
        else if( type == "datetime" )
            static_cast< const ActionParameterDateTime* >( it->second )->CommitTo( asn.horaire );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLima::Clean( ASN1T_LimaOrder& asn ) const
{
    delete[] asn.fonctions.elem;
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const QString& type = it->second->GetType();
        if( type == "location" )
            static_cast< const ActionParameterLocation* >( it->second )->Clean( asn.lima );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLima::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
