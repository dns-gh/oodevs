// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::InhabitantPanel */

#include "clients_gui_pch.h"
#include "InhabitantPanel.h"
#include "moc_InhabitantPanel.cpp"
#include "CheckBox.h"
#include "DensityWidget.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: InhabitantPanel constructor
// Created: LGY 2010-12-31
// -----------------------------------------------------------------------------
InhabitantPanel::InhabitantPanel( QWidget* parent, kernel::Controllers& controllers )
    : PreferencePanel_ABC( parent, "InhabitantPanel" )
    , controllers_( controllers )
    , options_    ( controllers.options_ )
{
    QGroupBox* box = new QGroupBox( 2, Qt::Horizontal, tr( "Colors" ), this );
    QVBox* hBox = new QVBox( box );
    density_ = new CheckBox( tr( "Change urban blocks color based on population density" ), hBox );
    connect( density_, SIGNAL( toggled( bool ) ), SLOT( OnChanged( bool ) ) );
    QGroupBox* group = new QGroupBox( 1, Qt::Horizontal, tr( "Gradient map" ), hBox );
    widget_ = new DensityWidget( group, controllers );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPanel destructor
// Created: LGY 2010-12-31
// -----------------------------------------------------------------------------
InhabitantPanel::~InhabitantPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPanel::OnChanged
// Created: LGY 2010-12-31
// -----------------------------------------------------------------------------
void InhabitantPanel::OnChanged( bool value )
{
    options_.Change( "UrbanDensityColor", value );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPanel::OptionChanged
// Created: LGY 2011-01-11
// -----------------------------------------------------------------------------
void InhabitantPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "UrbanDensityColor" )
        density_->setChecked( value.To< bool >() );
}
