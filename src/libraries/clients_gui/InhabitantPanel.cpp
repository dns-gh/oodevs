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
    QGroupBox* densityGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Density Gradient map" ), hBox );
    new DensityWidget( densityGroup, controllers, "Density" );
    QGroupBox* occupationGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Occupation Gradient map" ), hBox );
    new DensityWidget( occupationGroup, controllers, "Accommodation" );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPanel destructor
// Created: LGY 2010-12-31
// -----------------------------------------------------------------------------
InhabitantPanel::~InhabitantPanel()
{
    // NOTHING
}
