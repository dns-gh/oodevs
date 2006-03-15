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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Initialize
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::Initialize( const geometry::Rectangle2f& )
{
    static GraphicSetup setup; // $$$$ AGE 2005-10-20: 
    DataFactory dataFactory;
    GraphicFactory factory( *this, setup, dataFactory );

    factory.LoadGraphicDirectory( dataDirectory_ );
}


// -----------------------------------------------------------------------------
// Name: TerrainLayer::HandleKeyPress
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
bool TerrainLayer::HandleKeyPress( int )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::AddShape
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::AddShape( const GraphicShapeProxy& shape )
{
    shapes_.push_back( shape );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::ShouldUseList
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
bool TerrainLayer::ShouldUseList( const std::string& )
{   
    return false;
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
        it->DrawArea( viewport );
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
        it->DrawBorders( viewport );
       
    glStencilFunc( GL_GEQUAL, 1, 0xFFFFFFFF );
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
        it->DrawBorders( viewport );

    glDisable( GL_STENCIL_TEST );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::DrawLinearShapes
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::DrawLinearShapes( const geometry::Rectangle2f& viewport ) const
{
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
        it->DrawLines( viewport );
}

