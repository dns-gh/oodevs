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
#include "CheckBox.h"
#include "Elevation2dLayer.h"
#include "GradientWidget.h"
#include "GradientPreferences.h"
#include "RichGroupBox.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/OptionsController.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ElevationPanel constructor
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
ElevationPanel::ElevationPanel( QWidget* parent, Elevation2dLayer& layer, kernel::Controllers& controllers,
                                const Painter_ABC& painter )
    : PreferencePanel_ABC( parent, "ElevationPanel" )
    , layer_              ( layer )
    , controllers_        ( controllers )
    , options_            ( controllers.options_ )
    , preferences_        ( *new GradientPreferences( controllers_.options_ ) )
    , enableHs_           ( true )
    , previousEnableHs_   ( true )
    , directionHs_        ( 315 )
    , previousDirectionHs_( 315 )
    , strengthHs_         ( 1 )
    , previousStrengthHs_ ( 1 )
{
    QVBoxLayout* container = new QVBoxLayout( this );
    container->setSizeConstraint( QLayout::SetMinimumSize );
    {
        fitColorGradienttoViewPort_ = new CheckBox( "fitColorGradienttoViewPort", tr( "Fit color gradient to viewport" ) );
        fitColorGradienttoViewPort_->setChecked( true );
        connect( fitColorGradienttoViewPort_, SIGNAL( toggled( bool ) ), SLOT( OnEnableVariableGradient( bool ) ) );

        gradient_ = new GradientWidget( 0, preferences_, controllers, painter );
        connect( fitColorGradienttoViewPort_, SIGNAL( toggled( bool ) ), gradient_, SLOT( OnEnableVariableGradient( bool ) ) );
        connect( gradient_, SIGNAL( GradientChanged( Gradient& ) ), SLOT( OnGradientChanged( Gradient& ) ) );

        RichGroupBox* gradientBox = new RichGroupBox( "gradientMap", tr( "Gradient map" ) );
        QVBoxLayout* gradientBoxLayout = new QVBoxLayout( gradientBox );
        gradientBoxLayout->addWidget( gradient_ );
        gradientBoxLayout->addStretch();

        RichGroupBox* box = new RichGroupBox( "elevationColor", tr( "Elevation colors" ) );
        QVBoxLayout* boxLayout = new QVBoxLayout( box );
        boxLayout->addWidget( fitColorGradienttoViewPort_, 1 );
        boxLayout->addWidget( gradientBox );
        boxLayout->addStretch();
        container->addWidget( box );
        container->addStretch();
    }
    {
        QLabel* directionLabel = new QLabel( tr( "Direction" ) );
        hsDial_ = new QDial( 0, 359, 1, directionHs_ );
        hsDial_->setObjectName( "hill-shade-direction" );
        hsDial_->setMaximumSize( 50, 50 );
        connect( hsDial_, SIGNAL( valueChanged( int ) ), SLOT( OnHillShadeDirection( int ) ) );

        QLabel* strengthLabel = new QLabel( tr( "Strength" ) );
        hillShadeStrength_ = new QSlider( 1, 50, 1, int( strengthHs_ ), Qt::Horizontal );
        hillShadeStrength_->setObjectName( "hill-shade-strength" );
        connect( hillShadeStrength_, SIGNAL( valueChanged( int ) ), SLOT( OnStrengthChanged( int ) ) );

        hsBox_ = new RichGroupBox( "hsBox", tr( "Hillshade" ) );
        connect( hsBox_, SIGNAL( toggled( bool ) ), SLOT( OnEnableHillshade( bool ) ) );
        hsBox_->setCheckable( true );
        hsBox_->setChecked( enableHs_ );
        QGridLayout* hsBoxLayout = new QGridLayout();
        hsBoxLayout->addWidget( hsDial_, 0, 1 );
        hsBoxLayout->addWidget( strengthLabel, 1, 0 );
        hsBoxLayout->addWidget( hillShadeStrength_, 1, 1 );
        hsBoxLayout->setColStretch( 1, 10 );
        container->addWidget( hsBox_ );

        QVBoxLayout* hsLayout = new QVBoxLayout( hsBox_ );
        hsLayout->addLayout( hsBoxLayout );
        hsLayout->addStretch();

        hsBoxLayout->addWidget( directionLabel, 0, 0 );
        container->addStretch( 1 );
    }
    setLayout( container );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel destructor
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
ElevationPanel::~ElevationPanel()
{
    controllers_.Unregister( *this );
    delete &preferences_;
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnGradientChanged
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void ElevationPanel::OnGradientChanged( Gradient& gradient )
{
    layer_.SetGradient( gradient );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::Commit
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void ElevationPanel::Commit()
{
    fitColorGradienttoViewPort_->Commit();
    gradient_->Commit();
    previousEnableHs_ = enableHs_;
    previousDirectionHs_ = directionHs_;
    previousStrengthHs_ = strengthHs_;
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::Reset
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void ElevationPanel::Reset()
{
    fitColorGradienttoViewPort_->Revert();
    gradient_->Reset();
    enableHs_ = previousEnableHs_;
    hsBox_->setChecked( enableHs_ );
    directionHs_ = previousDirectionHs_;
    hsDial_->setValue( directionHs_ );
    strengthHs_ = previousStrengthHs_;
    hillShadeStrength_->setValue( static_cast< int >( strengthHs_ ) );
    OnEnableHillshade( enableHs_ );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnEnableVariableGradient
// Created: AGE 2007-01-18
// -----------------------------------------------------------------------------
void ElevationPanel::OnEnableVariableGradient( bool bState )
{
    layer_.EnableVariableGradient( bState );
    options_.Change( "Elevation/FitToViewPort", bState );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnEnableHillshade
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void ElevationPanel::OnEnableHillshade( bool bState )
{
    enableHs_ = bState;
    layer_.SetHillShadeStrength( enableHs_ ? strengthHs_ : 0.f );
    options_.Change( "HillShade/Enabled", bState );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnHillShadeDirection
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void ElevationPanel::OnHillShadeDirection( int value )
{
    layer_.SetHillShadeDirection( directionHs_ = value );
    options_.Change( "HillShade/Direction", value );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnStrengthChanged
// Created: SBO 2007-07-20
// -----------------------------------------------------------------------------
void ElevationPanel::OnStrengthChanged( int value )
{
    strengthHs_ = pow( 1.1f, value );
    OnEnableHillshade( enableHs_ );
    options_.Change( "HillShade/Strength", value );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OptionChanged
// Created: SBO 2008-08-18
// -----------------------------------------------------------------------------
void ElevationPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    QString option ( name.c_str() );
    if( option == "Elevation/FitToViewPort" )
    {
        fitColorGradienttoViewPort_->setChecked( value.To< bool >() );
        return;
    }
    else if( option == "HillShade/Enabled" )
    {
        hsBox_->setChecked( value.To< bool >() );
        return;
    }
    else if( option == "HillShade/Direction" )
    {
        hsDial_->setValue( value.To< int >() );
        return;
    }
    else if( option == "HillShade/Strength" )
    {
        hillShadeStrength_->setValue( value.To< int >() );
        return;
    }
    else if( !option.startsWith( "Elevation/Gradients/" ) )
        return;
    option.remove( "Elevation/Gradients/" );
    preferences_.SetGradient( option, value.To< QString >() );
    gradient_->Reset(); // $$$$ SBO 2008-08-18: bof... notify Gradient creation/destruction instead
}
