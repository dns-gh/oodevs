// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "BooleanOptionButton.h"
#include "moc_BooleanOptionButton.cpp"
#include "astec_kernel/Options.h"

// -----------------------------------------------------------------------------
// Name: BooleanOptionButton constructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
BooleanOptionButton::BooleanOptionButton( const QIconSet& iconSet, const QString& toolTip, QToolBar* parent, Options& options, const std::string& option)
    : QToolButton( iconSet, "", "", 0, "", parent, "" )
    , options_( options )
    , option_( option )
    , toolTip_( toolTip )
{
    QToolTip::add( this, toolTip_ );
    setUsesTextLabel( true );                  
    setTextPosition( QToolButton::BesideIcon );
    setToggleButton( true );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( OnToggled( bool ) ) );
    options_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: BooleanOptionButton destructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
BooleanOptionButton::~BooleanOptionButton()
{
    options_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: BooleanOptionButton::OnToggled
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void BooleanOptionButton::OnToggled( bool on )
{
    options_.Change( option_, on );
}

// -----------------------------------------------------------------------------
// Name: BooleanOptionButton::OptionChanged
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void BooleanOptionButton::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == option_ && isOn() != value.To< bool >() )
        setOn( value.To< bool >() );
}
