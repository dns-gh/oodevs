// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainLayer.h"
#include "graphics/GraphicSetup_ABC.h"
#include "graphics/DataFactory.h"
#include "graphics/GraphicData.h"
#include "graphics/GraphicFactory.h"
#include "graphics/GraphicShape.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/OptionVariant.h"

using namespace pathfind;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainLayer constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
TerrainLayer::TerrainLayer( Controllers& controllers, const GlTools_ABC& tools, GraphicSetup_ABC& setup )
    : controllers_  ( controllers )
    , tools_        ( tools )
    , setup_        ( setup )
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
    Purge();
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Purge
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void TerrainLayer::Purge()
{
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
    {
        const T_ShapeContainer* container = it->second;
        T_ShapeContainer::View whole = container->CreateView();
        while( whole.HasMoreElements() )
            delete whole.NextElement();
        delete container;
    }
    shapes_.clear();
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::NotifyUpdated
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::NotifyUpdated( const ModelLoaded& modelLoaded )
{
    parameters_.Load( modelLoaded.scipioXml_ );
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
        container->SetRefinementPolicy( 15 );
    }
    container->Insert( &shape );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::ShouldLoad
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
bool TerrainLayer::ShouldLoad( const std::string& filename )
{
    return filename.find( ".bin" )   != std::string::npos
        && filename.find( "review" ) == std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Paint
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( shapes_.empty() && !parameters_.graphicsDirectory_.empty() )
    {
        world_.Set( 0, 0, parameters_.width_, parameters_.height_ );
        DataFactory dataFactory;
        GraphicFactory factory( *this, dataFactory );
        factory.LoadGraphicDirectory( parameters_.graphicsDirectory_ );
    }

    glEnableClientState( GL_VERTEX_ARRAY );
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        DrawInnerShapes( viewport );
        DrawShapesBorders( viewport );
        DrawLinearShapes( viewport );
        DrawNames( viewport );
    glPopAttrib();
}

namespace 
{
    struct Drawer {
        explicit Drawer( const geometry::Rectangle2f& viewport )
            : viewport_( viewport )
        {};
        bool operator()( const TesselatedShape* shape ) const  {
            if( ! viewport_.Intersect( shape->BoundingBox() ).IsEmpty() )
                shape->Draw();
            return true;
        }
        geometry::Rectangle2f viewport_;
    };
    struct DrawBorderser {
        explicit DrawBorderser( const geometry::Rectangle2f& viewport )
            : viewport_( viewport )
        {};
        bool operator()( const TesselatedShape* shape )  const {
            if( ! viewport_.Intersect( shape->BoundingBox() ).IsEmpty() )
                shape->DrawBorders();
            return true;
        }
        geometry::Rectangle2f viewport_;
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
    const Drawer drawer( viewport );
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
    {
        const TerrainData& data = it->first;
        if( ShouldDisplayArea( data, viewport ) )
        {
            SetupAreaGraphics( data );
            Apply( *it->second, viewport, drawer );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::DrawShapesBorders
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::DrawShapesBorders( const geometry::Rectangle2f& viewport ) const
{
    const DrawBorderser drawer( viewport );
    glClear( GL_STENCIL_BUFFER_BIT );
    glEnable( GL_STENCIL_TEST );

    glStencilFunc( GL_NEVER, 1, 0xFFFFFFFF );
    glStencilOp( GL_INCR, GL_INCR, GL_INCR );
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
    {
        const TerrainData& data = it->first;
        if( ShouldDisplayBorder( data, viewport ) )
            Apply( *it->second, viewport, drawer );
    }
       
    glStencilFunc( GL_GEQUAL, 1, 0xFFFFFFFF );
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
    {
        const TerrainData& data = it->first;
        if( ShouldDisplayBorder( data, viewport ) )
        {
            SetupBorderGraphics( data );
            Apply( *it->second, viewport, drawer );
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
    const DrawBorderser drawer( viewport );
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
    {
        const TerrainData& data = it->first;
        if( ShouldDisplayLinear( data, viewport ) )
        {
            SetupLineGraphics( data );
            Apply( *it->second, viewport, drawer );
        }
    }
}

namespace
{
    struct Printer {
        Printer( const GlTools_ABC& tools, const geometry::Rectangle2f& viewport )
            : tools_( &tools ) 
            , viewport_( viewport )
        {};
        bool operator()( const TesselatedShape* shape ) const {
            if( ! viewport_.Intersect( shape->BoundingBox() ).IsEmpty() )
                tools_->Print( shape->GetName(), shape->BoundingBox().Center() );
            return true;
        }
        const GlTools_ABC* tools_;
        geometry::Rectangle2f viewport_;
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

    const Printer printer( tools_, viewport );
    glColor3f( 0, 0, 0 );
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
    {
        const TerrainData& data = it->first;
        if( (  data.Linear() && smallNames_.IsSet( viewport.Width() < 10000.f ) )
          ||( !data.Linear() && bigNames_  .IsSet( viewport.Width() < 30000.f ) ) )
            Apply( *it->second, viewport, printer );
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
     const GraphicData data( d );
     setup_.SetupLineGraphics( &data );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::SetupBorderGraphics
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void TerrainLayer::SetupBorderGraphics( const TerrainData& d ) const
{
     const GraphicData data( d );
     setup_.SetupBorderGraphics( &data );
}
    
// -----------------------------------------------------------------------------
// Name: TerrainLayer::SetupAreaGraphics
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void TerrainLayer::SetupAreaGraphics( const TerrainData& d ) const
{
     const GraphicData data( d );
     setup_.SetupAreaGraphics( &data );
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

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Reset
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void TerrainLayer::Reset()
{
    Purge();
}
