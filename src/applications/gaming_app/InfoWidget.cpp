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
#include "clients_gui/DisplayExtractor.h"

// -----------------------------------------------------------------------------
// Name: InfoWidget constructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoWidget::InfoWidget( QWidget* parent,
                        kernel::Controllers& controllers,
                        const kernel::Profile_ABC& profile,
                        const gui::EntitySymbols& icons,
                        gui::ItemFactory_ABC& itemFactory,
                        gui::DisplayExtractor& extractor,
                        const kernel::KnowledgeConverter_ABC& converter,
                        Model& model,
                        const Simulation& simulation,
                        SimulationController& simulationController,
                        UnitStateDialog& unitStateDialog )
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
    new InfoButtonsWidget( this, controllers, itemFactory, extractor, model, simulation, profile, simulationController, unitStateDialog );

    QTabWidget* tabs = new QTabWidget( this );
    tabs->setTabShape( QTabWidget::Triangular );
    reports_ = new InfoReportsTab( tabs, controllers, extractor );
    tabs->addTab( reports_, MAKE_PIXMAP( msg ), tr( "Reports" ) );
    conflicts_ = new InfoConflictsTab( tabs, controllers, extractor, converter );
    tabs->addTab( conflicts_, MAKE_PIXMAP( conflict ), tr( "Conflicts" ) );
    missions_ = new InfoMissionsTab( tabs, controllers, extractor );
    tabs->addTab( missions_, MAKE_PIXMAP( mission ), tr( "Missions" ) );
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

void InfoWidget::Purge()
{
    reports_->Purge();
    conflicts_->Purge();
    missions_->Purge();
}
