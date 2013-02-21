// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UrbanLayer.h"
#include "preparation/UrbanHierarchies.h"
#include "preparation/UrbanModel.h"
#include "preparation/UrbanMenuManager.h"

// -----------------------------------------------------------------------------
// Name: UrbanLayer constructor
// Created: LGY 2012-01-06
// -----------------------------------------------------------------------------
UrbanLayer::UrbanLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                        gui::View_ABC& view, UrbanModel& model, const kernel::Profile_ABC& profile )
    : gui::UrbanLayer( controllers, tools, strategy, view, profile )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer destructor
// Created: LGY 2012-01-06
// -----------------------------------------------------------------------------
UrbanLayer::~UrbanLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::IsInSelection
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
bool UrbanLayer::IsInSelection( const kernel::Entity_ABC& entity, const geometry::Point2f& point ) const
{
    const UrbanHierarchies* hierarchies = static_cast< const UrbanHierarchies* >( entity.Retrieve< kernel::Hierarchies >() );
    if( hierarchies && hierarchies->GetLevel() == eUrbanLevelBlock )
        return gui::UrbanLayer::IsInSelection( entity, point );
    return false;
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::IsInside
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
bool UrbanLayer::IsInside( const kernel::Entity_ABC& entity, const geometry::Rectangle2f& rectangle ) const
{
    const UrbanHierarchies* hierarchies = static_cast< const UrbanHierarchies* >( entity.Retrieve< kernel::Hierarchies >() );
    if( hierarchies && hierarchies->GetLevel() == eUrbanLevelBlock )
        return gui::UrbanLayer::IsInside( entity, rectangle );
    return false;
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::HandleMousePress
// Created: LGY 2012-01-16
// -----------------------------------------------------------------------------
bool UrbanLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( controllers_.GetCurrentMode() == eModes_LivingArea )
        return false;
    return gui::UrbanLayer::HandleMousePress( event, point );
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::HandleKeyPress
// Created: JSR 2012-06-08
// -----------------------------------------------------------------------------
bool UrbanLayer::HandleKeyPress( QKeyEvent* key )
{
    if( key->key() != Qt::Key_Delete || ( controllers_.GetCurrentMode() != eModes_Terrain ) || actualSelection_.empty() )
        return false;
    model_.DeleteBlocks( actualSelection_ );
    return true;
}
