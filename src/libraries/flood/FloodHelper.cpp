// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "FloodHelper.h"
#include "ElevationGetter_ABC.h"
#include <queue>
#include <windows.h>
#include <gl/gl.h>

using namespace flood;
using namespace geometry;

const int FloodHelper::cellWidth_ = 100;

// -----------------------------------------------------------------------------
// Name: FloodHelper constructor
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
FloodHelper::FloodHelper( const ElevationGetter_ABC& getter )
    : getter_   ( getter)
    , refDist_  ( 0 )
    , oldDepth_ ( -1 )
    , ppCells_  ( 0 )
    , textureId_( 0 )
    , halfWidth_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodHelper destructor
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
FloodHelper::~FloodHelper()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: FloodHelper::ComputePolygons
// Created: JSR 2010-12-08
// -----------------------------------------------------------------------------
void FloodHelper::GenerateFlood( const Point2f& center, int depth, int refDist )
{
    if( depth == oldDepth_ && refDist == refDist_ )
        return;
    Reset();
    oldDepth_ = depth;
    refDist_ = refDist;
    center_ = center;
    // generate cells
    halfWidth_ = static_cast< unsigned short >( refDist / cellWidth_ );
    int width = 2 * halfWidth_ + 1;
    ppCells_ = new sCell*[ width ];
    for( int x = 0; x < width; ++x )
        ppCells_[ x ] = new sCell[ width ];
    // propagate from center
    Propagate( getter_.GetElevationAt( center ) + depth );
    // mark cells
    int nCurrentPolIndex = 0;
    int unmarkedX, unmarkedY;
    while( FindFirstUnmarkedCell( unmarkedX, unmarkedY ) )
        MarkCells( unmarkedX, unmarkedY, ++nCurrentPolIndex );
    // polygons creation
    for( int nPol = 1; nPol <= nCurrentPolIndex; ++nPol )
        CreatePolygon( nPol );
    // delete cells
    for( unsigned int i = width; i; )
        delete [] ppCells_[ --i ];
    delete [] ppCells_;
}

// -----------------------------------------------------------------------------
// Name: FloodHelper::Draw
// Created: JSR 2010-12-14
// -----------------------------------------------------------------------------
void FloodHelper::Draw() const
{
    if( textureId_ == 0 )
        const_cast< FloodHelper* >( this )->RenderTexture();
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, textureId_ );
    glColor4f( 1, 1, 1, 1 );
    glBegin( GL_QUADS );
        glTexCoord2f( 0, 0 );
        glVertex3f( center_.X() - refDist_, center_.Y() - refDist_, 0 );
        glTexCoord2f( 1, 0 );
        glVertex3f( center_.X() + refDist_, center_.Y() - refDist_, 0 );
        glTexCoord2f( 1, 1 );
        glVertex3f( center_.X() + refDist_, center_.Y() + refDist_, 0 );
        glTexCoord2f( 0, 1 );
        glVertex3f( center_.X() - refDist_, center_.Y() + refDist_, 0 );
    glEnd();
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable( GL_TEXTURE_2D );
}

#define MAKE_PAIR( x, y )  std::make_pair< unsigned short, unsigned short >( ( x ), ( y ) )

// -----------------------------------------------------------------------------
// Name: FloodHelper::Propagate
// Created: JSR 2010-12-14
// -----------------------------------------------------------------------------
void FloodHelper::Propagate( int floodElevation )
{
    std::queue< std::pair< unsigned short, unsigned short > > queue;
    queue.push( MAKE_PAIR( halfWidth_, halfWidth_ ) );
    while( !queue.empty() )
    {
        unsigned short x = queue.front().first;
        unsigned short y = queue.front().second;
        Point2f cellCenter( center_.X() + ( x - halfWidth_ ) * cellWidth_ , center_.Y() + ( y - halfWidth_ ) * cellWidth_ );
        short elevation = getter_.GetElevationAt( cellCenter );
        sCell& cell = ppCells_[ x ][ y ];
        if( !cell.visited_ )
        {
            cell.visited_ = true;
            if( cellCenter.SquareDistance( center_ ) < refDist_ * refDist_ && elevation <= floodElevation )
            {
                cell.polIndex_ = -1; // to be marked
                if( floodElevation - elevation > 1)
                    cell.deep_ = true;
                if( x > 0 && !ppCells_[ x - 1 ][ y ].visited_ )
                    queue.push( MAKE_PAIR( x - 1, y ) );
                if( x < 2 * halfWidth_ + 1 && !ppCells_[ x + 1 ][ y ].visited_ )
                    queue.push( MAKE_PAIR( x + 1, y ) );
                if( y > 0 && !ppCells_[ x ][ y - 1 ].visited_ )
                    queue.push( MAKE_PAIR( x, y - 1 ) );
                if( y < 2 * halfWidth_ && !ppCells_[ x ][ y + 1 ].visited_ )
                    queue.push( MAKE_PAIR( x, y + 1 ) );
            }
        }
        queue.pop();
    }
}

