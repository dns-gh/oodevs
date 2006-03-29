// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Elevation3dLayer.h"
#include "graphics/ElevationTextureTree.h"
#include "graphics/Visitor3d.h"
#include "GLTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: Elevation3dLayer constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation3dLayer::Elevation3dLayer( const ElevationMap& elevation )
    : elevation_( elevation )
    , tree_( 0 )
    , zRatio_( 5.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Elevation3dLayer destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation3dLayer::~Elevation3dLayer()
{
    delete tree_;
}

// -----------------------------------------------------------------------------
// Name: Elevation3dLayer::Initialize
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Elevation3dLayer::Initialize( const geometry::Rectangle2f& )
{
    if( ! tree_ )
        tree_ = new ElevationTextureTree( elevation_, *this );
}

// -----------------------------------------------------------------------------
// Name: Elevation3dLayer::HandleKeyPress
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
bool Elevation3dLayer::HandleKeyPress( QKeyEvent* event )
{
    if( event  && event ->key() == Qt::Key_Plus )
    {
        zRatio_ *= 1.1f;
        return true;
    }
    if( event  && event ->key() == Qt::Key_Minus )
    {
        zRatio_ *= 0.9f;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Elevation3dLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Elevation3dLayer::Paint( const ViewFrustum& frustum )
{
    glPushAttrib( GL_CURRENT_BIT | GL_TEXTURE_BIT );
    glPushMatrix(); 
        glScalef( 1.f, 1.f, zRatio_ );
        glColor3f( 1, 1, 1 );
        Visitor3d visitor( elevation_, frustum );
        tree_->Accept( visitor );
    glPopMatrix();
    glPopAttrib();
}

namespace
{
    unsigned char Map( short nIn, short nInMin, short nInMax, unsigned char nOutMin, unsigned int nOutMax )
    {
        if( nIn < nInMin )
            nIn = nInMin;
        if( nIn > nInMax )
            nIn = nInMax;
        const float rRatio = float( nIn - nInMin ) / float( nInMax - nInMin );
        const float rOut   = nOutMin + rRatio * ( float( nOutMax ) - float( nOutMin ) );
        return unsigned char( floor( rOut ) );
    }
}


// -----------------------------------------------------------------------------
// Name: Elevation3dLayer::SelectColor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Elevation3dLayer::SelectColor( short elevation, short maxElevation, unsigned char* color )
{
    static const unsigned char colors[6][3] =
    {
        { 0,   60 , 0   },
        { 0,   96 , 0   },
        { 60,  120, 0   },
        { 120, 96 , 0   },
        { 120, 0,   0   },
        { 120, 120, 120 }
    };

    if( elevation == 0 )
    {
        *color = *(color+1) = 0;
        *(color+2) = 128;
        return;
    }

    const unsigned int  nSliceLength = maxElevation / 5;
    if( nSliceLength > 0 )
    {
        const int nSlice = elevation / nSliceLength;
        for( unsigned int nComposante = 0; nComposante < 3; ++nComposante )
            *( color + nComposante ) = Map( elevation, short( nSlice * nSliceLength ), short( ( nSlice + 1 ) * nSliceLength ), colors[nSlice][nComposante], colors[nSlice+1][nComposante] );
        
    }
}
