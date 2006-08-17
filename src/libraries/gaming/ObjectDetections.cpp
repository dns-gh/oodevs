// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectDetections.h"
#include "clients_kernel/Controller.h"
#include "Attr_Def.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Positions.h"

// -----------------------------------------------------------------------------
// Name: ObjectDetections constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectDetections::ObjectDetections( Controller& controller, const Resolver_ABC< Object_ABC >& resolver )
    : controller_( controller )
    , resolver_( resolver )
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
void ObjectDetections::DoUpdate( const ObjectDetectionMessage& message )
{
    unsigned long  id;
    unsigned char  nObjectVisType;

    message >> id >> nObjectVisType;
    Object_ABC* object = & resolver_.Get( id );

    if( nObjectVisType == eVisTypeInvisible )
        perceivedObjects_.erase( object );
    else
        perceivedObjects_.insert( object );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectDetections::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void ObjectDetections::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    // $$$$ AGE 2006-04-21: viewport
    if( ! tools.ShouldDisplay( "VisionLines" ) || perceivedObjects_.empty() )
        return;
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glColor4d( COLOR_DETECTED );
    glLineWidth( 1.f );
    for( CIT_Objects it = perceivedObjects_.begin(); it != perceivedObjects_.end(); ++it )
        tools.DrawLine( where, (*it)->Get< Positions >().GetPosition() );
    glPopAttrib();
}
