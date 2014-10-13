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
#include "Elevation2dLayer.h"
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
                                const kernel::DetectionMap& detection,
                                const std::shared_ptr< Elevation2dLayer >& elevation2dLayer,
                                const std::shared_ptr< GradientPreferences >& preferences )
    : PreferencePanel_ABC( parent, "ElevationPanel" )
    , options_( options )
    , layer_( elevation2dLayer )
{
    gradient_ = new GradientPreferencesEditor( options, detection, preferences, "elevation-gradient" );

    auto hsBox = new OptionGroupBox( options, "hill-shade-enabled", "HillShade/Enabled", tr( "Hillshade" ) );
    auto hsDial = new OptionDial( options, "hill-shade-direction", "HillShade/Direction", 0, 359 );
    hsDial->setMaximumSize( 50, 50 );
    QVBoxLayout* hsLayout = new QVBoxLayout( hsBox );
    hsLayout->addWidget( tools::AddLabeledWidget( tr( "Direction" ), hsDial ) );
    auto hsSlider = new OptionSlider( options, "hill-shade-strength", "HillShade/Strength", 1, 50 );
    hsLayout->addWidget( tools::AddLabeledWidget( tr( "Strength" ), hsSlider ) );

    QVBoxLayout* container = new QVBoxLayout( this );
    container->addWidget( tools::AddGroupBoxWidget( tr( "Elevation colors" ), "elevation-groupbox", gradient_ ) );
    container->addWidget( hsBox );
    container->addStretch( 1 );
    setLayout( container );

    connect( gradient_, SIGNAL( FitToViewportChanged( int ) ), SLOT( OnFitToViewPortChanged( int ) ) );
    connect( gradient_, SIGNAL( UpdateGradient() ), SLOT( OnGradientUpdated() ) );
    connect( hsBox, SIGNAL( clicked( bool ) ), SLOT( OnEnableHillshade( bool ) ) );
    connect( hsDial, SIGNAL( valueChanged( int ) ), SLOT( OnHillShadeDirection( int ) ) );
    connect( hsSlider, SIGNAL( valueChanged( int ) ), SLOT( OnStrengthChanged( int ) ) );
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
void ElevationPanel::Load( const GlProxy& )
{
    gradient_->Load();
    // gradient_->SetElevation2dTexture( view.GetOptions().GetElevation2dTexture() );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnGradientUpdated
// Created: ABR 2014-10-08
// -----------------------------------------------------------------------------
void ElevationPanel::OnGradientUpdated()
{
    layer_->UpdateGradient();
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnFitToViewPortChanged
// Created: AGE 2007-01-18
// -----------------------------------------------------------------------------
void ElevationPanel::OnFitToViewPortChanged( int bState )
{
    layer_->EnableVariableGradient( bState == Qt::Checked );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnEnableHillshade
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void ElevationPanel::OnEnableHillshade( bool bState )
{
    layer_->EnableHillShade( bState );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnHillShadeDirection
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void ElevationPanel::OnHillShadeDirection( int value )
{
    layer_->SetHillShadeDirection( value );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnStrengthChanged
// Created: SBO 2007-07-20
// -----------------------------------------------------------------------------
void ElevationPanel::OnStrengthChanged( int value )
{
    layer_->SetHillShadeStrength( pow( 1.1f, value ) );
}
