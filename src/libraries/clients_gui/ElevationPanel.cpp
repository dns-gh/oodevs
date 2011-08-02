// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::ElevationPanel */

#include "clients_gui_pch.h"
#include "ElevationPanel.h"
#include "moc_ElevationPanel.cpp"
#include "ColorButton.h"
#include "CheckBox.h"
#include "Elevation2dLayer.h"
#include "GradientWidget.h"
#include "GradientPreferences.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Options.h"

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
    Q3VBox* container = new Q3VBox( this );
    container->setAutoFillBackground( true );
    {
        Q3GroupBox* box = new Q3GroupBox( 1, Qt::Horizontal, tr( "Elevation colors" ), container );

        Q3HBox* hBox = new Q3HBox( box );
        fitColorGradienttoViewPort_ = new CheckBox( tr( "Fit color gradient to viewport" ), hBox );
        fitColorGradienttoViewPort_->setChecked( true );

        gradient_ = new GradientWidget( new Q3GroupBox( 1, Qt::Horizontal, tr( "Gradient map" ), box ), preferences_, controllers, painter );

        connect( fitColorGradienttoViewPort_, SIGNAL( toggled( bool ) ), SLOT( OnEnableVariableGradient( bool ) ) );
        connect( fitColorGradienttoViewPort_, SIGNAL( toggled( bool ) ), gradient_, SLOT( OnEnableVariableGradient( bool ) ) );
        connect( gradient_, SIGNAL( GradientChanged( Gradient& ) ), SLOT( OnGradientChanged( Gradient& ) ) );
    }
    {
        hsBox_ = new Q3GroupBox( 2, Qt::Horizontal, tr( "Hillshade" ), container );
        hsBox_->setCheckable( true );
        hsBox_->setChecked( enableHs_ );
        new QLabel( tr( "Direction" ), hsBox_ );
        hsDial_ = new QDial( 0, 359, 1, directionHs_, hsBox_ );
        hsDial_->setMaximumSize( 50, 50 );

        new QLabel( tr( "Strength" ), hsBox_ );
        hillShadeStrength_ = new QSlider( 1, 50, 1, int( strengthHs_ ), Qt::Horizontal, hsBox_ );

        connect( hsBox_, SIGNAL( toggled( bool ) ), SLOT( OnEnableHillshade( bool ) ) );
        connect( hsDial_, SIGNAL( valueChanged( int ) ), SLOT( OnHillShadeDirection( int ) ) );
        connect( hillShadeStrength_, SIGNAL( valueChanged( int ) ), SLOT( OnStrengthChanged( int ) ) );
    }
    setWidget( container );
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
    options_.Change( "FitColorGradienttoViewPort", bState );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnEnableHillshade
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void ElevationPanel::OnEnableHillshade( bool bState )
{
    enableHs_ = bState;
    layer_.SetHillShadeStrength( enableHs_ ? strengthHs_ : 0.f );
    options_.Change( "EnableHillShade", bState );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnHillShadeDirection
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void ElevationPanel::OnHillShadeDirection( int value )
{
    layer_.SetHillShadeDirection( directionHs_ = value );
    options_.Change( "HillShadeDirection", value );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnStrengthChanged
// Created: SBO 2007-07-20
// -----------------------------------------------------------------------------
void ElevationPanel::OnStrengthChanged( int value )
{
    strengthHs_ = pow( 1.1f, value );
    OnEnableHillshade( enableHs_ );
    options_.Change( "HillShadeStrength", value );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OptionChanged
// Created: SBO 2008-08-18
// -----------------------------------------------------------------------------
void ElevationPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    QString option ( name.c_str() );
    if( option == "FitColorGradienttoViewPort" )
    {
        fitColorGradienttoViewPort_->setChecked( value.To< bool >() );
        return;
    }
    else if( option == "EnableHillShade" )
    {
        hsBox_->setChecked( value.To< bool >() );
        return;
    }
    else if( option == "HillShadeDirection" )
    {
        hsDial_->setValue( value.To< int >() );
        return;
    }
    else if( option == "HillShadeStrength" )
    {
        hillShadeStrength_->setValue( value.To< int >() );
        return;
    }
    else if( !option.startsWith( "Gradients/" ) )
        return;
    option.remove( "Gradients/" );
    preferences_.SetGradient( option, value.To< QString >() );
    gradient_->Reset(); // $$$$ SBO 2008-08-18: bof... notify Gradient creation/destruction instead
}
