// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::RefreshRatePanel */

#include "clients_gui_pch.h"
#include "RefreshRatePanel.h"
#include "moc_RefreshRatePanel.cpp"
#include "RichSpinBox.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RefreshRatePanel constructor
// Created: LDC 2014-02-21
// -----------------------------------------------------------------------------
RefreshRatePanel::RefreshRatePanel( QWidget* parent, kernel::Controllers& controllers )
    : PreferencePanel_ABC( parent, "CoordinateSystemsPanel" )
    , controllers_ ( controllers )
    , options_  ( controllers.options_ )
{
    Q3GroupBox* box = new Q3GroupBox( 2, Qt::Vertical, tr( "Refresh rate" ), this );
    new QLabel( tr( "Select refresh rate (in ms):" ), box );
    spinBox_ = new RichSpinBox( "RefreshRateSpinBox", box, 10 );
    spinBox_->setValue( 50 );
    setWidget( box );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RefreshRatePanel destructor
// Created: LDC 2014-02-21
// -----------------------------------------------------------------------------
RefreshRatePanel::~RefreshRatePanel()
{
     controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RefreshRatePanel::Commit
// Created: LDC 2014-02-21
// -----------------------------------------------------------------------------
void RefreshRatePanel::Commit()
{
    options_.Change( "RefreshRate", spinBox_->value() );
}

// -----------------------------------------------------------------------------
// Name: RefreshRatePanel::Reset
// Created: LDC 2014-02-21
// -----------------------------------------------------------------------------
void RefreshRatePanel::Reset()
{
    spinBox_->setValue( 50 );
    options_.Change( "RefreshRate", 50 );
}
    
// -----------------------------------------------------------------------------
// Name: RefreshRatePanel::OptionChanged
// Created: LDC 2014-02-21
// -----------------------------------------------------------------------------
void RefreshRatePanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    QString option ( name.c_str() );
    if( option == "FrameRate" )
    {
        spinBox_->setValue( value.To< int >() );
    }
}
