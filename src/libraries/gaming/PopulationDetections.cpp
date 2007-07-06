// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationDetections.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationConcentration_ABC.h"
#include "clients_kernel/PopulationFlow_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "Attr_def.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationDetections constructor
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
PopulationDetections::PopulationDetections( Controller& controller, const kernel::CoordinateConverter_ABC& converter, const Resolver_ABC< Population_ABC >& resolver, const kernel::Entity_ABC& entity )
    : controller_( controller )
    , converter_( converter )
    , resolver_( resolver )
    , entity_( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections destructor
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
PopulationDetections::~PopulationDetections()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections::DoUpdate
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationDetections::DoUpdate( const ASN1T_MsgPopulationConcentrationDetection& message )
{
    const PopulationPart_ABC* pConcentration = & resolver_.Get( message.population_oid ).GetConcentration( message.concentration_oid );
    if( message.visibility == EnumUnitVisibility::invisible )
        perceived_.erase( pConcentration );
    else
        perceived_.insert( pConcentration );
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections::DoUpdate
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationDetections::DoUpdate( const ASN1T_MsgPopulationFlowDetection& message )
{
    const PopulationPart_ABC* pFlow = & resolver_.Get( message.population_oid ).GetFlow( message.flow_oid );

    if( message.visible_flow.coordinates.n )
        perceived_.insert( pFlow );
    else
        perceived_.erase( pFlow );
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void PopulationDetections::Draw( const geometry::Point2f&, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    const geometry::Point2f& center = entity_.Get< kernel::Positions >().GetPosition();
    if( !viewport.IsVisible( center ) )
        return;
    glPushAttrib( GL_CURRENT_BIT );
    glColor4f( COLOR_DETECTED );
    for( CIT_Parts it = perceived_.begin(); it != perceived_.end(); ++it )
        tools.DrawLine( center, (*it)->GetPosition() );
    glPopAttrib();
}


