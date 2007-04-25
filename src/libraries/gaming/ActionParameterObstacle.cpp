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

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterObstacle::ActionParameterObstacle( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, const ASN1T_MissionGenObject& asn )
    : ActionParameter< QString >( parameter )
    , LocationPositions( converter )
    , type_( types.Get( asn.type ) )
{
    Update( asn.position );
    SetValue( type_.GetName() );
//    SetParameters( asn );
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
void ActionParameterObstacle::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    LocationPositions::Draw( where, viewport, tools );
    type_.Draw( GetPosition(), viewport, tools );
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
