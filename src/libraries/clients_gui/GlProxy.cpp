// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Gl3dWidget.h"
#include "GLOptions.h"
#include "GlProxy.h"
#include "GlTooltip.h"
#include "GlWidget.h"
#include "Layer.h"
#include "TooltipsLayer_ABC.h"
#include "clients_kernel/Filter_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GlProxy constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlProxy::GlProxy( kernel::Controllers& controllers,
                  const kernel::Profile_ABC& profile,
                  const kernel::StaticModel& staticModel,
                  const kernel::EntityResolver_ABC& model,
                  const std::shared_ptr< Lighting_ABC >& lighting )
    : options_( new GLOptions( controllers, profile, staticModel, model, lighting ) )
    , optionsController_( controllers.options_ )
{
    optionsController_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GlProxy destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlProxy::~GlProxy()
{
    optionsController_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::GetOptions
// Created: ABR 2014-10-16
// -----------------------------------------------------------------------------
GLOptions& GlProxy::GetOptions()
{
    return *options_;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::GetOptions
// Created: ABR 2014-10-16
// -----------------------------------------------------------------------------
const GLOptions& GlProxy::GetOptions() const
{
    return *options_;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::SetTooltipsLayer
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
void GlProxy::SetTooltipsLayer( const std::shared_ptr< TooltipsLayer_ABC >& layer )
{
    tooltipLayer_ = layer;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::AddLayers
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::AddLayers( const T_LayersVector& layers )
{
    layers_.insert( layers_.end(), layers.begin(), layers.end() );
    std::for_each( layers.begin(), layers.end(),
        [&]( const T_Layer& layer )
        {
            if( widget2d_ )
                widget2d_->Register( layer );
            if( widget3d_ )
                widget3d_->Register( layer );
        } );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::RemoveLayer
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void GlProxy::RemoveLayer( const T_Layer& layer )
{
    auto it = std::find( layers_.begin(), layers_.end(), layer );
    if( it != layers_.end() )
        layers_.erase( it );
    if( widget2d_ )
        widget2d_->Unregister( layer );
    if( widget3d_ )
        widget3d_->Unregister( layer );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ApplyToLayers
// Created: ABR 2014-09-30
// -----------------------------------------------------------------------------
void GlProxy::ApplyToLayers( const T_LayerFunctor& functor ) const
{
    std::for_each( layers_.begin(), layers_.end(), functor );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::MoveBelow
// Created: ABR 2014-09-24
// -----------------------------------------------------------------------------
bool GlProxy::MoveBelow( const T_Layer& lhs, const T_Layer& rhs )
{
    auto itLhs = std::find( layers_.begin(), layers_.end(), lhs );
    auto itRhs = std::find( layers_.begin(), layers_.end(), rhs );
    if( itLhs == layers_.end() || itRhs == layers_.end() )
        throw std::exception( "Unable to find layers to move below" );
    if( itLhs < itRhs )
        return false;
    layers_.erase( itLhs );
    layers_.insert( itRhs, lhs );
    if( widget2d_ )
        widget2d_->MoveBelow( lhs, rhs );
    return true;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::UpdateLayerOrder
// Created: ABR 2014-09-30
// -----------------------------------------------------------------------------
void GlProxy::UpdateLayerOrder( kernel::Options& options )
{
    T_LayersVector orderedLayers;
    ApplyToLayers( [&orderedLayers]( const T_Layer& layer ) {
        if( layer->IsConfigurable() || layer->GetType() == eLayerTypes_RasterDynamic )
            orderedLayers.insert( orderedLayers.begin(), layer );
    } );
    std::sort( orderedLayers.begin(),
               orderedLayers.end(),
               [&]( const T_Layer& lhs, const T_Layer& rhs ) -> bool {
                   const int lhsPosition = options.Get( "Layers/" + lhs->GetOptionName() + "/Position" ).To< int >();
                   const int rhsPosition = options.Get( "Layers/" + rhs->GetOptionName() + "/Position" ).To< int >();
                   return lhsPosition < rhsPosition;
               } );
    bool moved = true;
    while( moved )
    {
        moved = false;
        for( auto it = orderedLayers.rbegin(); it != orderedLayers.rend() && it + 1 != orderedLayers.rend(); ++it )
            moved |= MoveBelow( *( it + 1 ), *it );
    }
    for( int i = 0; i < static_cast< int >( orderedLayers.size() ); ++i )
    {
        const auto& layer = orderedLayers[ i ];
        layer->SetAlpha( options.Get( "Layers/" + layer->GetOptionName() + "/Alpha" ).To< float >() );
        optionsController_.Change( "Layers/" + layer->GetOptionName() + "/Position", i );
    }
}

// -----------------------------------------------------------------------------
// Name: GlProxy::SetWidget3D
// Created: ABR 2014-09-24
// -----------------------------------------------------------------------------
void GlProxy::SetWidget3D( const std::shared_ptr< Gl3dWidget >& newWidget )
{
    widget3d_ = newWidget;
    if( widget3d_ )
        std::for_each( layers_.begin(), layers_.end(), [&]( const T_Layer& layer ){ 
            widget3d_->Register( layer );
        } );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::SetWidget2D
// Created: ABR 2014-09-24
// -----------------------------------------------------------------------------
void GlProxy::SetWidget2D( const std::shared_ptr< GlWidget >& newWidget )
{
    widget2d_ = newWidget;
    if( widget2d_ )
        std::for_each( layers_.begin(), layers_.end(), [&]( const T_Layer& layer ){ 
            widget2d_->Register( layer );
        } );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ChangeTo
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::ChangeTo( const std::shared_ptr< Gl3dWidget >& newWidget )
{
    view_  = newWidget;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ChangeTo
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::ChangeTo( const std::shared_ptr< GlWidget >& newWidget )
{
    view_  = newWidget;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::CenterOn
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::CenterOn( const geometry::Point2f& point )
{
    view_->CenterOn( point );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::GetCenter
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
geometry::Point2f GlProxy::GetCenter() const
{
    return view_->GetCenter();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Zoom
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
void GlProxy::Zoom( float width )
{
    view_->Zoom( width );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ShouldDisplay
// Created: LGY 2013-03-11
// -----------------------------------------------------------------------------
bool GlProxy::ShouldDisplay( E_LayerTypes type ) const
{
    return view_ && view_->ShouldDisplay( type );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Pixels
// Created: AGE 2007-04-05
// -----------------------------------------------------------------------------
float GlProxy::Pixels( const geometry::Point2f& at /* = geometry::Point2f()*/ ) const
{
    return view_->Pixels( at );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Zoom
// Created: RPD 2009-12-14
// -----------------------------------------------------------------------------
float GlProxy::Zoom() const
{
    return view_->Zoom();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::GetAdaptiveZoomFactor
// Created: RPD 2009-12-14
// -----------------------------------------------------------------------------
float GlProxy::GetAdaptiveZoomFactor( bool bVariableSize /*= true*/ ) const
{
    return view_->GetAdaptiveZoomFactor( bVariableSize );
}

float GlProxy::GetCurrentAlpha() const
{
    return view_->GetCurrentAlpha();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::StipplePattern
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
unsigned short GlProxy::StipplePattern( int factor /* = 1*/ ) const
{
    return view_->StipplePattern( factor );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::SetCurrentColor
// Created: AGE 2007-05-25
// -----------------------------------------------------------------------------
void GlProxy::SetCurrentColor( float r, float g, float b, float a /* = 1*/ )
{
    if( view_ )
        view_->SetCurrentColor( r, g, b, a );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::SetCurrentCursor
// Created: SBO 2008-02-12
// -----------------------------------------------------------------------------
void GlProxy::SetCurrentCursor( const QCursor& cursor )
{
    if( view_ )
        view_->SetCurrentCursor( cursor );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawCross
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawCross( const geometry::Point2f& at, float size /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    if( view_ )
        view_->DrawCross( at, size, unit );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawLine
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawLine( const geometry::Point2f& from, const geometry::Point2f& to, float width ) const
{
    if( view_ )
        view_->DrawLine( from, to, width );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawStippledLine
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void GlProxy::DrawStippledLine( const geometry::Point2f& from, const geometry::Point2f& to ) const
{
    if( view_ )
        view_->DrawStippledLine( from, to );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawLines
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawLines( const T_PointVector& points ) const
{
    if( view_ )
        view_->DrawLines( points );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawRectangle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void GlProxy::DrawRectangle( const T_PointVector& points ) const
{
    if( view_ )
        view_->DrawRectangle( points );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawPolygon
// Created: AGE 2007-05-23
// -----------------------------------------------------------------------------
void GlProxy::DrawPolygon( const T_PointVector& points ) const
{
    if( view_ )
        view_->DrawPolygon( points );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawSelectedPolygon
// Created: JSR 2012-05-14
// -----------------------------------------------------------------------------
void GlProxy::DrawSelectedPolygon( const T_PointVector& points ) const
{
    if( view_ )
        view_->DrawSelectedPolygon( points );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawDecoratedPolygon
// Created: SLG 2009-12-14
// -----------------------------------------------------------------------------
void GlProxy::DrawDecoratedPolygon( const kernel::T_TessellatedPolygon& polygon, const T_PointVector& vertices,
                                    const geometry::Point2f& center, const std::string& name, unsigned int fontHeight )
{
    if( view_ )
        view_->DrawDecoratedPolygon( polygon, vertices, center, name, fontHeight );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawArrow
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawArrow( const geometry::Point2f& from, const geometry::Point2f& to, float size /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    if( view_ )
        view_->DrawArrow( from, to, size, unit );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawCurvedArrow
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawCurvedArrow( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio /* = 0.2f*/, float size /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    if( view_ )
        view_->DrawCurvedArrow( from, to, curveRatio, size, unit );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawArc
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void GlProxy::DrawArc( const geometry::Point2f& center, const geometry::Point2f& p1, const geometry::Point2f& p2, float width ) const
{
    if( view_ )
        view_->DrawArc( center, p1, p2, width );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawCircle
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawCircle( const geometry::Point2f& center, float radius /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    if( view_ )
        view_->DrawCircle( center, radius, unit );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawDisc
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawDisc( const geometry::Point2f& center, float radius /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    if( view_ )
        view_->DrawDisc( center, radius, unit );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawHalfDisc
// Created: JSR 2013-09-25
// -----------------------------------------------------------------------------
void GlProxy::DrawHalfDisc( const geometry::Point2f& center, float angleDegrees, float radius /*= -1.f*/, E_Unit unit /*= meters*/ ) const
{
    if( view_ )
        view_->DrawHalfDisc( center, angleDegrees, radius, unit );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawLife
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawLife( const geometry::Point2f& center, float height, float factor /* = 1.f*/, bool fixedSize /*= true */ ) const
{
    if( view_ )
        view_->DrawLife( center, height, factor, fixedSize );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Print
// Created: AGE 2007-06-20
// -----------------------------------------------------------------------------
void GlProxy::Print( const std::string& message, const geometry::Point2f& where ) const
{
    if( view_ )
        view_->Print( message, where );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Print
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::Print( const std::string& message, const geometry::Point2f& where, const QFont& font ) const
{
    if( view_ )
        view_->Print( message, where, font );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawInfrastructureSymbol
// Created: LGY 2013-06-12
// -----------------------------------------------------------------------------
void GlProxy::DrawInfrastructureSymbol( const std::string& symbol, const geometry::Point2f& where, float factor, float thickness ) const
{
    if( view_ )
        view_->DrawInfrastructureSymbol( symbol, where, factor, thickness );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawApp6Symbol
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawApp6Symbol( const std::string& symbol, const geometry::Point2f& where, float factor /* = 1.f*/, float thickness /* = 1.f*/, unsigned int direction /*= 0*/ ) const
{
    if( view_ )
        view_->DrawApp6Symbol( symbol, where, factor, thickness, direction );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawApp6SymbolFixedSize
// Created: MMC 2013-02-05
// -----------------------------------------------------------------------------
void GlProxy::DrawApp6SymbolFixedSize( const std::string& symbol, const geometry::Point2f& where, float factor, unsigned int direction ) const
{
    if( view_ )
        view_->DrawApp6SymbolFixedSize( symbol, where, factor, direction );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawUnitSymbol
// Created: LDC 2013-04-09
// -----------------------------------------------------------------------------
void GlProxy::DrawUnitSymbol( const std::string& symbol, const std::string& moveSymbol, const std::string& staticSymbol, const std::string& level, bool isMoving, const geometry::Point2f& where, float factor, unsigned int direction, float width, float depth ) const
{
    if( view_ )
        view_->DrawUnitSymbol( symbol, moveSymbol, staticSymbol, level, isMoving, where, factor, direction, width, depth );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawUnitSymbolAndTail
// Created: LDC 2013-04-26
// -----------------------------------------------------------------------------
void GlProxy::DrawUnitSymbolAndTail( const std::string& symbol, const std::string& level, const T_PointVector& points ) const
{
    if( view_ )
        view_->DrawUnitSymbolAndTail( symbol, level, points );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawIcon
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void GlProxy::DrawIcon( const char** xpm, const geometry::Point2f& where, float size /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    if( view_ )
        view_->DrawIcon( xpm, where, size, unit );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawImage
// Created: AGE 2006-11-17
// -----------------------------------------------------------------------------
void GlProxy::DrawImage( const QImage& image, const geometry::Point2f& where ) const
{
    if( view_ )
        view_->DrawImage( image, where );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawCell
// Created: AGE 2006-12-01
// -----------------------------------------------------------------------------
void GlProxy::DrawCell( const geometry::Point2f& center ) const
{
    if( view_ )
        view_->DrawCell( center );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawSvg
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void GlProxy::DrawSvg( const std::string& svg, const geometry::Point2f& center, float ratio /* = 1.f*/, bool fixedSize /*= true*/ ) const
{
    if( view_ )
        view_->DrawSvg( svg, center, ratio, fixedSize );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawTacticalGraphics
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void GlProxy::DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, bool overlined, float pointSize, bool fixedSize /*= true*/ ) const
{
    if( view_ )
        view_->DrawTacticalGraphics( symbol, location, overlined, pointSize, fixedSize );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::CreateTooltip
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
std::unique_ptr< GlTooltip_ABC > GlProxy::CreateTooltip() const
{
    if( tooltipLayer_ )
        return std::unique_ptr< GlTooltip_ABC >( new GlTooltip( *tooltipLayer_ ) );
    throw MASA_EXCEPTION( "ToolTipLayer not defined." );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ShouldEdit
// Created: ABR 2012-05-25
// -----------------------------------------------------------------------------
bool GlProxy::ShouldEdit( const kernel::GraphicalEntity_ABC& selectable ) const
{
    for( auto it = layers_.begin(); it != layers_.end(); ++it )
        if( (*it)->IsIn( selectable ) )
            return !(*it)->IsReadOnly();
    return true;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::FillSelection
// Created: LGY 2013-03-11
// -----------------------------------------------------------------------------
void GlProxy::FillSelection( const geometry::Point2f& point, T_ObjectsPicking& selection,
        const boost::optional< E_LayerTypes >& type )
{
    if( view_ )
        view_->FillSelection( point, selection, type );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::RenderPicking
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void GlProxy::RenderPicking( const T_ObjectPicking& object )
{
    if( view_ )
        view_->RenderPicking( object );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::IsPickingMode
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
bool GlProxy::IsPickingMode() const
{
    return view_ && view_->IsPickingMode();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Picking
// Created: LGY 2013-03-06
// -----------------------------------------------------------------------------
void GlProxy::Picking()
{
    if( view_ )
        view_->Picking();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::LineWidth
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
float GlProxy::LineWidth( float base ) const
{
    return view_->LineWidth( base );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::GetPickingColor
// Created: LGY 2013-03-15
// -----------------------------------------------------------------------------
QColor GlProxy::GetPickingColor() const
{
    return view_->GetPickingColor();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::MapToterrainCoordinates
// Created: NPT 2013-06-12
// -----------------------------------------------------------------------------
geometry::Point2f GlProxy::MapToterrainCoordinates( int x, int y )
{
    return view_->MapToterrainCoordinates( x, y );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::HasFocus
// Created: NPT 2013-06-13
// -----------------------------------------------------------------------------
bool GlProxy::HasFocus()
{
    return view_ && view_->HasFocus();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawShapeText
// Created: LGY 2014-06-12
// -----------------------------------------------------------------------------
void GlProxy::DrawShapeText( const QImage& image, const geometry::Point2f& where ) const
{
    view_->DrawShapeText( image, where );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Purge
// Created: ABR 2014-09-25
// -----------------------------------------------------------------------------
void GlProxy::Purge()
{
    for( auto it = layers_.begin(); it != layers_.end(); ++it )
        ( *it )->Reset();
    view_.reset();
    widget2d_.reset();
    widget3d_.reset();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::GetCurrentPass
// Created: ABR 2014-09-30
// -----------------------------------------------------------------------------
std::string GlProxy::GetCurrentPass() const
{
    return view_ ? view_->GetCurrentPass() : "";
}

// -----------------------------------------------------------------------------
// Name: GlProxy::OptionChanged
// Created: ABR 2014-10-17
// -----------------------------------------------------------------------------
void GlProxy::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    // will move in the GL widgets manager when multi view will be present
    if( !optionsController_.GetGeneralOptions()->Has( name ) )
        GetOptions().Set( name, value );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::OptionChanged
// Created: ABR 2014-10-17
// -----------------------------------------------------------------------------
void GlProxy::NotifyUpdated( const kernel::Filter_ABC& filter )
{
    GetOptions().SetFilterEntity( filter.GetFilteredEntity() );
}
