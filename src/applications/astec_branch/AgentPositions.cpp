// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentPositions.h"
#include "CoordinateConverter.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: AgentPositions constructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentPositions::AgentPositions( const CoordinateConverter& converter )
    : converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentPositions destructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentPositions::~AgentPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::GetPosition
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
geometry::Point2f AgentPositions::GetPosition() const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::DoUpdate
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentPositions::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.positionPresent )
        position_ = converter_.ConvertToXY( message.position );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::IsAt
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool AgentPositions::IsAt( const geometry::Point2f& pos, float /*precision*/ /*= 100.f*/ ) const // $$$$ AGE 2006-03-23: 
{
    const float halfSizeX = 500.f * 0.5f; // $$$$ SBO 2006-03-21: use font size?
    const float sizeY     = 400.f;
    const geometry::Rectangle2f agentBBox( position_.X() - halfSizeX, position_.Y(),
                                           position_.X() + halfSizeX, position_.Y() + sizeY );
    return agentBBox.IsInside( pos );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::IsIn
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool AgentPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( position_ );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void AgentPositions::Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const
{
    tools.DrawCross( where, GL_CROSSSIZE );    
}