// -----------------------------------------------------------------------------
// Name: FloodHelper::FindFirstUnmarkedCell
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
bool FloodHelper::FindFirstUnmarkedCell( int& xRet, int& yRet ) const
{
    return FindFirstMarkedCell( xRet, yRet, -1 );
}

// -----------------------------------------------------------------------------
// Name: FloodHelper::FindFirstMarkedCell
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
bool FloodHelper::FindFirstMarkedCell( int& xRet, int& yRet, int index ) const
{
    for( int y = 0; y < 2 * halfWidth_ + 1; ++y )
        for( int x = 0; x < 2 * halfWidth_ + 1; ++x )
            if( ppCells_[ x ][ y ].polIndex_ == index )
            {
                xRet = x;
                yRet = y;
                return true;
            }
    return false;    
}

// -----------------------------------------------------------------------------
// Name: FloodHelper::MarkAdjacentCells
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
int FloodHelper::MarkAdjacentCells( int x, int y, bool deep, int nPolygonIndex )
{
    ppCells_[ x ][ y ].polIndex_ = nPolygonIndex;
    while( ++x < 2 * halfWidth_ + 1 && ppCells_[ x ][ y ].polIndex_ == -1 && ppCells_[ x ][ y ].deep_ == deep )
        ppCells_[ x ][ y ].polIndex_ = nPolygonIndex;
    return x - 1;
}

// -----------------------------------------------------------------------------
// Name: FloodHelper::MarkCells
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
void FloodHelper::MarkCells( int xStart, int yStart, int nPolygonIndex )
{
    bool deep = ppCells_[ xStart ][ yStart ].deep_;
    while( xStart != -1 && yStart < 2 * halfWidth_ + 1 )
    {
        int xEnd = MarkAdjacentCells( xStart, yStart, deep, nPolygonIndex );
        xStart = FindUnmarkedOnNextLine( xStart, xEnd, yStart, deep );
        ++yStart;
    }
}

// -----------------------------------------------------------------------------
// Name: FloodHelper::CreatePolygon
// Created: JSR 2010-12-14
// -----------------------------------------------------------------------------
void FloodHelper::CreatePolygon( int nPolygonIndex )
{
    static const float halfCellWidth = static_cast< float >( 0.5 * cellWidth_ );
    static const Vector2f vBottomLeft( -halfCellWidth, -halfCellWidth );
    static const Vector2f vTopLeft( -halfCellWidth, halfCellWidth );
    static const Vector2f vBottomRight( halfCellWidth, -halfCellWidth );
    static const Vector2f vTopRight( halfCellWidth, halfCellWidth );
    int cellX, cellY;
    Point2f center;
    Polygon2f::T_Vertices vertices;
    if( FindFirstMarkedCell( cellX, cellY, nPolygonIndex ) )
    {
        center.Set( center_.X() + ( cellX - halfWidth_ ) * cellWidth_, center_.Y() + ( cellY - halfWidth_ )* cellWidth_ );
        vertices.push_back( center + vBottomLeft );
        vertices.push_back( center + vTopLeft );
        int yIndex = cellY;
        int xLastLine = cellX;
        int xNextLine;
        // go from bottom to top(left side)
        while( ++yIndex != 2 * halfWidth_ + 1 && ( xNextLine = FindFirstMarkedOnLine( yIndex, nPolygonIndex ) ) != -1 )
        {
            center.Set( center_.X() + ( xNextLine - halfWidth_ ) * cellWidth_, center_.Y() + ( yIndex - halfWidth_ )* cellWidth_ );
            if( xLastLine != xNextLine )
            {
                xLastLine = xNextLine;
                vertices.push_back( center + vBottomLeft );
            }
            vertices.push_back( center + vTopLeft );
        }
        // go from top to bottom(right side)
        if( --yIndex >= 0 && ( xNextLine = FindLastMarkedOnLine( yIndex, nPolygonIndex ) ) != -1 )
        {
            center.Set( center_.X() + ( xNextLine - halfWidth_ ) * cellWidth_, center_.Y() + ( yIndex - halfWidth_ ) * cellWidth_ );
            vertices.push_back( center + vTopRight );
            vertices.push_back( center + vBottomRight );
            xLastLine = xNextLine;
            while( --yIndex >= 0 && ( xNextLine = FindLastMarkedOnLine( yIndex, nPolygonIndex ) ) != -1 )
            {
                center.Set( center_.X() + ( xNextLine - halfWidth_ ) * cellWidth_, center_.Y() + ( yIndex - halfWidth_ ) * cellWidth_ );
                if( xLastLine != xNextLine )
                {
                    xLastLine = xNextLine;
                    vertices.push_back( center + vTopRight );
                }
                vertices.push_back( center + vBottomRight );
            }
        }
        if( ppCells_[ cellX ][ cellY ].deep_ )
            deepAreas_.push_back( new Polygon2f( vertices ) );
        else
            lowAreas_.push_back( new Polygon2f( vertices ) );
    }
}

