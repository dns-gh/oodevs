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
#include "clients_gui/GLOptions.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Point.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePositions constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
ObjectKnowledgePositions::ObjectKnowledgePositions( const kernel::CoordinateConverter_ABC& converter, const kernel::ObjectKnowledge_ABC& knowledge,
                                                    const kernel::ObjectType& type )
    : LocationPositions( converter )
    , knowledge_( knowledge )
    , type_     ( type )
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
void ObjectKnowledgePositions::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& /*viewport*/, gui::GLView_ABC& tools ) const
{
    if( const kernel::Location_ABC* location = GetLocation() )
        tools.DrawTacticalGraphics( knowledge_.GetSymbol(), *location, tools.GetOptions().IsDrawingSelection(), type_.GetPointSize(),
            dynamic_cast< const kernel::Point* >( location ) != nullptr );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePositions::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void ObjectKnowledgePositions::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    Draw( where, viewport, tools );
}
