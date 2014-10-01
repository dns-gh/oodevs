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

using namespace propagation;
using namespace geometry;

const int FloodModel::cellWidth_ = 100;

// -----------------------------------------------------------------------------
// Name: FloodModel constructor
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
FloodModel::FloodModel( const ElevationGetter_ABC& getter )
    : getter_( getter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodModel destructor
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
FloodModel::~FloodModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodModel::ComputePolygons
// Created: JSR 2010-12-08
// -----------------------------------------------------------------------------
void FloodModel::GenerateFlood( const Point2f& center, T_Polygons& deepAreas, T_Polygons& lowAreas, int depth, int refDist ) const
{
    deepAreas.clear();
    lowAreas.clear();
    unsigned short halfWidth = static_cast< unsigned short >( refDist / cellWidth_ );
    int width = 2 * halfWidth + 1;
    sCell** ppCells = new sCell*[ width ];
    for( int x = 0; x < width; ++x )
        ppCells[ x ] = new sCell[ width ];
    // propagate from center
    Propagate( getter_.GetElevationAt( center ) + depth, halfWidth, center, ppCells, refDist );
    // mark cells
    int nCurrentPolIndex = 0;
    int unmarkedX, unmarkedY;
    while( FindFirstUnmarkedCell( unmarkedX, unmarkedY, halfWidth, ppCells ) )
        MarkCells( unmarkedX, unmarkedY, ++nCurrentPolIndex, halfWidth, ppCells );

    // polygons creation
    for( int nPol = 1; nPol <= nCurrentPolIndex; ++nPol )
        CreatePolygon( nPol, center, halfWidth, ppCells, deepAreas, lowAreas );

    // delete cells
    for( unsigned int i = width; i; )
        delete [] ppCells[ --i ];
    delete [] ppCells;
}

// -----------------------------------------------------------------------------
// Name: FloodModel::GenerateFlood
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
void FloodModel::GenerateFlood( const Point2d& center, T_Polygons& deepAreas, T_Polygons& lowAreas, int depth, int refDist ) const
{
    return GenerateFlood( Point2f( static_cast< float >( center.X() ), static_cast< float >( center.Y() ) ), deepAreas, lowAreas, depth, refDist );
}

// -----------------------------------------------------------------------------
// Name: FloodModel::GetMaxElevationInCell
// Created: JSR 2014-08-11
// -----------------------------------------------------------------------------
double FloodModel::GetMaxElevationInCell( const geometry::Point2f& center, double floodElevation ) const
{
    const float size = getter_.GetCellSize();
    const float x1 = center.X() - cellWidth_ * 0.5f;
    const float y1 = center.Y() - cellWidth_ * 0.5f;
    const float x2 = center.X() + cellWidth_ * 0.5f;
    const float y2 = center.Y() + cellWidth_ * 0.5f;
    double ret = 0;
    for( float y = y1; y < y2; y += size )
        for( float x = x1; x < x2; x += size )
        {
            ret = std::max( ret, getter_.GetElevationAt( geometry::Point2f( x + 0.5f * size, y + 0.5f * size ) ) );
            if( ret > floodElevation )
                return ret;
        }
    return ret;
}

// -----------------------------------------------------------------------------
// Name: FloodModel::Propagate
// Created: JSR 2010-12-14
// -----------------------------------------------------------------------------
void FloodModel::Propagate( double floodElevation, unsigned short halfWidth, const Point2f& center, sCell** ppCells, int refDist ) const
{
    std::queue< std::pair< unsigned short, unsigned short > > queue;
    queue.push( std::make_pair( halfWidth, halfWidth ) );
    while( !queue.empty() )
    {
        const unsigned short x = queue.front().first;
        const unsigned short y = queue.front().second;
        sCell& cell = ppCells[ x ][ y ];
        if( !cell.visited_ )
        {
            const Point2f cellCenter( center.X() + ( x - halfWidth ) * cellWidth_ , center.Y() + ( y - halfWidth ) * cellWidth_ );
            cell.visited_ = true;
            if( cellCenter.SquareDistance( center ) < refDist * refDist )
            {
                const double elevation = GetMaxElevationInCell( cellCenter, floodElevation );
                if( elevation <= floodElevation )
                {
                    cell.polIndex_ = -1; // to be marked
                    if( floodElevation - elevation > 1 )
                        cell.deep_ = true;
                    if( x > 0 && !ppCells[ x - 1 ][ y ].visited_ )
                        queue.push( std::make_pair( x - 1, y ) );
                    if( x < 2 * halfWidth && !ppCells[ x + 1 ][ y ].visited_ )
                        queue.push( std::make_pair( x + 1, y ) );
                    if( y > 0 && !ppCells[ x ][ y - 1 ].visited_ )
                        queue.push( std::make_pair( x, y - 1 ) );
                    if( y < 2 * halfWidth && !ppCells[ x ][ y + 1 ].visited_ )
                        queue.push( std::make_pair( x, y + 1 ) );
                }
            }
        }
        queue.pop();
    }
}

// -----------------------------------------------------------------------------
// Name: FloodModel::FindFirstUnmarkedCell
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
bool FloodModel::FindFirstUnmarkedCell( int& xRet, int& yRet, unsigned short halfWidth, sCell** ppCells ) const
{
    return FindFirstMarkedCell( xRet, yRet, -1, halfWidth, ppCells );
}

// -----------------------------------------------------------------------------
// Name: FloodModel::FindFirstMarkedCell
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
bool FloodModel::FindFirstMarkedCell( int& xRet, int& yRet, int index, unsigned short halfWidth, sCell** ppCells ) const
{
    for( int y = 0; y < 2 * halfWidth + 1; ++y )
        for( int x = 0; x < 2 * halfWidth + 1; ++x )
            if( ppCells[ x ][ y ].polIndex_ == index )
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
int FloodModel::MarkAdjacentCells( int x, int y, bool deep, int nPolygonIndex, unsigned short halfWidth, sCell** ppCells ) const
{
    ppCells[ x ][ y ].polIndex_ = nPolygonIndex;
    while( ++x < 2 * halfWidth + 1 && ppCells[ x ][ y ].polIndex_ == -1 && ppCells[ x ][ y ].deep_ == deep )
        ppCells[ x ][ y ].polIndex_ = nPolygonIndex;
    return x - 1;
}

// -----------------------------------------------------------------------------
// Name: FloodModel::MarkCells
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
void FloodModel::MarkCells( int xStart, int yStart, int nPolygonIndex, unsigned short halfWidth, sCell** ppCells ) const
{
    const bool deep = ppCells[ xStart ][ yStart ].deep_;
    while( xStart != -1 && yStart < 2 * halfWidth + 1 )
    {
        const int xEnd = MarkAdjacentCells( xStart, yStart, deep, nPolygonIndex, halfWidth, ppCells );
        xStart = FindUnmarkedOnNextLine( xStart, xEnd, yStart, deep, halfWidth, ppCells );
        ++yStart;
    }
}

// -----------------------------------------------------------------------------
// Name: FloodModel::CreatePolygon
// Created: JSR 2010-12-14
// -----------------------------------------------------------------------------
void FloodModel::CreatePolygon( int nPolygonIndex, const geometry::Point2f& center, unsigned short halfWidth, sCell** ppCells, T_Polygons& deepAreas, T_Polygons& lowAreas ) const
{
    static const float halfCellWidth = static_cast< float >( 0.5 * cellWidth_ );
    static const Vector2f vBottomLeft( -halfCellWidth, -halfCellWidth );
    static const Vector2f vTopLeft( -halfCellWidth, halfCellWidth );
    static const Vector2f vBottomRight( halfCellWidth, -halfCellWidth );
    static const Vector2f vTopRight( halfCellWidth, halfCellWidth );
    int cellX, cellY;
    Point2f point;
    Polygon2f::T_Vertices vertices;
    if( FindFirstMarkedCell( cellX, cellY, nPolygonIndex, halfWidth, ppCells ) )
    {
        point.Set( center.X() + ( cellX - halfWidth ) * cellWidth_, center.Y() + ( cellY - halfWidth )* cellWidth_ );
        vertices.push_back( point + vBottomLeft );
        vertices.push_back( point + vTopLeft );
        int yIndex = cellY;
        int xLastLine = cellX;
        int xNextLine;
        // go from bottom to top(left side)
        while( ++yIndex != 2 * halfWidth + 1 && ( xNextLine = FindFirstMarkedOnLine( yIndex, nPolygonIndex, halfWidth, ppCells ) ) != -1 )
        {
            point.Set( center.X() + ( xNextLine - halfWidth ) * cellWidth_, center.Y() + ( yIndex - halfWidth )* cellWidth_ );
            if( xLastLine != xNextLine )
            {
                xLastLine = xNextLine;
                vertices.push_back( point + vBottomLeft );
            }
            vertices.push_back( point + vTopLeft );
        }
        // go from top to bottom(right side)
        if( --yIndex >= 0 && ( xNextLine = FindLastMarkedOnLine( yIndex, nPolygonIndex, halfWidth, ppCells ) ) != -1 )
        {
            point.Set( center.X() + ( xNextLine - halfWidth ) * cellWidth_, center.Y() + ( yIndex - halfWidth ) * cellWidth_ );
            vertices.push_back( point + vTopRight );
            vertices.push_back( point + vBottomRight );
            xLastLine = xNextLine;
            while( --yIndex >= 0 && ( xNextLine = FindLastMarkedOnLine( yIndex, nPolygonIndex, halfWidth, ppCells ) ) != -1 )
            {
                point.Set( center.X() + ( xNextLine - halfWidth ) * cellWidth_, center.Y() + ( yIndex - halfWidth ) * cellWidth_ );
                if( xLastLine != xNextLine )
                {
                    xLastLine = xNextLine;
                    vertices.push_back( point + vTopRight );
                }
                vertices.push_back( point + vBottomRight );
            }
        }
        if( ppCells[ cellX ][ cellY ].deep_ )
            deepAreas.push_back( Polygon2f( vertices ) );
        else
            lowAreas.push_back( Polygon2f( vertices ) );
    }
}

// -----------------------------------------------------------------------------
// Name: FloodModel::FindUnmarkedOnNextLine
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
int FloodModel::FindUnmarkedOnNextLine( int xStart, int xEnd, int y, bool deep, unsigned short halfWidth, sCell** ppCells ) const
{
    if( ++y < 2 * halfWidth + 1 )
        for( int x = xStart; x <= xEnd; ++x )
            if( ppCells[ x ][ y ].polIndex_ == -1 && ppCells[ x ][ y ].deep_ == deep )
            {
                while( --x != -1 && ppCells[ x ][ y ].polIndex_ == -1 && ppCells[ x ][ y ].deep_ == deep )
                    ;
                return x + 1;
            }
    return -1;
}

// -----------------------------------------------------------------------------
// Name: FloodModel::FindFirstMarkedOnLine
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
int FloodModel::FindFirstMarkedOnLine( int y, int index, unsigned short halfWidth, sCell** ppCells ) const
{
    int x = -1;
    while( ++x < 2 * halfWidth + 1 )
        if( ppCells[ x ][ y ].polIndex_ == index )
            return x;
    return -1;
}

// -----------------------------------------------------------------------------
// Name: FloodModel::FindLastMarkedOnLine
// Created: JSR 2010-12-10
// -----------------------------------------------------------------------------
int FloodModel::FindLastMarkedOnLine( int y, int index, unsigned short halfWidth, sCell** ppCells ) const
{
    int x = 2 * halfWidth + 1;
    while( --x >= 0 )
        if( ppCells[ x ][ y ].polIndex_ == index )
            return x;
    return -1;
}

// -----------------------------------------------------------------------------
// Name: FloodModel::serialize
// Created: LGY 2012-07-31
// -----------------------------------------------------------------------------
template< typename Archive >
void FloodModel::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< FloodModel_ABC >( *this );
}
