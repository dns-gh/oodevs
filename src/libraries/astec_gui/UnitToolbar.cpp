// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "UnitToolbar.h"
#include "astec_gaming/Controllers.h"
#include "ToolListButton.h"
#include "TristateOption.h"

// -----------------------------------------------------------------------------
// Name: UnitToolbar constructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
UnitToolbar::UnitToolbar( QMainWindow* pParent, Controllers& controllers )
    : QToolBar( pParent, "unit toolbar" )
{
    setLabel( tr( "Options unité" ) );

    TristateOption::Populate( *new ToolListButton< TristateOption >( MAKE_ICON( visionlines ),    tr( "Afficher lignes de vision" ),   this, controllers.options_, "VisionLines" ) );
    TristateOption::Populate( *new ToolListButton< TristateOption >( MAKE_ICON( visioncones ),    tr( "Afficher cones de vision" ),    this, controllers.options_, "VisionCones" ) );
    TristateOption::Populate( *new ToolListButton< TristateOption >( MAKE_ICON( visionsurfaces ), tr( "Afficher surfaces de vision" ), this, controllers.options_, "VisionSurfaces" ) );
    TristateOption::Populate( *new ToolListButton< TristateOption >( MAKE_ICON( path ),           tr( "Afficher les itinéraires" ),    this, controllers.options_, "Paths" ) );
    TristateOption::Populate( *new ToolListButton< TristateOption >( MAKE_ICON( oldpath ),        tr( "Afficher chemin parcouru" ),    this, controllers.options_, "OldPaths" ) );
}

// -----------------------------------------------------------------------------
// Name: UnitToolbar destructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
UnitToolbar::~UnitToolbar()
{
    // NOTHING
}