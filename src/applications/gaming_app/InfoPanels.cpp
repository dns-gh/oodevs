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
#include "ReportPanel.h"
#include "AgentStatePanel.h"
#include "AgentResourcesPanel.h"
#include "ObjectPanel.h"
#include "AgentMaintenancePanel.h"
#include "AgentMedicalPanel.h"
#include "AgentSupplyPanel.h"
#include "clients_gui/PopulationPanel.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: InfoPanels constructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
InfoPanels::InfoPanels( QWidget* parent, Controllers& controllers, ItemFactory_ABC& factory )
    : Panels( parent )
{
    AddPanel( new AgentStatePanel         ( this, *this, controllers, factory ) );
    AddPanel( new AgentResourcesPanel     ( this, *this, controllers, factory ) );
    AddPanel( new AgentKnowledgePanel     ( this, *this, controllers, factory ) );
    AddPanel( new PopulationPanel         ( this, *this, controllers, factory ) );
    AddPanel( new PopulationKnowledgePanel( this, *this, controllers, factory ) );
    AddPanel( new ObjectPanel             ( this, *this, controllers, factory ) );
    AddPanel( new ObjectReportPanel       ( this, *this, controllers, factory ) );
    AddPanel( new ObjectKnowledgePanel    ( this, *this, controllers, factory ) );
    AddPanel( new ReportPanel             ( this, *this, controllers, factory ) );
    AddPanel( new AgentMaintenancePanel   ( this, *this, controllers, factory ) );
    AddPanel( new AgentMedicalPanel       ( this, *this, controllers, factory ) );
    AddPanel( new AgentSupplyPanel        ( this, *this, controllers, factory ) );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels destructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
InfoPanels::~InfoPanels()
{
    // NOTHING
}
