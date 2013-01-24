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
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePositions constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
ObjectKnowledgePositions::ObjectKnowledgePositions( const kernel::CoordinateConverter_ABC& converter, const kernel::ObjectKnowledge_ABC& knowledge )
    : LocationPositions( converter )
    , knowledge_( knowledge )
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
void ObjectKnowledgePositions::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    if( message.has_location()  )
        Update( message.location() );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePositions::Draw
// Created: AGE 2006-05-19
// -----------------------------------------------------------------------------
void ObjectKnowledgePositions::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& tools ) const
{
    if( const kernel::Location_ABC* location = GetLocation() )
        tools.DrawTacticalGraphics( knowledge_.GetSymbol(), *location, tools.ShouldDisplay() );
}
