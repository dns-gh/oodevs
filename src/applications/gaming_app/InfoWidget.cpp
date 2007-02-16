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
#include "InfoEventsWidget.h"
#include "InfoButtonsWidget.h"
#include "InfoReportsTab.h"
#include "InfoConflictsTab.h"
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: InfoWidget constructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoWidget::InfoWidget( QWidget* parent, kernel::Controllers& controllers, gui::SymbolIcons& icons, gui::ItemFactory_ABC& itemFactory )
    : QHBox( parent )
{
    layout()->setSpacing( 10 );
    layout()->setAlignment( Qt::AlignLeft );
    new InfoStatusWidget( this, controllers, icons );
    new InfoEventsWidget( this, controllers );
    new InfoButtonsWidget( this, controllers, itemFactory );

    // $$$$ SBO 2007-02-05: 
    QTabWidget* tabs = new QTabWidget( this );
    tabs->addTab( new InfoReportsTab( this, controllers, itemFactory ), MAKE_PIXMAP( msg ), tr( "Reports" ) );
    tabs->addTab( new InfoConflictsTab( this, controllers, itemFactory ), MAKE_PIXMAP( conflict ), tr( "Conflicts" ) );
    tabs->addTab( new QWidget( this ), tr( "Knowledges" ) );
}

// -----------------------------------------------------------------------------
// Name: InfoWidget destructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoWidget::~InfoWidget()
{
    // NOTHING
}
