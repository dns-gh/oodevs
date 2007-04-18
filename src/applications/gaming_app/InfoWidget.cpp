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
#include "InfoStatusWidget.h"
#include "InfoSubordinatesWidget.h"
#include "InfoStancesWidget.h"
#include "InfoStatusBar.h"
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
    : QHBox( parent )
{
    layout()->setSpacing( 10 );
    layout()->setAlignment( Qt::AlignTop | Qt::AlignLeft );
    new InfoStatusWidget( this, controllers, profile, icons );
    new InfoSubordinatesWidget( this, controllers, profile, icons );

    QVBox* box = new QVBox( this );
    box->setSpacing( 5 );
    box->layout()->setAlignment( Qt::AlignTop | Qt::AlignHCenter );
    new InfoStatusBar( box, controllers );
    new InfoSummariesWidget( box, controllers );
    new InfoEventsWidget( box, controllers );
    new InfoStancesWidget( box, controllers );

    new InfoButtonsWidget( this, controllers, itemFactory );

    QTabWidget* tabs = new QTabWidget( this );
    tabs->setTabShape( QTabWidget::Triangular );
    tabs->addTab( new InfoReportsTab( this, controllers, itemFactory ), MAKE_PIXMAP( msg ), tr( "Reports" ) );
    tabs->addTab( new InfoConflictsTab( this, controllers, itemFactory ), MAKE_PIXMAP( conflict ), tr( "Conflicts" ) );
    tabs->addTab( new InfoMissionsTab( this, controllers, itemFactory ), MAKE_PIXMAP( mission ), tr( "Missions" ) );
}

// -----------------------------------------------------------------------------
// Name: InfoWidget destructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoWidget::~InfoWidget()
{
    // NOTHING
}
