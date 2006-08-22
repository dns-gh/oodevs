// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UnitToolbar.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TristateOption.h"
#include "clients_gui/ToolListButton.h"
#include "clients_gui/resources.h"

using namespace kernel;
using namespace gui;

namespace
{
    void Populate( ToolListButton< TristateOption >& button )
    {
        button.AddItem( qApp->tr( TristateOption::auto_ ), TristateOption::auto_ );
        button.AddItem( qApp->tr( TristateOption::on_ ), TristateOption::on_ );
        button.AddItem( qApp->tr( TristateOption::off_ ), TristateOption::off_ );
        button.setMinimumWidth( 55 );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitToolbar constructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
UnitToolbar::UnitToolbar( QMainWindow* pParent, Controllers& controllers )
    : QToolBar( pParent, "unit toolbar" )
{
    setLabel( tr( "Options unité" ) );

    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( visionlines ),    tr( "Afficher lignes de vision" ),   this, controllers.options_, "VisionLines" ) );
    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( visioncones ),    tr( "Afficher cones de vision" ),    this, controllers.options_, "VisionCones" ) );
    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( visionsurfaces ), tr( "Afficher surfaces de vision" ), this, controllers.options_, "VisionSurfaces" ) );
    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( path ),           tr( "Afficher les itinéraires" ),    this, controllers.options_, "Paths" ) );
    Populate( *new ToolListButton< TristateOption >( MAKE_ICON( oldpath ),        tr( "Afficher chemin parcouru" ),    this, controllers.options_, "OldPaths" ) );
}

// -----------------------------------------------------------------------------
// Name: UnitToolbar destructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
UnitToolbar::~UnitToolbar()
{
    // NOTHING
}