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
    : model_    ( model )
    , textureId_( 0 )
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
    if( textureId_ == 0 )
        const_cast< FloodDrawer* >( this )->RenderTexture();
    const geometry::Point2f& center = model_.GetCenter();
    int refDist = model_.GetReferenceDistance();
    glDisable( GL_TEXTURE_GEN_S );
    glDisable( GL_TEXTURE_GEN_T );
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, textureId_ );
    glColor4f( 1, 1, 1, 1 );
    glBegin( GL_QUADS );
        glTexCoord2f( 0, 0 );
        glVertex2f( center.X() - refDist, center.Y() - refDist );
        glTexCoord2f( 1, 0 );
        glVertex2f( center.X() + refDist, center.Y() - refDist );
        glTexCoord2f( 1, 1 );
        glVertex2f( center.X() + refDist, center.Y() + refDist );
        glTexCoord2f( 0, 1 );
        glVertex2f( center.X() - refDist, center.Y() + refDist );
    glEnd();
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable( GL_TEXTURE_2D );
}

// -----------------------------------------------------------------------------
// Name: FloodDrawer::ResetTexture
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
void FloodDrawer::ResetTexture()
{
    if( textureId_ )
    {
        glDeleteTextures( 1, &textureId_ );
        textureId_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: FloodDrawer::RenderTexture
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
void FloodDrawer::RenderTexture()
{
    // TODO voir si on peut créer la texture une fois pour toute au début?
    // TODO créer la texture totalement offscreen?
    // create texture
    glGenTextures( 1, &textureId_ );
    glBindTexture( GL_TEXTURE_2D, textureId_ );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, 4, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
    glBindTexture( GL_TEXTURE_2D, 0 );
    // push
    glPushMatrix();
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLint coords[ 4 ];
    glGetIntegerv( GL_VIEWPORT, coords );
    // draw scene
    glViewport( 0, 0, 512, 512 );
    glOrtho( 0.0f, 512, 0.0f, 512, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glEnable( GL_STENCIL_TEST );
    glColor4f( 0, 0, 1.f, 0.7f );
    DrawPolygons( model_.GetDeepAreas() );
    glColor4f( 0.3f, 0.3f, 1.f, 0.7f );
    DrawPolygons( model_.GetLowAreas() );
    glDisable( GL_STENCIL_TEST );
    // copy texture
    glBindTexture( GL_TEXTURE_2D, textureId_ );
    glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA , 0, 0, 512, 512, 0 );
    glBindTexture( GL_TEXTURE_2D, 0 );
    // pop
    glPopMatrix();
    glViewport( coords[ 0 ], coords[ 1 ], coords[ 2 ], coords[ 3 ] );
}

// -----------------------------------------------------------------------------
// Name: FloodDrawer::DrawPolygons
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
void FloodDrawer::DrawPolygons( const FloodModel::T_Polygons& polygons ) const
{
    float factor = 256.f / model_.GetReferenceDistance(); // 512 / ( 2 * refDist_ )
    float offsetX = model_.GetCenter().X() - model_.GetReferenceDistance();
    float offsetY = model_.GetCenter().Y() - model_.GetReferenceDistance();
    for( FloodModel::CIT_Polygons it = polygons.begin(); it != polygons.end(); ++it )
        if( !( *it )->Vertices().empty() )
        {
            glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ); // disable writing to color buffer
            glStencilFunc( GL_ALWAYS, 0x1, 0x1 );
            glStencilOp( GL_KEEP, GL_INVERT, GL_INVERT );
            glBegin( GL_TRIANGLE_FAN );
            for( Polygon2f::CIT_Vertices it2 = ( *it )->Vertices().begin(); it2 != ( *it )->Vertices().end(); ++it2 )
                glVertex2d( ( it2->X() - offsetX ) * factor, ( it2->Y() - offsetY ) * factor );
            glEnd();
            glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );    // enable writing to color buffer
            glStencilFunc( GL_EQUAL, 0x1, 0x1 );                  // test if it is odd(1)
            glStencilOp( GL_KEEP, GL_INVERT, GL_INVERT );
            glBegin( GL_TRIANGLE_FAN );
            for( Polygon2f::CIT_Vertices it2 = ( *it )->Vertices().begin(); it2 != ( *it )->Vertices().end(); ++it2 )
                glVertex2d( ( it2->X() - offsetX ) * factor, ( it2->Y() - offsetY ) * factor );
            glEnd();
        }
}
