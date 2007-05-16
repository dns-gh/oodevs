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
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterLima constructor
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
ActionParameterLima::ActionParameterLima( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : ActionParameter< QString >( name )
{
    AddParameter( *new ActionParameterLocation( tools::translate( "ActionParameter", "Location" ), converter, location ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterLima::ActionParameterLima( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_LimaOrder& asn )
    : ActionParameter< QString >( name )
{
    QStringList functions;
    for( unsigned int i = 0; i < asn.fonctions.n; ++i )
        functions.append( tools::ToString( (E_FuncLimaType)asn.fonctions.elem[i] ) );
    SetValue( functions.join( ", " ) );
    AddParameter( *new ActionParameterLocation( tools::translate( "ActionParameter", "Location" ), converter, asn.lima ) );
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
    xos << attribute( "type", "lima" );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLima::DisplayInToolTip
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
void ActionParameterLima::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    ActionParameter< QString >::DisplayInToolTip( displayer );
    displayer.Display( "", GetValue() );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayInToolTip( displayer );
}
