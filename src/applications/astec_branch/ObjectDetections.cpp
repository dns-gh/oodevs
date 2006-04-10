// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectDetections.h"
#include "Controller.h"
#include "Attr_Def.h"
#include "GlTools_ABC.h"
#include "Object.h"
#include "Positions.h"

// -----------------------------------------------------------------------------
// Name: ObjectDetections constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectDetections::ObjectDetections( Controller& controller, const Resolver_ABC< Object >& resolver )
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
    Object* object = & resolver_.Get( id );

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
void ObjectDetections::Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const
{
    if( ! tools.ShouldDisplay( "VisionLines" ) )
        return;
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glColor4d( COLOR_DETECTED );
    glLineWidth( 1.f );
    for( CIT_Objects it = perceivedObjects_.begin(); it != perceivedObjects_.end(); ++it )
        tools.DrawLine( where, (*it)->Get< Positions >().GetPosition() );
    glPopAttrib();
}
