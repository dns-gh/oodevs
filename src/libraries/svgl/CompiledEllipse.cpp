// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "CompiledEllipse.h"
#include "RenderingContext_ABC.h"
#include "gl_extensions.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: CompiledEllipse constructor
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
CompiledEllipse::CompiledEllipse( const Node& node, const geometry::Point& center, float rx, float ry, unsigned nSegments )
    : Node( node )
{
    points_.reserve( nSegments );
    const float increment = 2.f * std::acos( -1.f ) / float( nSegments );
    float angle = 0;
    for( unsigned i = 0; i < nSegments; ++i )
    {
        points_.push_back( geometry::Point( center.X() + std::cos( angle ) * rx, center.Y() + std::sin( angle ) * ry ) );
        angle += increment;
    }
}

// -----------------------------------------------------------------------------
// Name: CompiledEllipse destructor
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
CompiledEllipse::~CompiledEllipse()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompiledEllipse::CompileInternal
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
Node& CompiledEllipse::CompileInternal( RenderingContext_ABC& , References_ABC& ) const
{
    return *new CompiledEllipse( *this );
}

// -----------------------------------------------------------------------------
// Name: CompiledEllipse::LockArray
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
void CompiledEllipse::LockArray() const
{
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 2, GL_FLOAT, 0, &points_.front() );
    gl::lockArrays( 0, static_cast< GLsizei >( points_.size() ) );
}

// -----------------------------------------------------------------------------
// Name: CompiledEllipse::DrawInternal
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
void CompiledEllipse::DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const
{
    bool arraySet = false;
    if( !points_.empty() && context.SetupFill( references ) )
    {
        arraySet = true;
        LockArray();
        glDrawArrays( GL_POLYGON, 0, static_cast< GLsizei >( points_.size() ));
    }

    if( !points_.empty() && context.SetupStroke( references ) )
    {
        if( ! arraySet )
            LockArray();
        arraySet = true;
        glDrawArrays( GL_LINE_LOOP, 0, static_cast< GLsizei >( points_.size() ));
    }
    if( arraySet )
        gl::unlockArrays();
}
