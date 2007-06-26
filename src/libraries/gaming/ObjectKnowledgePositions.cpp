// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectKnowledgePositions.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePositions constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
ObjectKnowledgePositions::ObjectKnowledgePositions( const CoordinateConverter_ABC& converter )
    : LocationPositions( converter )
    , realObjectKnown_( false )
    , perceived_      ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePositions destructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
ObjectKnowledgePositions::~ObjectKnowledgePositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePositions::DoUpdate
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void ObjectKnowledgePositions::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    if( message.m.locationPresent )
        Update( message.location );
    if( message.m.perceivedPresent )
        perceived_ = message.perceived;
    if( message.m.real_objectPresent )
        realObjectKnown_ = true;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePositions::Draw
// Created: AGE 2006-05-19
// -----------------------------------------------------------------------------
void ObjectKnowledgePositions::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    LocationPositions::Draw( where, viewport, tools );
}
