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

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ElevationPanel constructor
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
ElevationPanel::ElevationPanel( QWidget* parent, Elevation2dLayer& layer )
    : PreferencePanel_ABC( parent )
    , layer_( layer )
    , enableHs_( true )
    , previousEnableHs_( true )
    , directionHs_( 315 )
    , previousDirectionHs_( 315 )
    , strengthHs_( 1 )
    , previousStrengthHs_( 1 )
{
    {
        QGroupBox* box = new QGroupBox( 2, Qt::Horizontal, tr( "Elevation colors" ), this );
        
        new QLabel( tr( "Highest heights color: " ), box );
        max_ = new ColorButton( box, "", black );
        
        new QLabel( tr( "Lowest heights color: " ), box );
        min_ = new ColorButton( box, "", white );

        new QLabel( tr( "Fit color gradient to viewport" ), box );
        QCheckBox* check = new QCheckBox( box );
        check->setChecked( true );

        connect( check, SIGNAL( toggled( bool ) ), SLOT( OnEnableVariable( bool ) ) );
        connect( min_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorChanged() ) );
        connect( max_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorChanged() ) );
    }
    {
        hsBox_ = new QGroupBox( 2, Qt::Horizontal, tr( "Hillshade" ), this );
        hsBox_->setCheckable( true );
        hsBox_->setChecked( enableHs_ );
        new QLabel( tr( "Direction" ), hsBox_ );
        hsDial_ = new QDial( 0, 359, 1, directionHs_, hsBox_ );

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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnColorChanged
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void ElevationPanel::OnColorChanged()
{
    Gradient gradient;
    gradient.AddColor( 0, min_->GetColor() );
    gradient.AddColor( 1, max_->GetColor() );
    layer_.SetGradient( gradient );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::Commit
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void ElevationPanel::Commit()
{
    min_->Commit();
    max_->Commit();
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
    min_->Revert();
    max_->Revert();
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
