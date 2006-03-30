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
#include "OptionVariant.h"
#include "Controllers.h"
#include "graphics/GraphicData.h"

// -----------------------------------------------------------------------------
// Name: TerrainLayer constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
TerrainLayer::TerrainLayer( Controllers& controllers, QGLWidget& widget, const std::string& dataDirectory )
    : controllers_  ( controllers )
    , dataDirectory_( dataDirectory )
    , widget_       ( widget )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
TerrainLayer::~TerrainLayer()
{
    controllers_.Remove( *this );
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Initialize
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::Initialize( const geometry::Rectangle2f& )
{
    if( shapes_.empty() )
    {
        DataFactory dataFactory;
        GraphicFactory factory( *this, *this, dataFactory );
        factory.LoadGraphicDirectory( dataDirectory_ );
    }
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
        && filename.find( "review" ) == std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Paint
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::Paint( const geometry::Rectangle2f& viewport )
{
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT );
        DrawInnerShapes( viewport );
        DrawShapesBorders( viewport );
        DrawLinearShapes( viewport );
        DrawNames( viewport );
    glPopAttrib();
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
// Name: TerrainLayer::DrawNames
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void TerrainLayer::DrawNames( const geometry::Rectangle2f& viewport ) const
{
    // $$$$ AGE 2006-03-30: 
    // différencier smallNames => routes, rivieres, ... => trucs linéaires
    //              bigNames   => villes, fleuves, ...  => trucs surfaciques
    // lods, ...
    glColor3f( 0, 0, 0 );
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
    {
        const bool linear = IsLinear( **it );
        if( (  linear && smallNames_.IsSet( viewport.Width() < 10000.f ) )
         || ( !linear && bigNames_.IsSet( viewport.Width() < 30000.f ) ) )
            (*it)->DrawName( viewport, widget_ ); // $$$$ AGE 2006-03-30: use tools, not widget
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::IsLinear
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
bool TerrainLayer::IsLinear( const GraphicShape& shape ) const
{
    const GraphicData* data = static_cast< const GraphicData* >( shape.GetData() );
    return (( const TerrainData )( *data )).Linear();
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

// -----------------------------------------------------------------------------
// Name: TerrainLayer::OptionChanged
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void TerrainLayer::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "SmallText" )
        smallNames_ = value.To< TristateOption >();
    if( name == "BigText" )
        bigNames_ = value.To< TristateOption >();
}

