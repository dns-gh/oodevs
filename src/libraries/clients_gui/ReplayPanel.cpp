// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ReplayPanel.h"
#include "moc_ReplayPanel.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_gui/CheckBox.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ReplayPanel constructor
// Created: SLI 2014-06-06
// -----------------------------------------------------------------------------
ReplayPanel::ReplayPanel( QWidget* parent, kernel::Controllers& controllers )
    : PreferencePanel_ABC( parent, "ReplayPanel" )
    , controllers_( controllers )
    , options_( controllers.options_ )
{
    Q3GroupBox* box = new Q3GroupBox( 2, Qt::Vertical, tr( "Replay" ), this );
    checkbox_ = new gui::CheckBox( "ReplayPanelCheckBox", tr( "Set smooth movement" ), box );
    checkbox_->setChecked( true );
    setWidget( box );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ReplayPanel destructor
// Created: SLI 2014-06-06
// -----------------------------------------------------------------------------
ReplayPanel::~ReplayPanel()
{
     controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ReplayPanel::Commit
// Created: SLI 2014-06-06
// -----------------------------------------------------------------------------
void ReplayPanel::Commit()
{
    options_.Change( "Replay/SmoothPositions", checkbox_->isChecked() );
}

// -----------------------------------------------------------------------------
// Name: ReplayPanel::Reset
// Created: SLI 2014-06-06
// -----------------------------------------------------------------------------
void ReplayPanel::Reset()
{
    checkbox_->setChecked( true );
    options_.Change( "Replay/SmoothPositions", true );
}

// -----------------------------------------------------------------------------
// Name: ReplayPanel::OptionChanged
// Created: SLI 2014-06-06
// -----------------------------------------------------------------------------
void ReplayPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    QString option( name.c_str() );
    if( option == "Replay/SmoothPositions" )
        checkbox_->setChecked( value.To< bool >() );
}
