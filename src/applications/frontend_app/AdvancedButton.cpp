// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "AdvancedButton.h"
#include "moc_AdvancedButton.cpp"

// -----------------------------------------------------------------------------
// Name: AdvancedButton constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
AdvancedButton::AdvancedButton( QWidget* widget )
    : QPushButton( tr( "Advanced view >>" ), widget )
{
    setToggleButton( true );
    setFixedWidth( 100 );
    connect( this, SIGNAL( toggled( bool ) ), SLOT( OnToggle( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: AdvancedButton destructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
AdvancedButton::~AdvancedButton()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AdvancedButton::Link
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void AdvancedButton::Link( QWidget* widget )
{
    widgets_.push_back( widget );
    widget->hide();
}

// -----------------------------------------------------------------------------
// Name: AdvancedButton::OnToggle
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void AdvancedButton::OnToggle( bool on )
{
    for( T_Widgets::const_iterator it = widgets_.begin(); it != widgets_.end(); ++it )
        (*it)->setShown( on );
    setText( on ? tr( "Simple view <<" ) : tr( "Advanced view >>" ) );
}
