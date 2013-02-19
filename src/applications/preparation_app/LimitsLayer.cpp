// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LimitsLayer.h"
#include "moc_LimitsLayer.cpp"
#include "ModelBuilder.h"
#include "preparation/TacticalLinePositions.h"
#include "clients_gui/ValuedDragObject.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/ModeController_ABC.h"
#include "clients_kernel/Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LimitsLayer constructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
LimitsLayer::LimitsLayer( Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                          ParametersLayer& parameters, ModelBuilder& modelBuilder, gui::View_ABC& view, const kernel::Profile_ABC& profile )
    : TacticalLinesLayer( controllers, tools, strategy, parameters, view, profile )
    , modelBuilder_( modelBuilder )
    , tools_( tools )
    , dummy_( new QWidget() )
{
    controllers.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer destructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
LimitsLayer::~LimitsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::CanCreateLine
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
bool LimitsLayer::CanCreateLine()
{
    if( controllers_.modes_ && controllers_.modes_->GetCurrentMode() == ePreparationMode_LivingArea )
        return false;
    return modelBuilder_.CanCreateLine();
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Delete
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LimitsLayer::Delete( const kernel::TacticalLine_ABC& line )
{
    modelBuilder_.DeleteEntity( line );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::CreateLimit
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LimitsLayer::CreateLimit( const T_PointVector& points )
{
    modelBuilder_.CreateLimit( points );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::CreateLima
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LimitsLayer::CreateLima( const T_PointVector& points )
{
    modelBuilder_.CreateLima( points );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Precision
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
float LimitsLayer::Precision( const geometry::Point2f& point ) const
{
    return 5.f * tools_.Pixels( point );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::ShouldDisplay
// Created: LGY 2012-01-03
// -----------------------------------------------------------------------------
bool LimitsLayer::ShouldDisplay( const kernel::Entity_ABC& entity )
{
    if( !TacticalLinesLayer::ShouldDisplay( entity ) )
        return false;
    return drawLines_.IsSet( true, true );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::OptionChanged
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
void LimitsLayer::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "TacticalLines" )
        drawLines_ = value.To< FourStateOption >();
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::NotifyContextMenu
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
void LimitsLayer::NotifyContextMenu( const kernel::TacticalLine_ABC& /* line */, kernel::ContextMenu& menu )
{
    menu.InsertItem( "Creation", tools::translate( "LimitsLayer", "Edit..." ), this, SLOT( OnEdit() ) );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::OnEdit
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
void LimitsLayer::OnEdit()
{
    if( selected_ )
        static_cast< TacticalLinePositions& >( const_cast< kernel::Positions& >( selected_->Get< kernel::Positions >() ) ).Edit( parameters_ );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::CanDrop
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
bool LimitsLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& ) const
{
    if( !selected_ )
        return false;
    if( gui::ValuedDragObject::Provides< const TacticalLinePositions >( event ) )
        return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::HandleMoveDragEvent
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
bool LimitsLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    if( TacticalLinePositions* position = gui::ValuedDragObject::GetValue< TacticalLinePositions >( event ) )
    {
        if( !selected_ )
            return false;
        if( draggingPoint_.Distance( point ) >= 5.f * tools_.Pixels( point ) )
        {
            const geometry::Vector2f translation( draggingPoint_, point );
            const geometry::Rectangle2f boundingBox = position->GetBoundingBox() + translation;
            if( boundingBox.Intersect( world_ ) == boundingBox )
            {
                position->Translate( draggingPoint_, translation, 5.f * tools_.Pixels( point ) );
                draggingPoint_ = point;
            }
        }
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::HandleDropEvent
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool LimitsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& )
{
    if( selected_ && gui::ValuedDragObject::GetValue< TacticalLinePositions >( event ) )
    {
        draggingPoint_.Set( 0, 0 );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::HandleMousePress
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool LimitsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( event->button() == Qt::LeftButton && event->buttons() != Qt::NoButton && IsEligibleForDrag( point ) )
        if( const TacticalLinePositions* pos = static_cast< const TacticalLinePositions* >( selected_->Retrieve< kernel::Positions >() ) )
        {
            draggingPoint_ = point;
            Q3DragObject* drag = new gui::ValuedDragObject( pos, dynamic_cast< QWidget* >( dummy_.get() ) );
            drag->dragMove();
        }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::IsEligibleForDrag
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
bool LimitsLayer::IsEligibleForDrag( const geometry::Point2f& point ) const
{
    return selected_ && IsInSelection( *selected_, point );
}
