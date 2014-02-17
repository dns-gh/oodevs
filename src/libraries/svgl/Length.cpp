// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Length.h"
#include "RenderingContext_ABC.h"
#include <sstream>

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Length constructor
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
Length::Length( const std::string& width )
    : baseWidth_ ( 1 )
    , pixelWidth_( 1 )
    , fixed_( false )
    , factor_( 1.f )
{
    std::string unit;
    std::stringstream str( width );
    str >> baseWidth_ >> unit;
    fixed_ = unit.substr( 0, 2 ) == "px";
}

// -----------------------------------------------------------------------------
// Name: Length destructor
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
Length::~Length()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Length::Clone
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
Length& Length::Clone() const
{
    return *new Length( *this );
}

// -----------------------------------------------------------------------------
// Name: Length::ReachTop
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
void Length::ReachTop( const RenderingContext_ABC& context )
{
    pixelWidth_ = fixed_ ? baseWidth_ : baseWidth_ / context.Pixels();
    factor_ = context.LineWidthFactor();
}

// -----------------------------------------------------------------------------
// Name: Length::LeaveTop
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
void Length::LeaveTop( const RenderingContext_ABC& )
{
    factor_ = 1.;
}

// -----------------------------------------------------------------------------
// Name: Length::SetupLineWidth
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
void Length::SetupLineWidth() const
{
    float width = pixelWidth_;
    if( width > 10 ) width = 10;
    if( width < 1 )  width = 1;
    width *= factor_;
    glLineWidth( width );
}

// -----------------------------------------------------------------------------
// Name: Length::GetWidth
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
float Length::GetWidth() const
{
    return baseWidth_;
}

// -----------------------------------------------------------------------------
// Name: Length::IsFixed
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
bool Length::IsFixed() const
{
    return fixed_;
}
