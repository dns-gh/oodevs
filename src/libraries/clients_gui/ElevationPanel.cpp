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
#include "Elevation2dLayer.h"
#include "GradientWidget.h"
#include "GradientPreferences.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ElevationPanel constructor
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
ElevationPanel::ElevationPanel( QWidget* parent, Elevation2dLayer& layer, kernel::Controllers& controllers )
    : PreferencePanel_ABC( parent )
    , layer_( layer )
    , preferences_( *new GradientPreferences() )
    , enableHs_( true )
    , previousEnableHs_( true )
    , directionHs_( 315 )
    , previousDirectionHs_( 315 )
    , strengthHs_( 1 )
    , previousStrengthHs_( 1 )
{
    {
        QGroupBox* box = new QGroupBox( 1, Qt::Horizontal, tr( "Elevation colors" ), this );
        
        QHBox* hBox = new QHBox( box );
        new QLabel( tr( "Fit color gradient to viewport" ), hBox );
        QCheckBox* check = new QCheckBox( hBox );
        check->setChecked( true );

        gradient_ = new GradientWidget( new QGroupBox( 1, Qt::Horizontal, tr( "Gradient map" ), box ), preferences_, controllers );

        connect( check, SIGNAL( toggled( bool ) ), SLOT( OnEnableVariable( bool ) ) );
        connect( gradient_, SIGNAL( GradientChanged( const Gradient& ) ), SLOT( OnGradientChanged( const Gradient& ) ) );
    }
    {
        hsBox_ = new QGroupBox( 2, Qt::Horizontal, tr( "Hillshade" ), this );
        hsBox_->setCheckable( true );
        hsBox_->setChecked( enableHs_ );
        new QLabel( tr( "Direction" ), hsBox_ );
        hsDial_ = new QDial( 0, 359, 1, directionHs_, hsBox_ );
        hsDial_->setMaximumSize( 50, 50 );

        new QLabel( tr( "Strength" ), hsBox_ );
        QSlider* slider = new QSlider( 1, 50, 1, 1, Qt::Horizontal, hsBox_ );

        connect( hsBox_, SIGNAL( toggled( bool ) ), SLOT( OnEnableHillshade( bool ) ) );
        connect( hsDial_, SIGNAL( valueChanged( int ) ), SLOT( OnHillShadeDirection( int ) ) );
        connect( slider, SIGNAL( valueChanged( int ) ), SLOT( OnStrengthChanged( int ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel destructor
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
ElevationPanel::~ElevationPanel()
{
    delete &preferences_;
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnGradientChanged
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void ElevationPanel::OnGradientChanged( const Gradient& gradient )
{
    layer_.SetGradient( gradient );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::Commit
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void ElevationPanel::Commit()
{
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
    gradient_->Reset();
    enableHs_    = previousEnableHs_;    hsBox_->setChecked( enableHs_ );
    directionHs_ = previousDirectionHs_; hsDial_->setValue( directionHs_ );
    strengthHs_  = previousStrengthHs_;  OnEnableHillshade( enableHs_ );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnEnableVariable
// Created: AGE 2007-01-18
// -----------------------------------------------------------------------------
void ElevationPanel::OnEnableVariable( bool bDenis )
{
    layer_.EnableVariableGradient( bDenis );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnEnableHillshade
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void ElevationPanel::OnEnableHillshade( bool bDummy )
{
    enableHs_ = bDummy;
    layer_.SetHillShadeStrength( enableHs_ ? strengthHs_ : 0.f );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnHillShadeDirection
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void ElevationPanel::OnHillShadeDirection( int value )
{
    layer_.SetHillShadeDirection( directionHs_ = value );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnStrengthChanged
// Created: SBO 2007-07-20
// -----------------------------------------------------------------------------
void ElevationPanel::OnStrengthChanged( int value )
{
    strengthHs_ = pow( 1.1f, value );
    OnEnableHillshade( enableHs_ );
}
