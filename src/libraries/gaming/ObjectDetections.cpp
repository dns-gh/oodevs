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
#include "clients_kernel/Controller.h"
#include "Attr_Def.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Positions.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectDetections constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectDetections::ObjectDetections( Controller& controller, const tools::Resolver_ABC< Object_ABC >& resolver )
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
void ObjectDetections::DoUpdate( const MsgsSimToClient::MsgObjectDetection& message )
{
    Object_ABC* object = resolver_.Find( message.object_oid() );  // $$$$ _RC_ SLG 2010-07-28: Peut �tre null s'il s'agit d'un bloc urbain, revoir les d�tection des bloc urbain wrapper
    if( object )
    {
        if( message.visibility() == Common::invisible )
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
void ObjectDetections::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& , const GlTools_ABC& tools ) const
{
    if( ! tools.ShouldDisplay( "VisionLines" ) || perceivedObjects_.empty() )
        return;
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glColor4f( COLOR_DETECTED );
    glLineWidth( 1.f );
    for( CIT_Objects it = perceivedObjects_.begin(); it != perceivedObjects_.end(); ++it )
        tools.DrawLine( where, (*it)->Get< Positions >().GetPosition() );
    glPopAttrib();
}
