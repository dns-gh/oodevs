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
#include "SignalAdapter.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: BooleanOptionButton constructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
BooleanOptionButton::BooleanOptionButton( kernel::OptionsController& options,
                                          const QString& objectName,
                                          const std::string& optionName,
                                          const QIcon& iconSet,
                                          const QString& toolTip,
                                          QWidget* parent /* = 0 */ )
    : OptionWidget< RichWidget< QToolButton > >( options, objectName, optionName, parent )
{
    setIconSet( iconSet );
    QToolTip::add( this, toolTip );
    setCheckable( true );
    gui::connect( this, SIGNAL( clicked() ), [=,&options]{
        options.Change( optionName, isChecked() );
    } );
}

// -----------------------------------------------------------------------------
// Name: BooleanOptionButton destructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
BooleanOptionButton::~BooleanOptionButton()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BooleanOptionButton::OnOptionChanged
// Created: ABR 2014-10-10
// -----------------------------------------------------------------------------
void BooleanOptionButton::OnOptionChanged( const kernel::OptionVariant& value )
{
    setChecked( value.To< bool >() );
}
