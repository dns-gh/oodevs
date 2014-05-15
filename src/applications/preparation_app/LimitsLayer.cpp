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
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/OptionVariant.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LimitsLayer constructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
LimitsLayer::LimitsLayer( Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                          ParametersLayer& parameters, ModelBuilder& modelBuilder, gui::View_ABC& view, const kernel::Profile_ABC& profile )
    : TacticalLinesLayer( controllers, tools, strategy, parameters, view, profile, modelBuilder )
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
    if( controllers_.GetCurrentMode() == eModes_LivingArea )
        return false;
    return modelBuilder_.CanCreateLine();
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
    return drawLines_.IsSet( true, true, true );
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
    menu.InsertItem( "Creation", tr( "Edit..." ), this, SLOT( OnEdit() ) );
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
    return dnd::HasData< TacticalLinePositions >( event );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::HandleMoveDragEvent
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
bool LimitsLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    if( dnd::HasData< TacticalLinePositions >( event ) && selected_ )
    {
        if( TacticalLinePositions* position = static_cast< TacticalLinePositions* >( selected_.ConstCast()->Retrieve< kernel::Positions >() ) )
        {
            const geometry::Vector2f translation( dragPoint_, point );
            const geometry::Rectangle2f newBoundingBox = position->GetBoundingBox() + translation;
            if( newBoundingBox.Intersect( world_ ) == newBoundingBox )
            {
                position->Translate( dragPoint_, translation, Precision( point ) );
                dragPoint_ = point;
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
    if( dnd::HasData< TacticalLinePositions >( event ) && selected_ )
    {
        dragPoint_.Set( 0, 0 );
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
    if( event->button() == Qt::LeftButton && event->buttons() != Qt::NoButton && IsEligibleForDrag() )
        if( const TacticalLinePositions* pos = static_cast< const TacticalLinePositions* >( selected_->Retrieve< kernel::Positions >() ) )
        {
            dragPoint_ = point;
            dnd::CreateDragObject( pos, dummy_.get() );
        }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::IsEligibleForDrag
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
bool LimitsLayer::IsEligibleForDrag() const
{
    return selected_ && EntityLayerBase::IsInSelection( *selected_ );
}
