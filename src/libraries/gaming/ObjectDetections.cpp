// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectDetections.h"
#include "Attr_Def.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Positions.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectDetections constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectDetections::ObjectDetections( Controller& controller, const tools::Resolver_ABC< Object_ABC >& resolver )
    : controller_( controller )
    , resolver_  ( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectDetections destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectDetections::~ObjectDetections()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectDetections::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectDetections::DoUpdate( const sword::ObjectDetection& message )
{
    if( Object_ABC* object = resolver_.Find( message.observer().id() ) )
    {
        if( message.visibility() == sword::UnitVisibility::invisible )
            perceivedObjects_.erase( object );
        else
            perceivedObjects_.insert( object );
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectDetections::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void ObjectDetections::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& , const gui::GlTools_ABC& tools ) const
{
    if( ! tools.ShouldDisplay( "VisionLines" ) || perceivedObjects_.empty() )
        return;
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glColor4f( COLOR_DETECTED );
    glLineWidth( 1.f );
    for( auto it = perceivedObjects_.begin(); it != perceivedObjects_.end(); ++it )
        tools.DrawLine( where, (*it)->Get< Positions >().GetPosition() );
    glPopAttrib();
}
