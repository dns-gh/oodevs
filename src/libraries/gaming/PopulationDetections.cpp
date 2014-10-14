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
#include "Attr_def.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationConcentration_ABC.h"
#include "clients_kernel/PopulationFlow_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationDetections constructor
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
PopulationDetections::PopulationDetections( Controller& controller, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< Population_ABC >& resolver, const Entity_ABC& entity )
    : controller_( controller )
    , converter_ ( converter )
    , resolver_  ( resolver )
    , entity_    ( entity )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections destructor
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
PopulationDetections::~PopulationDetections()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections::DoUpdate
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationDetections::DoUpdate( const sword::CrowdConcentrationDetection& message )
{
    const PopulationPart_ABC* pConcentration = & resolver_.Get( message.detected_crowd().id() ).GetConcentration( message.detected_concentration().id() );
    if( message.visibility() == sword::UnitVisibility::invisible )
        perceived_.erase( pConcentration );
    else
        perceived_.insert( pConcentration );
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections::DoUpdate
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationDetections::DoUpdate( const sword::CrowdFlowDetection& message )
{
    const PopulationPart_ABC* pFlow = & resolver_.Get( message.detected_crowd().id() ).GetFlow( message.detected_flow().id() );
    if( message.visible_flow().location().coordinates().elem_size() )
        perceived_.insert( pFlow );
    else
        perceived_.erase( pFlow );
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections::NotifyDeleted
// Created: JSR 2013-07-31
// -----------------------------------------------------------------------------
void PopulationDetections::NotifyDeleted( const kernel::PopulationPart_ABC& part )
{
    auto it = perceived_.find( &part );
    if( it != perceived_.end() )
        perceived_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void PopulationDetections::Draw( const geometry::Point2f&, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( tools.ShouldDisplay( "VisionLines" ) )
    {
        const geometry::Point2f& center = entity_.Get< Positions >().GetPosition();
        if( !viewport.IsVisible( center ) )
            return;
        glPushAttrib( GL_CURRENT_BIT );
        static const float color[4] = { COLOR_DETECTED };
        glColor4f( color[0], color[1], color[2], tools.GetCurrentAlpha() );
        for( auto it = perceived_.begin(); it != perceived_.end(); ++it )
            tools.DrawLine( center, ( *it )->Get< Positions >().GetPosition() );
        glPopAttrib();
    }
}
