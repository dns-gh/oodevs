// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "VisionMap.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/DetectionMap.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: VisionMap constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
VisionMap::VisionMap( const DetectionMap& map )
    : map_( map )
    , left_  (  2000000 )
    , bottom_(  2000000 )
    , right_ ( -2000000 )
    , top_   ( -2000000 )
    , vision_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VisionMap destructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
VisionMap::~VisionMap()
{
    delete[] vision_;
}

// -----------------------------------------------------------------------------
// Name: VisionMap::Draw
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void VisionMap::Draw( const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    static const float colors[3][4] =
    {
        { COLOR_VISION_DETECTED  },
        { COLOR_VISION_RECO      },
        { COLOR_VISION_IDENTIED  }
    };

    if( viewport.Intersect( boundingBox_ ).IsEmpty() || !vision_ )
        return;

    const float translation = map_.GetCellSize() * 0.5;
    glPushMatrix();
    glTranslatef( translation, translation, 0 );
    glPushAttrib( GL_CURRENT_BIT );
    glPointSize( std::ceil( map_.GetCellSize() / tools.Pixels() ) );
    // $$$$ AGE 2006-11-28: y'a pas moins efficace...
    // $$$$ AGE 2007-02-01: mettre une texture et envoyer

    glBegin( GL_POINTS );
    for( char color = 1; color <= 3; ++color )
    {
        glColor4fv( colors[ color-1 ] );
        for( int y = 0; y < height_; ++y )
            for( int x = 0; x < width_; ++x )
                if( vision_[ y * width_ + x ] == color )
                {
                    const unsigned realX = left_   + x;
                    const unsigned realY = bottom_ + y;
                    const geometry::Point2f p = map_.Map( realX, realY );
                    tools.DrawCell( p );
                }
    }
    glEnd();
    glPopAttrib();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: VisionMap::Incorporate
// Created: AGE 2007-02-01
// -----------------------------------------------------------------------------
void VisionMap::Incorporate( const geometry::Rectangle2< int >& extent )
{
    Incorporate( extent.BottomLeft() );
    Incorporate( extent.TopRight() );
}

// -----------------------------------------------------------------------------
// Name: VisionMap::Incorporate
// Created: AGE 2007-02-01
// -----------------------------------------------------------------------------
void VisionMap::Incorporate( const geometry::Point2< int >& point )
{
    left_   = std::min( left_,   point.X() );
    bottom_ = std::min( bottom_, point.Y() );
    right_  = std::max( right_,  point.X() );
    top_    = std::max( top_,    point.Y() );
    boundingBox_.Incorporate( map_.Map( point.X(), point.Y() ) );
}

// -----------------------------------------------------------------------------
// Name: VisionMap::Initialize
// Created: AGE 2007-02-01
// -----------------------------------------------------------------------------
void VisionMap::Initialize()
{
    width_  = right_ - left_   + 1;
    height_ = top_   - bottom_ + 1;
    if( width_ > 0 && height_ > 0 )
    {
        delete[] vision_;
        vision_ = new char[ width_ * height_ ];
        memset( vision_, 0, width_ * height_ );
    }
}

// -----------------------------------------------------------------------------
// Name: VisionMap::DataAt
// Created: AGE 2007-02-01
// -----------------------------------------------------------------------------
char VisionMap::DataAt( const std::pair< unsigned, unsigned >& cell )
{
    if( vision_ 
     && cell.first  >= left_   && cell.first  <= right_
     && cell.second >= bottom_ && cell.second <= top_ )
    {
        const unsigned x = cell.first  - left_;
        const unsigned y = cell.second - bottom_;
        return vision_[ y * width_ + x ];
    }
    return 127;
}

// -----------------------------------------------------------------------------
// Name: VisionMap::ShouldUpdate
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
bool VisionMap::ShouldUpdate( const std::pair< unsigned, unsigned >& cell )
{
    const char d = DataAt( cell );
    return d >= eNotSeen && d < eIdentification;
}

// -----------------------------------------------------------------------------
// Name: VisionMap::Update
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
void VisionMap::Update( const std::pair< unsigned, unsigned >& cell, E_PerceptionResult perception )
{
    if( perception && cell.first  >= left_ && cell.first  <= right_ && cell.second >= bottom_ && cell.second <= top_ )
    {
        const unsigned x = cell.first  - left_;
        const unsigned y = cell.second - bottom_;
        vision_[ y * width_ + x ] = char( perception );
    }
}
