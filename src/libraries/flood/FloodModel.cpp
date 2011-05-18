// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "FloodModel.h"
#include "ElevationGetter_ABC.h"
#include <queue>

using namespace flood;
using namespace geometry;

const int FloodModel::cellWidth_ = 100;

// -----------------------------------------------------------------------------
// Name: FloodModel constructor
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
FloodModel::FloodModel( const ElevationGetter_ABC& getter )
    : getter_   ( getter)
    , refDist_  ( 0 )
    , oldDepth_ ( -1 )
    , ppCells_  ( 0 )
    , halfWidth_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodModel destructor
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
FloodModel::~FloodModel()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: FloodModel::operator=
// Created: JSR 2010-12-23
// -----------------------------------------------------------------------------
FloodModel& FloodModel::operator=( const FloodModel& from )
{
    Reset();
    center_ = from.center_;
    refDist_= from.refDist_;
    oldDepth_ = from.oldDepth_;
    halfWidth_ = from.halfWidth_;
    CIT_Polygons it;
    for( it = from.deepAreas_.begin(); it != from.deepAreas_.end(); ++it )
        deepAreas_.push_back( new Polygon2f( **it ) );
    for( it = from.lowAreas_.begin(); it != from.lowAreas_.end(); ++it )
        lowAreas_.push_back( new Polygon2f( **it ) );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: FloodModel::ComputePolygons
// Created: JSR 2010-12-08
// -----------------------------------------------------------------------------
void FloodModel::GenerateFlood( const Point2f& center, int depth, int refDist, bool force )
{
    if( !force && ( depth == oldDepth_ && refDist == refDist_ ) )
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
// Name: FloodModel::GenerateFlood
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
void FloodModel::GenerateFlood( const Point2d& center, int depth, int refDist, bool force )
{
    GenerateFlood( Point2f( static_cast< float >( center.X() ), static_cast< float >( center.Y() ) ), depth, refDist, force );
}

// -----------------------------------------------------------------------------
// Name: FloodModel::Propagate
// Created: JSR 2010-12-14
// -----------------------------------------------------------------------------
void FloodModel::Propagate( int floodElevation )
{
    std::queue< std::pair< unsigned short, unsigned short > > queue;
    queue.push( std::make_pair( halfWidth_, halfWidth_ ) );
    while( !queue.empty() )
    {
        unsigned short x = queue.front().first;
        unsigned short y = queue.front().second;
        Point2f cellCenter( center_.X() + ( x - halfWidth_ ) * cellWidth_ , center_.Y() + ( y - halfWidth_ ) * cellWidth_ );
        sCell& cell = ppCells_[ x ][ y ];
        if( !cell.visited_ )
        {
            short elevation = getter_.GetElevationAt( cellCenter );
            cell.visited_ = true;
            if( cellCenter.SquareDistance( center_ ) < refDist_ * refDist_ && elevation <= floodElevation )
            {
                cell.polIndex_ = -1; // to be marked
                if( floodElevation - elevation > 1 )
                    cell.deep_ = true;
                if( x > 0 && !ppCells_[ x - 1 ][ y ].visited_ )
                    queue.push( std::make_pair( x - 1, y ) );
                if( x < 2 * halfWidth_ && !ppCells_[ x + 1 ][ y ].visited_ )
                    queue.push( std::make_pair( x + 1, y ) );
                if( y > 0 && !ppCells_[ x ][ y - 1 ].visited_ )
                    queue.push( std::make_pair( x, y - 1 ) );
                if( y < 2 * halfWidth_ && !ppCells_[ x ][ y + 1 ].visited_ )
                    queue.push( std::make_pair( x, y + 1 ) );
            }
        }
        queue.pop();
    }
}

// -----------------------------------------------------------------------------
// Name: FloodModel::FindFirstUnmarkedCell
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
bool FloodModel::FindFirstUnmarkedCell( int& xRet, int& yRet ) const
{
    return FindFirstMarkedCell( xRet, yRet, -1 );
}

// -----------------------------------------------------------------------------
// Name: FloodModel::FindFirstMarkedCell
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
bool FloodModel::FindFirstMarkedCell( int& xRet, int& yRet, int index ) const
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
// Name: FloodModel::MarkAdjacentCells
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
int FloodModel::MarkAdjacentCells( int x, int y, bool deep, int nPolygonIndex )
{
    ppCells_[ x ][ y ].polIndex_ = nPolygonIndex;
    while( ++x < 2 * halfWidth_ + 1 && ppCells_[ x ][ y ].polIndex_ == -1 && ppCells_[ x ][ y ].deep_ == deep )
        ppCells_[ x ][ y ].polIndex_ = nPolygonIndex;
    return x - 1;
}

// -----------------------------------------------------------------------------
// Name: FloodModel::MarkCells
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
void FloodModel::MarkCells( int xStart, int yStart, int nPolygonIndex )
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
// Name: FloodModel::CreatePolygon
// Created: JSR 2010-12-14
// -----------------------------------------------------------------------------
void FloodModel::CreatePolygon( int nPolygonIndex )
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
// Name: FloodModel::FindUnmarkedOnNextLine
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
int FloodModel::FindUnmarkedOnNextLine( int xStart, int xEnd, int y, bool deep ) const
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
// Name: FloodModel::FindFirstMarkedOnLine
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
int FloodModel::FindFirstMarkedOnLine( int y, int index ) const
{
    int x = -1;
    while( ++x < 2 * halfWidth_ + 1 )
        if( ppCells_[ x ][ y ].polIndex_ == index )
            return x;
    return -1;
}

// -----------------------------------------------------------------------------
// Name: FloodModel::FindLastMarkedOnLine
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
int FloodModel::FindLastMarkedOnLine( int y, int index ) const
{
    int x = 2 * halfWidth_ + 1;
    while( --x >= 0 )
        if( ppCells_[ x ][ y ].polIndex_ == index )
            return x;
    return -1;
}

// -----------------------------------------------------------------------------
// Name: FloodModel::Reset
// Created: JSR 2010-12-14
// -----------------------------------------------------------------------------
void FloodModel::Reset()
{
    for( unsigned int i = 0; i < deepAreas_.size(); ++i )
        delete deepAreas_.at( i );
    for( unsigned int i = 0; i < lowAreas_.size(); ++i )
        delete lowAreas_.at( i );
    deepAreas_.clear();
    lowAreas_.clear();
}
