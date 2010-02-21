// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectKnowledgePositions.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePositions constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
ObjectKnowledgePositions::ObjectKnowledgePositions( const kernel::CoordinateConverter_ABC& converter, const kernel::ObjectKnowledge_ABC& knowledge )
    : LocationPositions( converter )
    , knowledge_( knowledge )
    , realObjectKnown_( false )
    , perceived_( false )
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
void ObjectKnowledgePositions::DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message )
{
    if( message.has_location()  )
        Update( message.location() );
    if( message.has_perceived()  )
        perceived_ = message.perceived() != 0;
    if( message.has_real_object()  )
        realObjectKnown_ = true;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePositions::Draw
// Created: AGE 2006-05-19
// -----------------------------------------------------------------------------
void ObjectKnowledgePositions::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& tools ) const
{
    if( const kernel::Location_ABC* location = GetLocation() )
        tools.DrawTacticalGraphics( knowledge_.GetSymbol(), *location, tools.ShouldDisplay() );
}
