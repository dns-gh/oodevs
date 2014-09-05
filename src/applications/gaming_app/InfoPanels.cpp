// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoPanels.h"
#include "AgentKnowledgePanel.h"
#include "ObjectKnowledgePanel.h"
#include "ObjectReportPanel.h"
#include "PopulationKnowledgePanel.h"
#include "ObjectPanel.h"
#include "UrbanKnowledgePanel.h"
#include "clients_gui/Panels.h"
#include "clients_gui/PopulationPanel.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: InfoPanels constructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
InfoPanels::InfoPanels( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::DisplayExtractor& extractor )
    : gui::RichDockWidget( controllers, parent, "info-panels" )
{
    setWindowTitle( tools::translate( "InfoPanels", "Knowledge" ) );
    gui::Panels* panels = new gui::Panels( "InfoPanels", this );
    setWidget( panels );
    panels->setMinimumWidth( 200 );
    panels->AddPanel( new AgentKnowledgePanel     ( panels, *panels, controllers, factory ) );
    panels->AddPanel( new gui::PopulationPanel    ( panels, *panels, controllers, factory ) );
    panels->AddPanel( new PopulationKnowledgePanel( panels, *panels, controllers, factory ) );
    panels->AddPanel( new ObjectPanel             ( panels, *panels, controllers, factory ) );
    panels->AddPanel( new ObjectReportPanel       ( panels, *panels, controllers, extractor ) );
    panels->AddPanel( new ObjectKnowledgePanel    ( panels, *panels, controllers, factory ) );
    panels->AddPanel( new UrbanKnowledgePanel     ( panels, *panels, controllers, factory ) );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels destructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
InfoPanels::~InfoPanels()
{
    // NOTHING
}
