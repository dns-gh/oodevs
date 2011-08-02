// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoWidget.h"
#include "moc_InfoWidget.cpp"
#include "InfoStatusWidget.h"
#include "InfoSubordinatesWidget.h"
#include "InfoStancesWidget.h"
#include "InfoSummariesWidget.h"
#include "InfoEventsWidget.h"
#include "InfoButtonsWidget.h"
#include "InfoReportsTab.h"
#include "InfoConflictsTab.h"
#include "InfoMissionsTab.h"
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: InfoWidget constructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoWidget::InfoWidget( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons, gui::ItemFactory_ABC& itemFactory )
    : Q3HBox( parent, "InfoWidget" )
{
    layout()->setSpacing( 10 );
    layout()->setAlignment( Qt::AlignTop | Qt::AlignLeft );
    new InfoStatusWidget( this, controllers, profile, icons );
    new InfoSubordinatesWidget( this, controllers, profile, icons );
    new InfoEventsWidget( this, controllers );

    Q3VBox* box = new Q3VBox( this );
    box->setSpacing( 5 );
    box->layout()->setAlignment( Qt::AlignTop | Qt::AlignLeft );
    new InfoSummariesWidget( box, controllers );
    new InfoStancesWidget( box, controllers );
    new InfoButtonsWidget( this, controllers, itemFactory );

    QTabWidget* tabs = new QTabWidget( this );
    tabs->setTabShape( QTabWidget::Triangular );
    tabs->addTab( new InfoReportsTab( tabs, controllers, itemFactory ), MAKE_PIXMAP( msg ), tr( "Reports" ) );
    tabs->addTab( new InfoConflictsTab( tabs, controllers, itemFactory ), MAKE_PIXMAP( conflict ), tr( "Conflicts" ) );
    tabs->addTab( new InfoMissionsTab( tabs, controllers, itemFactory ), MAKE_PIXMAP( mission ), tr( "Missions" ) );
    for( unsigned int i = 0; i < 3; ++i )
        tabs->setTabEnabled( tabs->page( i ), false );
    tabs->setCurrentPage( 0 );
}

// -----------------------------------------------------------------------------
// Name: InfoWidget destructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoWidget::~InfoWidget()
{
    // NOTHING
}
