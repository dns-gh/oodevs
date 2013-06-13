// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Gl3dWidget.h"
#include "GlProxy.h"
#include "GlTooltip.h"
#include "GlWidget.h"
#include "Layer.h"
#include "TooltipsLayer_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: GlProxy constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlProxy::GlProxy()
    : view_        ( 0 )
    , tools_       ( 0 )
    , tooltipLayer_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GlProxy destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlProxy::~GlProxy()
{
    for( IT_Layers it = layers_.begin(); it != layers_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Register
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::Register( Layer& layer )
{
    layers_.push_back( & layer );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Register
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
void GlProxy::Register( TooltipsLayer_ABC& layer )
{
    layers_.push_back( & layer );
    tooltipLayer_ = &layer;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Unregister
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void GlProxy::Unregister( Layer& layer )
{
    IT_Layers it = std::find( layers_.begin(), layers_.end(), &layer );
    if( it != layers_.end() )
        layers_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ChangeTo
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::ChangeTo( Gl3dWidget* newWidget )
{
    view_  = newWidget;
    tools_ = newWidget;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ChangeTo
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::ChangeTo( GlWidget* newWidget )
{
    view_  = newWidget;
    tools_ = newWidget;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::RegisterTo
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::RegisterTo( Gl3dWidget* newWidget )
{
    for( IT_Layers it = layers_.begin(); it != layers_.end(); ++it )
        (*it)->RegisterIn( *newWidget );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::RegisterTo
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::RegisterTo( GlWidget* newWidget )
{
    for( IT_Layers it = layers_.begin(); it != layers_.end(); ++it )
        (*it)->RegisterIn( *newWidget );
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
// Name: GlProxy::Zoom
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
void GlProxy::Zoom( float width )
{
    view_->Zoom( width );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::UnSelect
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
boost::tuple< bool, bool, bool > GlProxy::UnSelect() const
{
    return tools_->UnSelect();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Select
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void GlProxy::Select( bool b1, bool b2, bool b3 ) const
{
    tools_->Select( b1, b2, b3 );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ShouldDisplay
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
bool GlProxy::ShouldDisplay( const std::string& name ) const
{
    return tools_->ShouldDisplay( name );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ShouldDisplay
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
bool GlProxy::ShouldDisplay( const std::string& name, bool autoCondition ) const
{
    return tools_->ShouldDisplay( name, autoCondition );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ShouldDisplay
// Created: LGY 2013-03-11
// -----------------------------------------------------------------------------
bool GlProxy::ShouldDisplay( E_LayerTypes type ) const
{
    if( tools_ )
        return tools_->ShouldDisplay( type );
    return false;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Pixels
// Created: AGE 2007-04-05
// -----------------------------------------------------------------------------
float GlProxy::Pixels( const geometry::Point2f& at /* = geometry::Point2f()*/ ) const
{
    return tools_->Pixels( at );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Zoom
// Created: RPD 2009-12-14
// -----------------------------------------------------------------------------
float GlProxy::Zoom() const
{
    return tools_->Zoom();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::GetAdaptiveZoomFactor
// Created: RPD 2009-12-14
// -----------------------------------------------------------------------------
float GlProxy::GetAdaptiveZoomFactor( bool bVariableSize /*= true*/ ) const
{
    return tools_->GetAdaptiveZoomFactor( bVariableSize );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::StipplePattern
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
unsigned short GlProxy::StipplePattern( int factor /* = 1*/ ) const
{
    return tools_->StipplePattern( factor );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::SetCurrentColor
// Created: AGE 2007-05-25
// -----------------------------------------------------------------------------
void GlProxy::SetCurrentColor( float r, float g, float b, float a /* = 1*/ )
{
    tools_->SetCurrentColor( r, g, b, a );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::SetCurrentCursor
// Created: SBO 2008-02-12
// -----------------------------------------------------------------------------
void GlProxy::SetCurrentCursor( const QCursor& cursor )
{
    if( tools_ )
        tools_->SetCurrentCursor( cursor );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawCross
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawCross( const geometry::Point2f& at, float size /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    tools_->DrawCross( at, size, unit );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawLine
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawLine( const geometry::Point2f& from, const geometry::Point2f& to, float width ) const
{
    tools_->DrawLine( from, to, width );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawStippledLine
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void GlProxy::DrawStippledLine( const geometry::Point2f& from, const geometry::Point2f& to ) const
{
    tools_->DrawStippledLine( from, to );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawLines
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawLines( const T_PointVector& points ) const
{
    tools_->DrawLines( points );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawRectangle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void GlProxy::DrawRectangle( const T_PointVector& points ) const
{
    tools_->DrawRectangle( points );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawPolygon
// Created: AGE 2007-05-23
// -----------------------------------------------------------------------------
void GlProxy::DrawPolygon( const T_PointVector& points ) const
{
    tools_->DrawPolygon( points );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawSelectedPolygon
// Created: JSR 2012-05-14
// -----------------------------------------------------------------------------
void GlProxy::DrawSelectedPolygon( const T_PointVector& points ) const
{
    tools_->DrawSelectedPolygon( points );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawDecoratedPolygon
// Created: SLG 2009-12-14
// -----------------------------------------------------------------------------
void GlProxy::DrawDecoratedPolygon( const geometry::Polygon2f& polygon, const std::string& name, unsigned int fontHeight )
{
    tools_->DrawDecoratedPolygon( polygon, name, fontHeight );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawArrow
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawArrow( const geometry::Point2f& from, const geometry::Point2f& to, float size /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    tools_->DrawArrow( from, to, size, unit );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawCurvedArrow
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawCurvedArrow( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio /* = 0.2f*/, float size /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    tools_->DrawCurvedArrow( from, to, curveRatio, size, unit );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawArc
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void GlProxy::DrawArc( const geometry::Point2f& center, const geometry::Point2f& p1, const geometry::Point2f& p2, float width ) const
{
    tools_->DrawArc( center, p1, p2, width );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawCircle
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawCircle( const geometry::Point2f& center, float radius /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    tools_->DrawCircle( center, radius, unit );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawDisc
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawDisc( const geometry::Point2f& center, float radius /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    tools_->DrawDisc( center, radius, unit );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawLife
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawLife( const geometry::Point2f& center, float height, float factor /* = 1.f*/, bool fixedSize /*= true */ ) const
{
    tools_->DrawLife( center, height, factor, fixedSize );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Print
// Created: AGE 2007-06-20
// -----------------------------------------------------------------------------
void GlProxy::Print( const std::string& message, const geometry::Point2f& where ) const
{
    tools_->Print( message, where );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Print
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::Print( const std::string& message, const geometry::Point2f& where, const QFont& font ) const
{
    tools_->Print( message, where, font );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawInfrastructureSymbol
// Created: LGY 2013-06-12
// -----------------------------------------------------------------------------
void GlProxy::DrawInfrastructureSymbol( const std::string& symbol, const geometry::Point2f& where, float factor, float thickness ) const
{
    tools_->DrawInfrastructureSymbol( symbol, where, factor, thickness );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawApp6Symbol
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawApp6Symbol( const std::string& symbol, const geometry::Point2f& where, float factor /* = 1.f*/, float thickness /* = 1.f*/, unsigned int direction /*= 0*/ ) const
{
    tools_->DrawApp6Symbol( symbol, where, factor, thickness, direction );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawApp6Symbol
// Created: AGE 2008-05-07
// -----------------------------------------------------------------------------
void GlProxy::DrawApp6Symbol( const std::string& symbol, const std::string& style, const geometry::Point2f& where, float factor /* = 1.f*/, float thickness /* = 1.f*/, unsigned int direction /*= 0*/ ) const
{
    tools_->DrawApp6Symbol( symbol, style, where, factor, thickness, direction );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawApp6SymbolFixedSize
// Created: MMC 2013-02-05
// -----------------------------------------------------------------------------
void GlProxy::DrawApp6SymbolFixedSize( const std::string& symbol, const geometry::Point2f& where, float factor, unsigned int direction ) const
{
    tools_->DrawApp6SymbolFixedSize( symbol, where, factor, direction );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawUnitSymbol
// Created: LDC 2013-04-09
// -----------------------------------------------------------------------------
void GlProxy::DrawUnitSymbol( const std::string& symbol, const std::string& moveSymbol, const std::string& staticSymbol, const std::string& level, bool isMoving, const geometry::Point2f& where, float factor, unsigned int direction, float width, float depth ) const
{
    tools_->DrawUnitSymbol( symbol, moveSymbol, staticSymbol, level, isMoving, where, factor, direction, width, depth );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawUnitSymbolAndTail
// Created: LDC 2013-04-26
// -----------------------------------------------------------------------------
void GlProxy::DrawUnitSymbolAndTail( const std::string& symbol, const std::string& level, const T_PointVector& points ) const
{
    tools_->DrawUnitSymbolAndTail( symbol, level, points );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawIcon
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void GlProxy::DrawIcon( const char** xpm, const geometry::Point2f& where, float size /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    tools_->DrawIcon( xpm, where, size, unit );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawImage
// Created: AGE 2006-11-17
// -----------------------------------------------------------------------------
void GlProxy::DrawImage( const QImage& image, const geometry::Point2f& where ) const
{
    tools_->DrawImage( image, where );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawCell
// Created: AGE 2006-12-01
// -----------------------------------------------------------------------------
void GlProxy::DrawCell( const geometry::Point2f& center ) const
{
    tools_->DrawCell( center );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawSvg
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void GlProxy::DrawSvg( const std::string& svg, const geometry::Point2f& center, float ratio /* = 1.f*/, bool fixedSize /*= true*/ ) const
{
    tools_->DrawSvg( svg, center, ratio, fixedSize );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawTacticalGraphics
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void GlProxy::DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, bool overlined, bool fixedSize /*= true*/ ) const
{
    tools_->DrawTacticalGraphics( symbol, location, overlined, fixedSize );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Reset2d
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void GlProxy::Reset2d()
{
    for( IT_Layers it = layers_.begin(); it != layers_.end(); ++it )
        (*it)->Reset2d();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Reset3d
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void GlProxy::Reset3d()
{
    for( IT_Layers it = layers_.begin(); it != layers_.end(); ++it )
        (*it)->Reset3d();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::CreateTooltip
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
std::auto_ptr< GlTooltip_ABC > GlProxy::CreateTooltip() const
{
    if( tooltipLayer_ )
        return std::auto_ptr< GlTooltip_ABC >( new GlTooltip( *tooltipLayer_ ) );
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
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void GlProxy::FillSelection( const geometry::Point2f& point, T_ObjectsPicking& selection )
{
    if( tools_ )
        tools_->FillSelection( point, selection );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::FillSelection
// Created: LGY 2013-03-11
// -----------------------------------------------------------------------------
void GlProxy::FillSelection( const geometry::Point2f& point, T_ObjectsPicking& selection, E_LayerTypes type )
{
    if( tools_ )
        tools_->FillSelection( point, selection, type );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::RenderPicking
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void GlProxy::RenderPicking( const T_ObjectPicking& object )
{
    tools_->RenderPicking( object );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::IsPickingMode
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
bool GlProxy::IsPickingMode() const
{
    return tools_->IsPickingMode();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Picking
// Created: LGY 2013-03-06
// -----------------------------------------------------------------------------
void GlProxy::Picking()
{
    tools_->Picking();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::LineWidth
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
float GlProxy::LineWidth( float base ) const
{
    return tools_->LineWidth( base );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::GetPickingColor
// Created: LGY 2013-03-15
// -----------------------------------------------------------------------------
QColor GlProxy::GetPickingColor() const
{
    return tools_->GetPickingColor();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::MapToterrainCoordinates
// Created: NPT 2013-06-12
// -----------------------------------------------------------------------------
geometry::Point2f GlProxy::MapToterrainCoordinates( int x, int y )
{
    return tools_->MapToterrainCoordinates( x, y );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::HasFocus
// Created: NPT 2013-06-13
// -----------------------------------------------------------------------------
bool GlProxy::HasFocus()
{
    return tools_->HasFocus();
}

