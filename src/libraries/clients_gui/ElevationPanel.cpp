// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ElevationPanel.h"
#include "moc_ElevationPanel.cpp"
#include "GLOptions.h"
#include "GlProxy.h"
#include "GradientPreferencesEditor.h"
#include "OptionWidgets.h"
#include "SignalAdapter.h"
#include "Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/OptionsController.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ElevationPanel constructor
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
ElevationPanel::ElevationPanel( QWidget* parent,
                                kernel::OptionsController& options,
                                const kernel::DetectionMap& detection )
    : PreferencePanel_ABC( parent, "ElevationPanel" )
{
    gradientEditor_ = new GradientPreferencesEditor( options, detection, "elevation-gradient" );

    auto hsBox = new OptionGroupBox( options, "hill-shade-enabled", "HillShade/Enabled", tr( "Hillshade" ) );
    auto hsDial = new OptionDial( options, "hill-shade-direction", "HillShade/Direction", 0, 359 );
    hsDial->setMaximumSize( 50, 50 );
    QVBoxLayout* hsLayout = new QVBoxLayout( hsBox );
    hsLayout->addWidget( tools::AddLabeledWidget( tr( "Direction" ), hsDial ) );
    auto hsSlider = new OptionSlider( options, "hill-shade-strength", "HillShade/Strength", 1, 50 );
    hsLayout->addWidget( tools::AddLabeledWidget( tr( "Strength" ), hsSlider ) );

    QVBoxLayout* container = new QVBoxLayout( this );
    container->addWidget( tools::AddGroupBoxWidget( tr( "Elevation colors" ), "elevation-groupbox", gradientEditor_ ) );
    container->addWidget( hsBox );
    container->addStretch( 1 );
    setLayout( container );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel destructor
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
ElevationPanel::~ElevationPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::Load
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
void ElevationPanel::Load( const GlProxy& view )
{
    gradientEditor_->SetElevation2dTexture( view.GetOptions().GetElevation2dTexture() );
}
