// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Line.h"
#include "RenderingContext_ABC.h"
#include "Paint_ABC.h"
#include "Length.h"
#include "DashArray.h"
#include <sstream>

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Line constructor
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
Line::Line( xml::xistream& input, References_ABC& references, const PropertyFactory& factory )
    : Node( input, references, factory )
{
    input >> xml::optional() >> xml::attribute( "x1", from_.X() )
          >> xml::optional() >> xml::attribute( "y1", from_.Y() )
          >> xml::optional() >> xml::attribute( "x2", to_  .X() )
          >> xml::optional() >> xml::attribute( "y2", to_  .Y() );
}

// -----------------------------------------------------------------------------
// Name: Line destructor
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
Line::~Line()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Line::CompileInternal
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
Node& Line::CompileInternal( RenderingContext_ABC& , References_ABC& ) const
{
    return *new Line( *this );
}
    
// -----------------------------------------------------------------------------
// Name: Line::DrawInternal
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
void Line::DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const
{
    if( context.SetupStroke( references ) )
    {
        glBegin( GL_LINES );
            glVertex2fv( (const float*)&from_ );
            glVertex2fv( (const float*)&to_ );
        glEnd();
    }
}
