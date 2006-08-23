// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MapToolbar.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/TristateOption.h"
#include "clients_gui/BooleanOptionButton.h"
#include "clients_gui/ToolListButton.h"
#include "clients_gui/resources.h"

using namespace kernel;
using namespace gui;

namespace
{
    void Populate( ToolListButton< TristateOption >& button )
    {
        button.AddItem( TristateOption::AutoName(), TristateOption::Auto() );
        button.AddItem( TristateOption::OnName(),   TristateOption::On() );
        button.AddItem( TristateOption::OffName(),  TristateOption::Off() );
        button.setMinimumWidth( 55 );
    }
}

// -----------------------------------------------------------------------------
// Name: MapToolbar constructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MapToolbar::MapToolbar( QMainWindow* pParent, Controllers& controllers )
    : QToolBar    ( pParent, "map toolbar" )
{
    setLabel( tr( "Options carte" ) );

    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( textsmall ),     tr( "Afficher petits textes" ),   this, controllers.options_, "SmallText" ) );
    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( textbig ),       tr( "Afficher grands textes" ),   this, controllers.options_, "BigText" ) );
    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( tacticallines ), tr( "Afficher lignes tactiques" ), this, controllers.options_, "TacticalLines" ) );

    new BooleanOptionButton( MAKE_ICON( weather ), tr( "Afficher météo" ), this, controllers.options_, "Weather" );

    addSeparator();

    new QLabel( tr( "Grille:" ), this );
    ToolListButton< float >* pGridStep_ = new ToolListButton< float >( this, controllers.options_, "GridSize" );
    pGridStep_->AddItem( tr( "Off"    ),  -1 );
    pGridStep_->AddItem( tr( "0.1km"  ),  0.1f );
    pGridStep_->AddItem( tr( "0.25km" ),  0.25f );
    pGridStep_->AddItem( tr( "0.5km"  ),  0.5f );
    pGridStep_->AddItem( tr( "1.0km"  ),  1.0f );
    pGridStep_->AddItem( tr( "2.5km"  ),  2.5f );
    pGridStep_->AddItem( tr( "5.0km"  ),  5.0f );
    pGridStep_->AddItem( tr( "10.0km" ), 10.0f );

    addSeparator();
    new BooleanOptionButton( MAKE_ICON( threed ), tr( "3D" ), this, controllers.options_, "3D" );
}

// -----------------------------------------------------------------------------
// Name: MapToolbar destructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MapToolbar::~MapToolbar()
{
    // NOTHING
}
