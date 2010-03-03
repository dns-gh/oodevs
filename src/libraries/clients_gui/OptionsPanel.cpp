// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::OptionsPanel */

#include "clients_gui_pch.h"
#include "OptionsPanel.h"
#include "moc_OptionsPanel.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "Settings.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: OptionsPanel constructor
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
OptionsPanel::OptionsPanel( QWidget* pParent, Controllers& controllers )
    : PreferencePanel_ABC( pParent )
    , controllers_( controllers )
    , options_    ( controllers_.options_ )
{
    // Display panel
    QWidget* pDisplayPanel = new QWidget( this );
    QGridLayout* pSubLayout = new QGridLayout( pDisplayPanel, 9, 2, 5 );
    pSubLayout->setMargin( 5 );

    pCompute3dDistance_ = new QCheckBox( tr( "Display object icons"), pDisplayPanel );
    pSubLayout->addWidget( pCompute3dDistance_, 2, 0 );
    connect( pCompute3dDistance_, SIGNAL( toggled( bool ) ), this, SLOT( Change3dDistanceOption( bool ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel destructor
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
OptionsPanel::~OptionsPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel::OptionChanged
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void OptionsPanel::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "3dDistanceComputation" )
        pCompute3dDistance_->setChecked( value.To< bool >() );
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel::Change3dDistanceOption
// Created: SLG 2010-03-01
// -----------------------------------------------------------------------------
void OptionsPanel::Change3dDistanceOption( bool value )
{
    options_.Change( "3dDistanceComputation", value );
}