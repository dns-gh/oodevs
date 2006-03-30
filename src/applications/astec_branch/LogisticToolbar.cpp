// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogisticToolbar.h"
#include "Controllers.h"
#include "TristateOption.h"
#include "ToolListButton.h"

// -----------------------------------------------------------------------------
// Name: LogisticToolbar constructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
LogisticToolbar::LogisticToolbar( QMainWindow* pParent, Controllers& controllers )
    : QToolBar( pParent, "logistic toolbar" )
{
    setLabel( tr( "Options logistique" ) );

    TristateOption::Populate( *new ToolListButton< TristateOption >( MAKE_ICON( loglink ),     tr( "Afficher les liens logistiques" ),               this, controllers.options_, "LogisticLinks" ) );
    TristateOption::Populate( *new ToolListButton< TristateOption >( MAKE_ICON( missinglog ),  tr( "Afficher les liens logistiques manquant" ),      this, controllers.options_, "MissingLogisticLinks" ) );
    TristateOption::Populate( *new ToolListButton< TristateOption >( MAKE_ICON( realtimelog ), tr( "Affichage temps r�el des actions logistiques" ), this, controllers.options_, "RealTimeLogistic" ) );
    // $$$$ AGE 2006-03-30: aggregateall, desagregateall
}

// -----------------------------------------------------------------------------
// Name: LogisticToolbar destructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
LogisticToolbar::~LogisticToolbar()
{
    // NOTHING
}
