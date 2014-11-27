// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "DrawerPanel.h"
#include "clients_gui/DrawerModel.h"

// -----------------------------------------------------------------------------
// Name: DrawerPanel constructor
// Created: JSR 2014-11-26
// -----------------------------------------------------------------------------
DrawerPanel::DrawerPanel( QWidget* parent,
                          gui::PanelStack_ABC& panel,
                          const std::shared_ptr< gui::ParametersLayer >& layer,
                          kernel::Controllers& controllers,
                          gui::DrawerModel& model,
                          gui::DrawingTypes& types,
                          const tools::ExerciseConfig& config,
                          const kernel::Profile_ABC& profile )
    : gui::DrawerPanel( parent, panel, layer, controllers, model, types, config, profile )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel destructor
// Created: JSR 2014-11-26
// -----------------------------------------------------------------------------
DrawerPanel::~DrawerPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::DoClear
// Created: JSR 2014-11-26
// -----------------------------------------------------------------------------
void DrawerPanel::DoClear() const
{
    model_.Apply( [&]( const kernel::Drawing_ABC& drawing ) {
        if( drawing.IsControlledBy( profile_ ) )
            drawing.NotifyDestruction();
    });
}
