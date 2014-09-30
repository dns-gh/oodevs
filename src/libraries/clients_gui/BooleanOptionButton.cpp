// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "BooleanOptionButton.h"
#include "moc_BooleanOptionButton.cpp"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: BooleanOptionButton constructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
BooleanOptionButton::BooleanOptionButton( const QString& objectName, const QIcon& iconSet, const QString& toolTip, QWidget* parent, kernel::OptionsController& options,
                                          const std::string& option, bool savable )
    : RichWidget< QToolButton >( objectName, parent )
    , options_( options )
    , option_( option )
    , toolTip_( toolTip )
    , savable_( savable )
{
    setIconSet( iconSet );
    QToolTip::add( this, toolTip_ );
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
    options_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: BooleanOptionButton::OnToggled
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void BooleanOptionButton::OnToggled( bool on )
{
    options_.Change( option_, on, savable_ );
}

// -----------------------------------------------------------------------------
// Name: BooleanOptionButton::OptionChanged
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void BooleanOptionButton::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == option_ && isOn() != value.To< bool >() )
        setOn( value.To< bool >() );
}
