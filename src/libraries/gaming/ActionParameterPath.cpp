// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterPath.h"
#include "ActionParameterPathPoint.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Point.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const QString& name, const CoordinateConverter_ABC& converter, const Location_ABC& location, const Entity_ABC& entity )
    : ActionParameter< QString >( name )
    , converter_( converter )
{
    // $$$$ SBO 2007-05-16: entity ?
    location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const QString& name, const CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn, const Entity_ABC& entity )
    : ActionParameter< QString >( name )
    , converter_( converter )
{
    AddPoints( asn, entity );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location, const Entity_ABC& entity )
    : ActionParameter< QString >( parameter )
    , converter_( converter )
{
    // $$$$ SBO 2007-05-16: entity ?
    location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn, const Entity_ABC& entity )
    : ActionParameter< QString >( parameter )
    , converter_( converter )
{
    AddPoints( asn, entity );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
    , converter_( converter )
{
    xis >> list( "parameter", *this, &ActionParameterPath::ReadPoint );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath destructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::~ActionParameterPath()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::AddPoints
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterPath::AddPoints( const ASN1T_Localisation& asn, const Entity_ABC& entity )
{
    // $$$$ SBO 2007-05-16: entity
//    Point pt;
//    pt.AddPoint( entity.Get< Positions >().GetPosition() );
//    AddParameter( *new ActionParameterPathPoint( tools::translate( "ActionParameter", "Start" ), converter_, pt ) );
    for( unsigned int i = 0; i < asn.vecteur_point.n; ++i )
    {
        Point pt;
        pt.AddPoint( converter_.ConvertToXY( asn.vecteur_point.elem[i] ) );
        QString label;
        if( i + 1 == asn.vecteur_point.n )
            label = tools::translate( "ActionParameter", "Destination" );
        else
            label = tools::translate( "ActionParameter", "Way point %1" ).arg( i + 1 );
        AddParameter( *new ActionParameterPathPoint( label, converter_, pt ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::VisitLines
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterPath::VisitLines( const T_PointVector& points )
{
    unsigned int i = 0;
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
    {
        Point pt;
        pt.AddPoint( *it );
        QString label;
        if( ++i == points.size() )
            label = tools::translate( "ActionParameter", "Destination" );
        else
            label = tools::translate( "ActionParameter", "Way point %1" ).arg( i );
        AddParameter( *new ActionParameterPathPoint( label, converter_, pt ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::ReadPoint
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterPath::ReadPoint( xml::xistream& xis )
{
    AddParameter( *new ActionParameterPathPoint( xis, converter_ ) );
}
