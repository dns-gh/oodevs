// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentKnowledgePositions.h"
#include "CoordinateConverter_ABC.h"

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions constructor
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
AgentKnowledgePositions::AgentKnowledgePositions( const CoordinateConverter_ABC& converter )
    : converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions destructor
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
AgentKnowledgePositions::~AgentKnowledgePositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions::GetPosition
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
geometry::Point2f AgentKnowledgePositions::GetPosition() const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions::GetHeight
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
float AgentKnowledgePositions::GetHeight() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions::IsAt
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
bool AgentKnowledgePositions::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/ ) const
{
    const float halfSizeX = 250.f;
    const float sizeY     = 400.f;
    const geometry::Rectangle2f bBox( position_.X() - halfSizeX - precision, position_.Y() - precision,
                                      position_.X() + halfSizeX + precision, position_.Y() + sizeY + precision);
    return bBox.IsInside( pos );
}
    
// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions::IsIn
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
bool AgentKnowledgePositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( position_ );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions::DoUpdate
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void AgentKnowledgePositions::DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate& message )
{
    if( message.m.positionPresent )
        position_ = converter_.ConvertToXY( message.position );
}
