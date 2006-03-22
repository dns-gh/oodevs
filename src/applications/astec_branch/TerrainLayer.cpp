// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "TerrainLayer.h"
#include "graphics/GraphicSetup.h"
#include "graphics/DataFactory.h"
#include "graphics/GraphicFactory.h"
#include "graphics/GraphicShape.h"

// -----------------------------------------------------------------------------
// Name: TerrainLayer constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
TerrainLayer::TerrainLayer( const std::string& dataDirectory )
    : dataDirectory_( dataDirectory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
TerrainLayer::~TerrainLayer()
{
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Initialize
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::Initialize( const geometry::Rectangle2f& )
{
    DataFactory dataFactory;
    GraphicFactory factory( *this, *this, dataFactory );
    factory.LoadGraphicDirectory( dataDirectory_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::AddShape
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::AddShape( GraphicShape& shape )
{
    shapes_.push_back( &shape );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::ShouldLoad
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
bool TerrainLayer::ShouldLoad( const std::string& filename )
{
    return filename.find( ".bin" )    != std::string::npos
        && filename.find( "preview" ) == std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Paint
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::Paint( const geometry::Rectangle2f& viewport )
{
    DrawInnerShapes( viewport );
    DrawShapesBorders( viewport );
    DrawLinearShapes( viewport );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::DrawInnerShapes
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::DrawInnerShapes( const geometry::Rectangle2f& viewport ) const
{
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
        (*it)->DrawArea( viewport );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::DrawShapesBorders
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::DrawShapesBorders( const geometry::Rectangle2f& viewport ) const
{
    glClear( GL_STENCIL_BUFFER_BIT );
    glEnable( GL_STENCIL_TEST );

    glStencilFunc( GL_NEVER, 1, 0xFFFFFFFF );
    glStencilOp( GL_INCR, GL_INCR, GL_INCR );
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
        (*it)->DrawBorders( viewport );
       
    glStencilFunc( GL_GEQUAL, 1, 0xFFFFFFFF );
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
        (*it)->DrawBorders( viewport );

    glDisable( GL_STENCIL_TEST );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::DrawLinearShapes
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::DrawLinearShapes( const geometry::Rectangle2f& viewport ) const
{
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
        (*it)->DrawLines( viewport );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::SetupLineGraphics
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void TerrainLayer::SetupLineGraphics( const Data_ABC* pData )
{
     static GraphicSetup setup; // $$$$ AGE 2005-10-20: 
     setup.SetupLineGraphics( pData );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::SetupBorderGraphics
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void TerrainLayer::SetupBorderGraphics( const Data_ABC* pData )
{
     static GraphicSetup setup; // $$$$ AGE 2005-10-20: 
     setup.SetupBorderGraphics( pData );
}
    
// -----------------------------------------------------------------------------
// Name: TerrainLayer::SetupAreaGraphics
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void TerrainLayer::SetupAreaGraphics( const Data_ABC* pData )
{
     static GraphicSetup setup; // $$$$ AGE 2005-10-20: 
     setup.SetupAreaGraphics( pData );

     float colors[4];
     glGetFloatv( GL_CURRENT_COLOR, colors );
     QColor color( colors[0]*255, colors[1]*255, colors[2]*255 );
     int hsv[4];
     color.hsv( hsv, hsv+1, hsv+2 );

     static int factor = 0;
     (*hsv) = ( *hsv + ( factor++ / 100 ) ) % 360;
     color.setHsv( *hsv,*(hsv+1),*(hsv+2) );
     glColor4f( color.red()/255.f, color.green()/255.f, color.blue()/255.f, colors[3] );
}

