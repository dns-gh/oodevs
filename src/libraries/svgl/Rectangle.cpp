// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Rectangle.h"
#include "DrawerTesselator.h"
#include "RenderingContext_ABC.h"
#include "Paint_ABC.h"
#include "Length.h"
#include "DashArray.h"
#include <sstream>

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Rectangle constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Rectangle::Rectangle( xml::xistream& input, References_ABC& references, const PropertyFactory& factory )
    : Node( input, references, factory )
    , x1_( 0 )
    , y1_( 0 )
{
    float height, width;
    input >> xml::optional() >> xml::attribute( "x", x1_ )
          >> xml::optional() >> xml::attribute( "y", y1_ )
          >> xml::attribute( "width", width )
          >> xml::attribute( "height", height );
    x2_ = x1_ + width;
    y2_ = y1_ + height;
}

// -----------------------------------------------------------------------------
// Name: Rectangle constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Rectangle::Rectangle( const Rectangle& rhs )
    : Node( rhs )
    , x1_( rhs.x1_ )
    , y1_( rhs.y1_ )
    , x2_( rhs.x2_ )
    , y2_( rhs.y2_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Rectangle destructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Rectangle::~Rectangle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Rectangle::CompileInternal
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Node& Rectangle::CompileInternal( RenderingContext_ABC& , References_ABC& ) const
{
    return *new Rectangle( *this );
}

// -----------------------------------------------------------------------------
// Name: Rectangle::DrawInternal
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
void Rectangle::DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const
{
    if( context.SetupFill( references ) )
        glRectfv( &x1_, &x2_ );

    if( context.SetupStroke( references ) )
    {
        glBegin( GL_LINE_LOOP );
            glVertex2f( x1_, y1_ );
            glVertex2f( x1_, y2_ );
            glVertex2f( x2_, y2_ );
            glVertex2f( x2_, y1_ );
        glEnd();
    }
}
