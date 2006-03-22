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
InfoPanels::InfoPanels( QWidget* pParent, Controllers& controllers )
    : QWidgetStack( pParent )
{
    this->setMinimumSize( 1, 1 );
    pStatePanel_               = new AgentStatePanel         ( this, controllers );
    pResourcesPanel_           = new AgentResourcesPanel     ( this, controllers );
    pAgentKnowledgePanel_      = new AgentKnowledgePanel     ( this, controllers );
    pObjectPanel_              = new ObjectPanel             ( this, controllers );
    pObjectReportPanel_        = new ObjectReportPanel       ( this, controllers );
    pObjectKnowledgePanel_     = new ObjectKnowledgePanel    ( this, controllers );
    pAgentMaintenancePanel_    = new AgentMaintenancePanel   ( this, controllers );
    pAgentMedicalPanel_        = new AgentMedicalPanel       ( this, controllers );
    pAgentSupplyPanel_         = new AgentSupplyPanel        ( this, controllers );
    pReportPanel_              = new ReportPanel             ( this, controllers );
    pPopulationPanel_		   = new PopulationPanel         ( this, controllers );
    pPopulationKnowledgePanel_ = new PopulationKnowledgePanel( this, controllers );

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
