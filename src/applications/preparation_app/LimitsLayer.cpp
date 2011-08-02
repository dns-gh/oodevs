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
#include "ModelBuilder.h"
#include "preparation/TacticalLinePositions.h"
#include "clients_gui/ExclusiveEventStrategy.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LimitsLayer constructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
LimitsLayer::LimitsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                          ParametersLayer& parameters, ModelBuilder& modelBuilder, gui::View_ABC& view,
                          gui::ExclusiveEventStrategy& eventStrategy, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter )
    : TacticalLinesLayer( controllers, tools, strategy, parameters, view, profile, filter )
    , modelBuilder_( modelBuilder )
    , tools_( tools )
    , eventStrategy_( eventStrategy )
{
    // NOTHING
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
    return modelBuilder_.CanCreateLine();
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Delete
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LimitsLayer::Delete( const kernel::TacticalLine_ABC& line )
{
    modelBuilder_.DeleteEntity( line );
    eventStrategy_.ReleaseExclusiveFocus();
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::ShouldDisplay
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
bool LimitsLayer::ShouldDisplay( const kernel::Entity_ABC& )
{
    return true;
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
// Name: LimitsLayer::MouseMove
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
bool LimitsLayer::MouseMove( kernel::TacticalLine_ABC& entity, QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( mouse->state() == Qt::LeftButton && ! dragPoint_.IsZero() )
    {
        const geometry::Vector2f translation( dragPoint_, point );
        static_cast< TacticalLinePositions* >( &entity.Get< Positions >() )->Translate( dragPoint_, translation, Precision( point ) );
        dragPoint_ = point;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::MousePress
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
bool LimitsLayer::MousePress( kernel::TacticalLine_ABC& entity, QMouseEvent* mouse, const geometry::Point2f& point )
{
    const float precision = Precision( point );
    if( mouse->state() == Qt::ShiftModifier )
        static_cast< TacticalLinePositions* >( &entity.Get< Positions >() )->InsertPoint( point, precision );
    else if( mouse->state() == Qt::AltModifier )
        static_cast< TacticalLinePositions* >( &entity.Get< Positions >() )->RemovePoint( point, precision );

    if( entity.Get< Positions >().IsAt( point, precision ) )
    {
        eventStrategy_.TakeExclusiveFocus( *this );
        if( mouse->button() == Qt::LeftButton && mouse->state() == Qt::NoButton )
            dragPoint_ = point;
        else
            dragPoint_ = geometry::Point2f();
        return true;
    }
    eventStrategy_.ReleaseExclusiveFocus();
    return false;
}
