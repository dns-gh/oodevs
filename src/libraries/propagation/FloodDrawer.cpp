// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "FloodDrawer.h"
#include "FloodModel_ABC.h"
#include <windows.h>
#include <gl/gl.h>

using namespace propagation;
using namespace geometry;

// -----------------------------------------------------------------------------
// Name: FloodDrawer constructor
// Created: LGY 2012-06-29
// -----------------------------------------------------------------------------
FloodDrawer::FloodDrawer()
    : callListId_( 0 )
    , depth_( 0 )
    , refDist_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodDrawer constructor
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
FloodDrawer::FloodDrawer( const propagation::FloodModel_ABC& model, const geometry::Point2f& point, int depth, int refDist )
    : callListId_( 0 )
    , point_( point )
    , depth_( depth )
    , refDist_( refDist )
{
    model.GenerateFlood( point, deepAreas_, lowAreas_, depth, refDist );
}

// -----------------------------------------------------------------------------
// Name: FloodDrawer destructor
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
FloodDrawer::~FloodDrawer()
{
    glDeleteLists( callListId_, 1 );
}

// -----------------------------------------------------------------------------
// Name: FloodDrawer::Draw
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
void FloodDrawer::Draw() const
{
    if( callListId_ == 0 )
    {
        callListId_ = glGenLists( 1 );
        RenderTexture();
    }
    glCallList( callListId_ );
}

// -----------------------------------------------------------------------------
// Name: FloodDrawer::RenderTexture
// Created: LGY 2012-06-29
// -----------------------------------------------------------------------------
void FloodDrawer::Reset( const propagation::FloodModel_ABC& model, const geometry::Point2f& point, int depth, int refDist )
{
    model.GenerateFlood( point, deepAreas_, lowAreas_, depth, refDist );
    glDeleteLists( callListId_, 1 );
    callListId_ = 0;
}

namespace
{
    void DrawPolygons( const std::vector< geometry::Polygon2f >& polygons )
    {
        for( auto it = polygons.begin(); it != polygons.end(); ++it )
            if( !it->Vertices().empty() )
            {
                const GLsizei size = static_cast< GLsizei >( it->Vertices().size() );
                glVertexPointer( 2, GL_FLOAT, 0, &it->Vertices().front() );
                glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ); // disable writing to color buffer
                glStencilFunc( GL_ALWAYS, 0x1, 0x1 );
                glStencilOp( GL_KEEP, GL_INVERT, GL_INVERT );
                glDrawArrays( GL_TRIANGLE_FAN, 0, size );
                glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );    // enable writing to color buffer
                glStencilFunc( GL_EQUAL, 0x1, 0x1 );                  // test if it is odd(1)
                glStencilOp( GL_KEEP, GL_INVERT, GL_INVERT );
                glDrawArrays( GL_TRIANGLE_FAN, 0, size );
            }
    }
}

// -----------------------------------------------------------------------------
// Name: FloodDrawer::RenderTexture
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
void FloodDrawer::RenderTexture() const
{
    glNewList( callListId_, GL_COMPILE );
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT | GL_LIGHTING_BIT );
    glColor4f( 1, 1, 1, 1 );
    glEnableClientState( GL_VERTEX_ARRAY );
    glDisable( GL_LIGHTING );
    glEnable( GL_STENCIL_TEST );
    glColor4f( 0, 0, 1.f, 0.5f );
    DrawPolygons( deepAreas_ );
    glColor4f( 0.3f, 0.3f, 1.f, 0.5f );
    DrawPolygons( lowAreas_ );
    glDisableClientState( GL_VERTEX_ARRAY );
    glPopAttrib();
    glEndList();
}

const geometry::Point2f& FloodDrawer::GetCenter() const
{
    return point_;
}

int FloodDrawer::GetReferenceDistance() const
{
    return refDist_;
}

int FloodDrawer::GetDepth() const
{
    return depth_;
}
