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
#include "clients_gui/PopulationPanel.h"
#include "UrbanKnowledgePanel.h"

// -----------------------------------------------------------------------------
// Name: InfoPanels constructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
InfoPanels::InfoPanels( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::DisplayExtractor& extractor )
    : Panels( parent )
{
    setMinimumWidth( 200 );
    AddPanel( new AgentKnowledgePanel     ( this, *this, controllers, factory ) );
    AddPanel( new gui::PopulationPanel    ( this, *this, controllers, factory ) );
    AddPanel( new PopulationKnowledgePanel( this, *this, controllers, factory ) );
    AddPanel( new ObjectPanel             ( this, *this, controllers, factory ) );
    AddPanel( new ObjectReportPanel       ( this, *this, controllers, extractor ) );
    AddPanel( new ObjectKnowledgePanel    ( this, *this, controllers, factory ) );
    AddPanel( new UrbanKnowledgePanel     ( this, *this, controllers, factory ) );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels destructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
InfoPanels::~InfoPanels()
{
    // NOTHING
}