// -----------------------------------------------------------------------------
// Name: FloodHelper::FindUnmarkedOnNextLine
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
int FloodHelper::FindUnmarkedOnNextLine( int xStart, int xEnd, int y, bool deep ) const
{
    if( ++y < 2 * halfWidth_ + 1 )
        for( int x = xStart; x <= xEnd; ++x )
            if( ppCells_[ x ][ y ].polIndex_ == -1 && ppCells_[ x ][ y ].deep_ == deep )
            {
                while( --x != -1 && ppCells_[ x ][ y ].polIndex_ == -1 && ppCells_[ x ][ y ].deep_ == deep )
                    ;
                return x + 1;
            }
    return -1;
}

// -----------------------------------------------------------------------------
// Name: FloodHelper::FindFirstMarkedOnLine
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
int FloodHelper::FindFirstMarkedOnLine( int y, int index ) const
{
    int x = -1;
    while( ++x < 2 * halfWidth_ + 1 )
        if( ppCells_[ x ][ y ].polIndex_ == index )
            return x;
    return -1;
}

// -----------------------------------------------------------------------------
// Name: FloodHelper::FindLastMarkedOnLine
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
int FloodHelper::FindLastMarkedOnLine( int y, int index ) const
{
    int x = 2 * halfWidth_ + 1;
    while( --x >= 0 )
        if( ppCells_[ x ][ y ].polIndex_ == index )
            return x;
    return -1;
}

// -----------------------------------------------------------------------------
// Name: FloodHelper::Reset
// Created: JSR 2010-12-14
// -----------------------------------------------------------------------------
void FloodHelper::Reset()
{
    for( unsigned int i = 0; i < deepAreas_.size(); ++i )
        delete deepAreas_.at( i );
    for( unsigned int i = 0; i < lowAreas_.size(); ++i )
        delete lowAreas_.at( i );
    deepAreas_.clear();
    lowAreas_.clear();
    if( textureId_ )
    {
        glDeleteTextures( 1, &textureId_ );
        textureId_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: FloodHelper::RenderTexture
// Created: JSR 2010-12-14
// -----------------------------------------------------------------------------
void FloodHelper::RenderTexture()
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
    DrawPolygons( deepAreas_ );
    glColor4f( 0.3f, 0.3f, 1.f, 0.7f );
    DrawPolygons( lowAreas_ );
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
// Name: FloodHelper::DrawPolygons
// Created: JSR 2010-12-14
// -----------------------------------------------------------------------------
void FloodHelper::DrawPolygons( const T_Polygons& polygons ) const
{
    float factor = 256.f / refDist_; // 512 / ( 2 * refDist_ )
    float offsetX = center_.X() - refDist_;
    float offsetY = center_.Y() - refDist_;
    for( CIT_Polygons it = polygons.begin(); it != polygons.end(); ++it )
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
