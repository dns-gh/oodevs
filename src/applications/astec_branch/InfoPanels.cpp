// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/InfoPanels.cpp $
// $Author: Age $
// $Modtime: 21/04/05 16:05 $
// $Revision: 10 $
// $Workfile: InfoPanels.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "InfoPanels.h"
#include "AgentStatePanel.h"
#include "ReportPanel.h"
#include "AgentResourcesPanel.h"
#include "AgentKnowledgePanel.h"
#include "ObjectPanel.h"
#include "ObjectReportPanel.h"
#include "ObjectKnowledgePanel.h"
#include "PopulationPanel.h"
#include "PopulationKnowledgePanel.h"
#include "AgentMaintenancePanel.h"
#include "AgentMedicalPanel.h"
#include "AgentSupplyPanel.h"

// -----------------------------------------------------------------------------
// Name: InfoPanels constructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
InfoPanels::InfoPanels( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : QWidgetStack( pParent )
{
    this->setMinimumSize( 1, 1 );
    pStatePanel_               = new AgentStatePanel         ( this, controllers, factory  );
    pResourcesPanel_           = new AgentResourcesPanel     ( this, controllers, factory );
    pAgentKnowledgePanel_      = new AgentKnowledgePanel     ( this, controllers, factory );
    pObjectPanel_              = new ObjectPanel             ( this, controllers, factory  );
    pObjectReportPanel_        = new ObjectReportPanel       ( this, controllers, factory );
    pObjectKnowledgePanel_     = new ObjectKnowledgePanel    ( this, controllers, factory );
    pAgentMaintenancePanel_    = new AgentMaintenancePanel   ( this, controllers, factory );
    pAgentMedicalPanel_        = new AgentMedicalPanel       ( this, controllers, factory );
    pAgentSupplyPanel_         = new AgentSupplyPanel        ( this, controllers, factory );
    pReportPanel_              = new ReportPanel             ( this, controllers, factory );
    pPopulationPanel_		   = new PopulationPanel         ( this, controllers, factory );
    pPopulationKnowledgePanel_ = new PopulationKnowledgePanel( this, controllers, factory );

    pTabWidget_ = new QTabWidget( this );

    addWidget  ( pTabWidget_ );
    raiseWidget( pTabWidget_ );
    pTabWidget_->setCurrentPage( 0 );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
InfoPanels::~InfoPanels()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::Add
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void InfoPanels::Add( QWidget* widget, const QString& name )
{
    if( pTabWidget_->indexOf( widget ) != -1 )
        return;
    int index = pTabWidget_->currentPageIndex();
    pTabWidget_->insertTab( widget, name );
    pTabWidget_->setCurrentPage( index );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::Remove
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void InfoPanels::Remove( QWidget* widget )
{
    pTabWidget_->removePage( widget );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::sizeHint
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
QSize InfoPanels::sizeHint() const
{
    return QSize( 230, 340 );
}
