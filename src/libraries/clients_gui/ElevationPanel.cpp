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
        QGroupBox* box = new QGroupBox( 2, Qt::Horizontal, tr( "Elevation colors" ), this );
        
        new QLabel( tr( "Fit color gradient to viewport" ), box );
        QCheckBox* check = new QCheckBox( box );
        check->setChecked( true );

	    new QLabel( tr( "Gradient: " ), box );
    	gradient_ = new GradientWidget( box, preferences_, controllers );

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
        QHBox* buttonGroup = new QHBox( hsBox_ );
        QPushButton* minus = new QPushButton( "-", buttonGroup );
        QPushButton* plus  = new QPushButton( "+", buttonGroup );

        connect( hsBox_, SIGNAL( toggled( bool ) ), SLOT( OnEnableHillshade( bool ) ) );
        connect( hsDial_, SIGNAL( valueChanged( int ) ), SLOT( OnHillShadeDirection( int ) ) );
        connect( minus, SIGNAL( clicked() ), SLOT( OnMinus() ) );
        connect( plus,  SIGNAL( clicked() ), SLOT( OnPlus() ) );
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
// Name: ElevationPanel::OnMinus
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void ElevationPanel::OnMinus()
{
    strengthHs_ *= 0.9f;
    OnEnableHillshade( enableHs_ );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnPlus
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void ElevationPanel::OnPlus()
{
    strengthHs_ *= 1.1f;
    OnEnableHillshade( enableHs_ );
}
