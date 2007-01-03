// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Graphics3dPanel.h"
#include "moc_Graphics3dPanel.cpp"
#include "ColorButton.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Graphics3dPanel constructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
Graphics3dPanel::Graphics3dPanel( QWidget* parent )
    : PreferencePanel_ABC( parent )
{
    type_ = new QButtonGroup( 2, Qt::Horizontal, tr( "Display type" ), this );
    type_->insert( new QRadioButton( tr( "USRP" ), type_ ) );
    type_->insert( new QRadioButton( tr( "Elevation map" ), type_ ) );
    type_->setButton( 0 ); // $$$$ SBO 2007-01-03: use default configuration from registry or something...

    elevationBox_ = new QGroupBox( 2, Qt::Horizontal, tr( "Elevation map" ), this );
    new QLabel( tr( "Lowest heights color: " ), elevationBox_ );
    new ColorButton( elevationBox_ );
    new QLabel( tr( "Highest heights color: " ), elevationBox_ );
    new ColorButton( elevationBox_ );
    elevationBox_->hide();
    connect( type_, SIGNAL( clicked( int ) ), this, SLOT( TypeChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: Graphics3dPanel destructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
Graphics3dPanel::~Graphics3dPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Graphics3dPanel::TypeChanged
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void Graphics3dPanel::TypeChanged( int id )
{
    elevationBox_->setShown( id == 1 );
}
