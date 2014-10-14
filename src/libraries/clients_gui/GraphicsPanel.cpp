// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GraphicsPanel.h"
#include "GraphicPreferences.h"
#include "clients_kernel/OptionsController.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GraphicsPanel constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicsPanel::GraphicsPanel( QWidget* parent, kernel::OptionsController& options,
                              const std::shared_ptr< TerrainSettings >& settings )
    : PreferencePanel_ABC( parent, "GraphicsPanel" )
    , options_( options )
    , preferences_( new GraphicPreferences( options, settings ) )
{
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout( widget );
    layout->addWidget( preferences_ );
    layout->addStretch( 1 );
    setWidget( widget );
}

// -----------------------------------------------------------------------------
// Name: GraphicsPanel destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicsPanel::~GraphicsPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GraphicsPanel::ActiveOptionsChanged
// Created: ABR 2014-08-01
// -----------------------------------------------------------------------------
void GraphicsPanel::Load( const GlProxy& )
{
    //preferences_->Load( view.GetActiveOptions() );
    preferences_->Load( *options_.GetViewOptions() );
}
