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
#include "GlTools_ABC.h"

using namespace pathfind;

// -----------------------------------------------------------------------------
// Name: TerrainLayer constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
TerrainLayer::TerrainLayer( Controllers& controllers, const GlTools_ABC& tools, const std::string& dataDirectory )
    : controllers_  ( controllers )
    , dataDirectory_( dataDirectory )
    , tools_        ( tools )
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
    {
        const T_ShapeContainer* container = it->second;
        T_ShapeContainer::View whole = container->CreateView();
        while( whole.HasMoreElements() )
            delete whole.NextElement();
        delete container;
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Initialize
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::Initialize( const geometry::Rectangle2f& world )
{
    if( shapes_.empty() )
    {
        world_ = world;
        DataFactory dataFactory;
        GraphicFactory factory( *this, dataFactory );
        factory.LoadGraphicDirectory( dataDirectory_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::AddShape
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::AddShape( GraphicShape& )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::AddShape
// Created: AGE 2006-04-01
// -----------------------------------------------------------------------------
void TerrainLayer::AddShape( TesselatedShape& shape )
{
    const GraphicData* d = static_cast< const GraphicData* >( shape.GetData() );
    const TerrainData& data = *d;
    T_ShapeContainer*& container = shapes_[ data ];
    if( ! container )
    {
        container = new T_ShapeContainer( world_ );
        container->SetRefinementPolicy( 63 );
    }
    container->Insert( &shape );
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

namespace 
{
    struct Drawer {
        bool operator()( const TesselatedShape* shape ) const  {
            shape->Draw();
            return true;
        }
    };
    struct DrawBorderser {
        bool operator()( const TesselatedShape* shape )  const {
            shape->DrawBorders();
            return true;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Apply
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
template< typename Functor >
void TerrainLayer::Apply( const T_ShapeContainer& container, const geometry::Rectangle2f& viewport, const Functor& functor ) const
{
    SegmentIntersecter<> intersecter( viewport.BottomLeft(), viewport.TopRight() );
    container.Apply( intersecter, functor );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::DrawInnerShapes
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::DrawInnerShapes( const geometry::Rectangle2f& viewport ) const
{
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
    {
        const TerrainData& data = it->first;
        if( ShouldDisplayArea( data, viewport ) )
        {
            SetupAreaGraphics( data );
            Apply( *it->second, viewport, Drawer() );
        }
    }
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
    {
        const TerrainData& data = it->first;
        if( ShouldDisplayBorder( data, viewport ) )
            Apply( *it->second, viewport, DrawBorderser() );
    }
       
    glStencilFunc( GL_GEQUAL, 1, 0xFFFFFFFF );
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
    {
        const TerrainData& data = it->first;
        if( ShouldDisplayBorder( data, viewport ) )
        {
            SetupBorderGraphics( data );
            Apply( *it->second, viewport, DrawBorderser() );
        }
    }
    glDisable( GL_STENCIL_TEST );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::DrawLinearShapes
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::DrawLinearShapes( const geometry::Rectangle2f& viewport ) const
{
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
    {
        const TerrainData& data = it->first;
        if( ShouldDisplayLinear( data, viewport ) )
        {
            SetupLineGraphics( data );
            Apply( *it->second, viewport, DrawBorderser() );
        }
    }
}

namespace
{
    struct Printer {
        Printer( const GlTools_ABC& tools ) : tools_( &tools ) {};
        bool operator()( const TesselatedShape* shape ) const {
            tools_->Print( shape->GetName(), shape->BoundingBox().Center() );
            return true;
        }
        const GlTools_ABC* tools_;
    };
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::DrawNames
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void TerrainLayer::DrawNames( const geometry::Rectangle2f& viewport ) const
{
    if( ! bigNames_  .IsSet( viewport.Width() < 30000.f )
     && ! smallNames_.IsSet( viewport.Width() < 10000.f ) )
        return;

    glColor3f( 0, 0, 0 );
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
    {
        const TerrainData& data = it->first;
        if( (  data.Linear() && smallNames_.IsSet( viewport.Width() < 10000.f ) )
          ||( !data.Linear() && bigNames_  .IsSet( viewport.Width() < 30000.f ) ) )
            Apply( *it->second, viewport, Printer( tools_ ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::ShouldDisplayArea
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
bool TerrainLayer::ShouldDisplayArea( const TerrainData& data, const geometry::Rectangle2f& ) const
{
    return data.Border();
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::ShouldDisplayBorder
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
bool TerrainLayer::ShouldDisplayBorder( const TerrainData& data, const geometry::Rectangle2f& viewport ) const
{
    const float width = std::max( viewport.Width(), viewport.Height() );
    return data.Border() && width < 200000.f;
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::ShouldDisplayLinear
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
bool TerrainLayer::ShouldDisplayLinear( const TerrainData& data, const geometry::Rectangle2f& viewport ) const
{
    const float width = std::max( viewport.Width(), viewport.Height() );
    switch( data.Linear() )
    {
    case 0:
    default:
        return false;
    case TerrainData::cliff_:
    case TerrainData::smallriver_:
    case TerrainData::smallroad_:
    case TerrainData::railroad_:
        return width < 100000.f;
    case TerrainData::mediumriver_:
    case TerrainData::mediumroad_:
    case TerrainData::bridge_:
        return width < 150000.f;
    case TerrainData::largeriver_:
    case TerrainData::largeroad_:
    case TerrainData::motorway_:
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::SetupLineGraphics
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void TerrainLayer::SetupLineGraphics( const TerrainData& d ) const
{
     static GraphicSetup setup; // $$$$ AGE 2005-10-20:
     const GraphicData data( d );
     setup.SetupLineGraphics( &data );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::SetupBorderGraphics
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void TerrainLayer::SetupBorderGraphics( const TerrainData& d ) const
{
     static GraphicSetup setup; // $$$$ AGE 2005-10-20:
     const GraphicData data( d );
     setup.SetupBorderGraphics( &data );
}
    
// -----------------------------------------------------------------------------
// Name: TerrainLayer::SetupAreaGraphics
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void TerrainLayer::SetupAreaGraphics( const TerrainData& d ) const
{
     static GraphicSetup setup; // $$$$ AGE 2005-10-20:
     const GraphicData data( d );
     setup.SetupAreaGraphics( &data );

     float colors[4];
     glGetFloatv( GL_CURRENT_COLOR, colors );
     QColor color( colors[0]*255, colors[1]*255, colors[2]*255 );
     int hsv[4];
     color.hsv( hsv, hsv+1, hsv+2 );

     static int factor = 0;
     (*hsv) = ( *hsv + factor++ ) % 360;
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

// -----------------------------------------------------------------------------
// Name: TerrainLayer::ShapeTraits::CompareOnX
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
int TerrainLayer::ShapeTraits::CompareOnX( float rValue, TesselatedShape*const& shape ) const
{
    return pathfind::SegmentIntersecter<>
                ( shape->BoundingBox().BottomLeft()
                , shape->BoundingBox().TopRight() ).CompareOnX( rValue );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::ShapeTraits::CompareOnY
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
int TerrainLayer::ShapeTraits::CompareOnY( float rValue, TesselatedShape*const& shape ) const
{
    return pathfind::SegmentIntersecter<>
                ( shape->BoundingBox().BottomLeft()
                , shape->BoundingBox().TopRight() ).CompareOnY( rValue );
}

