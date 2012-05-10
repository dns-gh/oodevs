// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "FloodDrawer.h"
#include "FloodModel.h"
#include <windows.h>
#include <gl/gl.h>

using namespace flood;
using namespace geometry;

// -----------------------------------------------------------------------------
// Name: FloodDrawer constructor
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
FloodDrawer::FloodDrawer( const FloodModel& model )
    : model_     ( model )
    , callListId_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodDrawer destructor
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
FloodDrawer::~FloodDrawer()
{
    ResetTexture();
}

// -----------------------------------------------------------------------------
// Name: FloodDrawer::Draw
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
void FloodDrawer::Draw() const
{
    if( callListId_ == 0 )
        const_cast< FloodDrawer* >( this )->RenderTexture();

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT | GL_LIGHTING_BIT );

    glColor4f( 1, 1, 1, 1 );
    glCallList( callListId_ );

    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: FloodDrawer::ResetTexture
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
void FloodDrawer::ResetTexture()
{
    if( callListId_ )
    {
        glDeleteLists( callListId_, 1 );
        callListId_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: FloodDrawer::RenderTexture
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
void FloodDrawer::RenderTexture()
{
    callListId_ = glGenLists( 1 );
    glNewList( callListId_, GL_COMPILE );
    glEnableClientState( GL_VERTEX_ARRAY );
    glDisable(GL_LIGHTING);
    glEnable( GL_STENCIL_TEST );
    glColor4f( 0, 0, 1.f, 0.5f );
    DrawPolygons( model_.GetDeepAreas() );
    glColor4f( 0.3f, 0.3f, 1.f, 0.5f );
    DrawPolygons( model_.GetLowAreas() );
    glDisable( GL_STENCIL_TEST );
    glDisableClientState( GL_VERTEX_ARRAY );
    glEndList();

}

// -----------------------------------------------------------------------------
// Name: FloodDrawer::DrawPolygons
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
void FloodDrawer::DrawPolygons( const FloodModel::T_Polygons& polygons ) const
{
    for( FloodModel::CIT_Polygons it = polygons.begin(); it != polygons.end(); ++it )
        if( !( *it )->Vertices().empty() )
        {
            GLsizei size = static_cast< GLsizei >( ( *it )->Vertices().size() );
            glVertexPointer( 2, GL_FLOAT, 0, &( *it )->Vertices().front() );
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
