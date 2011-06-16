// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ElevationExtrema.h"
#include <graphics/ElevationMap.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ElevationExtrema constructor
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
ElevationExtrema::ElevationExtrema( const ElevationMap& map )
    : map_( map )
{
    Fill();
}

// -----------------------------------------------------------------------------
// Name: ElevationExtrema destructor
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
ElevationExtrema::~ElevationExtrema()
{
    for( std::vector< T_Container* >::const_iterator it = extrema_.begin(); it != extrema_.end(); ++it )
        delete *it;
}

struct ElevationExtrema::ExtremaFinder
{
    ExtremaFinder( const geometry::Rectangle2< int >& extent )
        : extent_( extent )
        , set_( false )
        , min_( 20000 )
        , max_( 0 ) {}
    bool operator()( const ElevationExtrema::Extrema& extrema )
    {
        if( ! extrema.extent_.Intersect( extent_ ).IsEmpty() )
        {
            set_ = true;
            min_ = std::min( min_, extrema.min_ );
            max_ = std::max( max_, extrema.max_ );
        }
        return true;
    };
    geometry::Rectangle2< int > extent_;
    bool set_;
    short min_, max_;
};

// -----------------------------------------------------------------------------
// Name: ElevationExtrema::FindExtrema
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void ElevationExtrema::FindExtrema( const geometry::Rectangle2f& extent, short& min, short& max ) const
{
    std::pair< unsigned, unsigned > topRight   = map_.Unmap( extent.TopRight() );
    std::pair< unsigned, unsigned > bottomLeft = map_.Unmap( extent.BottomLeft() );
    const geometry::Point2< int > tr( topRight.first,   topRight.second );
    const geometry::Point2< int > bl( bottomLeft.first, bottomLeft.second );
    spatialcontainer::SegmentIntersecter< int > intersecter( tr, bl );

    ExtremaFinder finder( geometry::Rectangle2< int >( bl, tr ) );
    extrema_[ FindLevel( extent.Width(), extent.Height() ) ]->Apply( intersecter, finder );
    min = std::max( short( 0 ),              finder.min_ );
    max = std::min( map_.MaximumElevation(), finder.max_ );
}

// -----------------------------------------------------------------------------
// Name: ElevationExtrema::Extrema::Extrema
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
ElevationExtrema::Extrema::Extrema( int x, int y, int w, int h, short min, short max )
    : extent_( x, y, x+w, y+h )
    , min_( min )
    , max_( max )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElevationExtrema::Extrema::CompareOnX
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
int ElevationExtrema::Extrema::CompareOnX( int value ) const
{
    return spatialcontainer::SegmentIntersecter< int >( extent_.BottomLeft(), extent_.TopRight() ).CompareOnX( value );
}

// -----------------------------------------------------------------------------
// Name: ElevationExtrema::Extrema::CompareOnY
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
int ElevationExtrema::Extrema::CompareOnY( int value ) const
{
    return spatialcontainer::SegmentIntersecter< int >( extent_.BottomLeft(), extent_.TopRight() ).CompareOnY( value );
}

// -----------------------------------------------------------------------------
// Name: ElevationExtrema::Extrema::operator==
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
bool ElevationExtrema::Extrema::operator==( const ElevationExtrema::Extrema& rhs ) const
{
    return extent_ == rhs.extent_;
}

// -----------------------------------------------------------------------------
// Name: ElevationExtrema::Fill
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void ElevationExtrema::Fill()
{
    AddExtrema( 0, 0, 0, map_.Width(), map_.Height() );
}

// -----------------------------------------------------------------------------
// Name: ElevationExtrema::AddExtrema
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
std::pair< short, short > ElevationExtrema::AddExtrema( unsigned level, int x, int y, int w, int h )
{
    if( extrema_.size() <= level )
        extrema_.resize( level + 1 );
    if( ! extrema_[ level ] )
    {
        extrema_[ level ] = new T_Container( geometry::Rectangle2< int >( 0, 0, map_.Width(), map_.Height() ) );
        extrema_[ level ]->SetRefinementPolicy( 20 );
    }

    typedef std::pair< short, short > T_Bermuda;
    T_Bermuda result( 20000, 0 );
    if( w > 100 || h > 100 )
    {
        const int hw = w / 2;
        const int hh = h / 2;

        T_Bermuda sub;
        sub = AddExtrema( level+1, x, y, hw, hh );
        result.first = std::min( result.first, sub.first ); result.second = std::max( result.second, sub.second );
        sub = AddExtrema( level+1, x+hw, y, w-hw, hh );
        result.first = std::min( result.first, sub.first ); result.second = std::max( result.second, sub.second );
        sub = AddExtrema( level+1, x, y+hh, hw, h-hh );
        result.first = std::min( result.first, sub.first ); result.second = std::max( result.second, sub.second );
        sub = AddExtrema( level+1, x+hw, y+hh, w-hw, h-hh );
        result.first = std::min( result.first, sub.first ); result.second = std::max( result.second, sub.second );
    }
    else
    {
        for( unsigned int i = x; i < unsigned int( x + w ); ++i )
            for( unsigned int j = y; j < unsigned int( y + h ); ++j )
            {
                const short elevation = *map_.Data( i, j );
                result.first  = std::min( result.first,  elevation );
                result.second = std::max( result.second, elevation );
            }
    }
    extrema_[ level ]->Insert( Extrema( x, y, w, h, result.first, result.second ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ElevationExtrema::FindLevel
// Created: AGE 2007-02-20
// -----------------------------------------------------------------------------
unsigned ElevationExtrema::FindLevel( float width, float height ) const
{
    float bw = map_.Extent().Width();
    float bh = map_.Extent().Height();
    unsigned level = 0;
    while( width < bw && height < bh && level + 1 < extrema_.size() )
    {
        bw *= 0.5f;
        bh *= 0.5f;
        ++level;
    }
    return level;
}
