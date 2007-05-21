// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterObstacle.h"
#include "clients_kernel/ObjectType.h"
#include "ActionParameterLocation.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterObstacle::ActionParameterObstacle( const QString& name, const ObjectType& type )
    : ActionParameter< QString >( name, type.GetName() )
    , type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterObstacle::ActionParameterObstacle( const OrderParameter& parameter, const ObjectType& type )
    : ActionParameter< QString >( parameter, type.GetName() )
    , type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle constructor
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
ActionParameterObstacle::ActionParameterObstacle( const QString& name, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& types, const ASN1T_MissionGenObject& asn )
    : ActionParameter< QString >( name )
    , type_( types.Get( asn.type ) )
{
    AddParameter( *new ActionParameterLocation( tools::translate( "ActionParameter", "Location" ), converter, asn.position ) );
    SetValue( type_.GetName() );
//    SetParameters( asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterObstacle::ActionParameterObstacle( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& types, const ASN1T_MissionGenObject& asn )
    : ActionParameter< QString >( parameter )
    , type_( types.Get( asn.type ) )
{
    AddParameter( *new ActionParameterLocation( tools::translate( "ActionParameter", "Location" ), converter, asn.position ) );
    SetValue( type_.GetName() );
//    SetParameters( asn );
}

namespace
{
    unsigned int ReadType( xml::xistream& xis )
    {
        unsigned int type;
        xis >> attribute( "value", type );
        return type;
    }

    QString ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> attribute( "name", name );
        return name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterObstacle::ActionParameterObstacle( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& types, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
    , type_( types.Get( ReadType( xis ) ) )
{
    xis >> start( "parameter" );
    AddParameter( *new ActionParameterLocation( converter, xis ) );
    xis >> end();
    SetValue( type_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterObstacle::ActionParameterObstacle( const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, xml::xistream& xis )
    : ActionParameter< QString >( ReadName( xis ) )
    , type_( types.Get( ReadType( xis ) ) )
{
    xis >> start( "parameter" );
    AddParameter( *new ActionParameterLocation( converter, xis ) );
    xis >> end();
    SetValue( type_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterObstacle::~ActionParameterObstacle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::Draw
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ActionParameterObstacle::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    ActionParameter< QString >::Draw( where, viewport, tools );
    type_.Draw( GetPosition(), viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterObstacle::Serialize( xml::xostream& xos ) const
{
    ActionParameter< QString >::Serialize( xos );
    xos << attribute( "value", type_.id_ ); // $$$$ SBO 2007-05-21:
    // $$$$ SBO 2007-05-21: Serialize additionnal parameters
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::SetParameters
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
void ActionParameterObstacle::SetParameters( const ASN1T_MissionGenObject& asn )
{
    // $$$$ SBO 2007-04-17: TODO
    switch( asn.type )
    {
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        break;
    case EnumObjectType::itineraire_logistique:
        break;
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        break;
    case EnumObjectType::rota:
        break;
    case EnumObjectType::site_franchissement:
    case EnumObjectType::bouchon_mines:
    case EnumObjectType::zone_minee_lineaire:
    case EnumObjectType::zone_minee_par_dispersion:
    default:
        ;
    }
}
