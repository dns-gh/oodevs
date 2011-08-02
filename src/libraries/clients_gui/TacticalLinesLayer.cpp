// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::TacticalLinesLayer */

#include "clients_gui_pch.h"
#include "TacticalLinesLayer.h"
#include "moc_TacticalLinesLayer.cpp"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "tools.h"
#include "ParametersLayer.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer constructor
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
TacticalLinesLayer::TacticalLinesLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                                        ParametersLayer& parameters, View_ABC& view, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter )
    : EntityLayer< kernel::TacticalLine_ABC >( controllers, tools, strategy, view, profile, filter )
    , controllers_ ( controllers )
    , tools_       ( tools )
    , strategy_    ( strategy )
    , parameters_  ( parameters )
    , isLimit_     ( true )
    , selected_    ( controllers_ )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer destructor
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
TacticalLinesLayer::~TacticalLinesLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::IsInSelection
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
bool TacticalLinesLayer::IsInSelection( const kernel::Entity_ABC& entity, const geometry::Point2f& point ) const
{
    return entity.Get< Positions >().IsAt( point, 10.f * tools_.Pixels( point ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::HandleKeyPress
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
bool TacticalLinesLayer::HandleKeyPress( QKeyEvent* k )
{
    if( !selected_ )
        return false;
    const int key = k->key();
    if( key == Qt::Key_Backspace || key == Qt::Key_Delete )
    {
        Delete( *selected_ );
        selected_ = 0;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::NotifyContextMenu
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void TacticalLinesLayer::NotifyContextMenu( const kernel::Nothing&, kernel::ContextMenu& menu )
{
    if( CanCreateLine() )
    {
        menu.InsertItem( "Creation", tools::translate( "gui::TacticalLinesLayer", "Create limit" ),       this, SLOT( OnCreateLimit() ) );
        menu.InsertItem( "Creation", tools::translate( "gui::TacticalLinesLayer", "Create phase line"  ), this, SLOT( OnCreateLima() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::ContextMenu
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void TacticalLinesLayer::ContextMenu( const kernel::Entity_ABC& entity, const geometry::Point2f& point, const QPoint& where )
{
    const kernel::TacticalLine_ABC& line = static_cast< const kernel::TacticalLine_ABC& >( entity );
    controllers_.actions_.ContextMenu( entity, line, point, where );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::CanCreateLine
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
bool TacticalLinesLayer::CanCreateLine()
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::NotifySelected
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void TacticalLinesLayer::NotifySelected( const kernel::TacticalLine_ABC* element )
{
    selected_ = element;
    gui::EntityLayer< kernel::TacticalLine_ABC >::NotifySelected( element );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::Handle
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void TacticalLinesLayer::Handle( kernel::Location_ABC& location )
{
    if( location.IsValid() )
        location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::ShouldDisplay
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
bool TacticalLinesLayer::ShouldDisplay( const kernel::Entity_ABC& )
{
    return drawLines_.IsSet( true, true );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::OptionChanged
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void TacticalLinesLayer::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "TacticalLines" )
        drawLines_ = value.To< FourStateOption >();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::VisitLines
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void TacticalLinesLayer::VisitLines( const T_PointVector& points )
{
    if( isLimit_ )
        CreateLimit( points );
    else
        CreateLima( points );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::OnCreateLimit
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void TacticalLinesLayer::OnCreateLimit()
{
    isLimit_ = true;
    parameters_.StartLine( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::OnCreateLima
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void TacticalLinesLayer::OnCreateLima()
{
    isLimit_ = false;
    parameters_.StartLine( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::HandleMouseMove
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
bool TacticalLinesLayer::HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point )
{
    return ( selected_ != 0 && MouseMove( *selected_.ConstCast(), mouse, point ) ) || EntityLayer< kernel::TacticalLine_ABC >::HandleMouseMove( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::HandleMousePress
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
bool TacticalLinesLayer::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    return ( selected_ != 0 && MousePress( *selected_.ConstCast(), mouse, point ) ) || EntityLayer< kernel::TacticalLine_ABC >::HandleMousePress( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::HandleMouseDoubleClick
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
bool TacticalLinesLayer::HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point )
{
    return ( selected_ != 0 && MouseDoubleClick( *selected_.ConstCast(), mouse, point ) ) || EntityLayer< kernel::TacticalLine_ABC >::HandleMouseDoubleClick( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::MouseMove
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
bool TacticalLinesLayer::MouseMove( kernel::TacticalLine_ABC& /*entity*/, QMouseEvent* /*mouse*/, const geometry::Point2f& /*point*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::MousePress
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
bool TacticalLinesLayer::MousePress( kernel::TacticalLine_ABC& /*entity*/, QMouseEvent* /*mouse*/, const geometry::Point2f& /*point*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::MouseDoubleClick
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
bool TacticalLinesLayer::MouseDoubleClick( kernel::TacticalLine_ABC& /*entity*/, QMouseEvent* /*mouse*/, const geometry::Point2f& /*point*/ )
{
    return false;
}
