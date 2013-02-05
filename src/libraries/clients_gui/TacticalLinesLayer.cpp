// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

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
TacticalLinesLayer::TacticalLinesLayer( kernel::Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                                        ParametersLayer& parameters, View_ABC& view, const kernel::Profile_ABC& profile )
    : EntityLayer< kernel::TacticalLine_ABC >( controllers, tools, strategy, view, profile, tr( "Tactical lines" ) )
    , controllers_ ( controllers )
    , tools_       ( tools )
    , strategy_    ( strategy )
    , parameters_  ( parameters )
    , isLimit_     ( true )
    , isEditing_   ( false )
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
    return entity.Get< Positions >().IsAt( point, 10.f * tools_.Pixels( point ), tools_.GetAdaptiveZoomFactor() );
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
void TacticalLinesLayer::ContextMenu( const Selectable_ABC& selectable, const geometry::Point2f& point, const QPoint& where )
{
    const Entity_ABC& entity = static_cast< const Entity_ABC& >( selectable );
    const TacticalLine_ABC& line = static_cast< const TacticalLine_ABC& >( entity );
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
// Name: TacticalLinesLayer::NotifySelectionChanged
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void TacticalLinesLayer::NotifySelectionChanged( const std::vector< const kernel::TacticalLine_ABC* >& elements )
{
    const kernel::TacticalLine_ABC* newSelected = elements.size() == 1 ? elements.front() : 0;
    if( isEditing_ && ( !newSelected || newSelected->GetId() != selected_->GetId() ) )
    {
        parameters_.Reset();
        isEditing_ = false;
    }
    selected_ = newSelected;
    gui::EntityLayer< kernel::TacticalLine_ABC >::NotifySelectionChanged( elements );
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
// Name: TacticalLinesLayer::VisitLines
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void TacticalLinesLayer::VisitLines( const T_PointVector& points )
{
    if( isLimit_ )
        CreateLimit( points );
    else
        CreateLima( points );
    isEditing_ = false;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::OnCreateLimit
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void TacticalLinesLayer::OnCreateLimit()
{
    isLimit_ = true;
    isEditing_ = true;
    parameters_.StartLine( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesLayer::OnCreateLima
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void TacticalLinesLayer::OnCreateLima()
{
    isLimit_ = false;
    isEditing_ = true;
    parameters_.StartLine( *this );
}
