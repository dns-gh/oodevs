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
{
    QGroupBox* box = new QGroupBox( 2, Qt::Horizontal, tr( "Elevation colors" ), this );
    
    new QLabel( tr( "Highest heights color: " ), box );
    max_ = new ColorButton( box, "", black );
    
    new QLabel( tr( "Lowest heights color: " ), box );
    min_ = new ColorButton( box, "", white );

    new QLabel( tr( "Fit color gradient to viewport" ), box );
    QCheckBox* check = new QCheckBox( box );
    check->setChecked( true );

    connect( check, SIGNAL( toggled( bool ) ), this, SLOT( OnEnableVariable( bool ) ) );
    connect( min_, SIGNAL( ColorChanged( const QColor& ) ), this, SLOT( OnColorChanged() ) );
    connect( max_, SIGNAL( ColorChanged( const QColor& ) ), this, SLOT( OnColorChanged() ) );
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
    layer_.SetColors( min_->GetColor(), max_->GetColor() );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::Commit
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void ElevationPanel::Commit()
{
    min_->Commit();
    max_->Commit();
    layer_.SetColors( min_->GetColor(), max_->GetColor() );
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::Reset
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void ElevationPanel::Reset()
{
    min_->Revert();
    max_->Revert();
}

// -----------------------------------------------------------------------------
// Name: ElevationPanel::OnEnableVariable
// Created: AGE 2007-01-18
// -----------------------------------------------------------------------------
void ElevationPanel::OnEnableVariable( bool bDenis )
{
    layer_.EnableVariableGradient( bDenis );
}
