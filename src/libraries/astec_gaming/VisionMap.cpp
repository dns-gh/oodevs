// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "VisionMap.h"
#include "astec_gui/GlTools_ABC.h"
#include "astec_kernel/DetectionMap.h"

// -----------------------------------------------------------------------------
// Name: VisionMap constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
VisionMap::VisionMap( const DetectionMap& map )
    : map_( map )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VisionMap destructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
VisionMap::~VisionMap()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VisionMap::Draw
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void VisionMap::Draw( const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    static const double colors[3][4] =
    {
        { COLOR_VISION_DETECTED  },
        { COLOR_VISION_RECO      },
        { COLOR_VISION_IDENTIED  }
    };

    if( vision_.empty() || viewport.Intersect( boundingBox_ ).IsEmpty() )
        return;
    const float translation = map_.GetCellSize() * 0.5;
    glPushMatrix();
    glTranslatef( translation, translation, 0 );
    glPushAttrib( GL_CURRENT_BIT );
    glPointSize( std::ceil( map_.GetCellSize() / tools.Pixels() ) );
        glBegin( GL_POINTS );
            for( CIT_VisionMap it = vision_.begin(); it != vision_.end(); ++it )
            {
                const geometry::Point2f p = map_.Map( it->first.first, it->first.second );
                assert( it->second >= 1 );
                glColor4dv( colors[it->second-1] );
                glVertex2fv( (float*)(&p) );
            }
        glEnd();
    glPopAttrib();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: VisionMap::Clear
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
void VisionMap::Clear()
{
    vision_.clear();
    boundingBox_ = geometry::Rectangle2f();
}

// -----------------------------------------------------------------------------
// Name: VisionMap::ShouldUpdate
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
bool VisionMap::ShouldUpdate( const std::pair< unsigned, unsigned >& cell )
{
    CIT_VisionMap it = vision_.find( cell );
    return it == vision_.end() || it->second < eIdentification;
}

// -----------------------------------------------------------------------------
// Name: VisionMap::Update
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
void VisionMap::Update( const std::pair< unsigned, unsigned >& cell, E_PerceptionResult perception )
{
    if( perception )
    {
        boundingBox_.Incorporate( map_.Map( cell.first, cell.second ) );
        vision_[ cell ] = perception;
    }
}
