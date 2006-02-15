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

// -----------------------------------------------------------------------------
// Name: ObjectDetections constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectDetections::ObjectDetections( Controller& controller, const Resolver_ABC< Object_ABC >& resolver )
    : controller_( controller )
    , resolver_( resolver )
{

}

// -----------------------------------------------------------------------------
// Name: ObjectDetections destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectDetections::~ObjectDetections()
{

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
