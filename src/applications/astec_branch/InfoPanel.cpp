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
// $Archive: /MVW_v10/Build/SDK/Light2/src/InfoPanel.cpp $
// $Author: Age $
// $Modtime: 21/04/05 16:05 $
// $Revision: 10 $
// $Workfile: InfoPanel.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "InfoPanel.h"

#include "MainWindow.h"
#include "AgentStatePanel.h"
//#include "ReportPanel.h"
//#include "AgentResourcesPanel.h"
//#include "AgentKnowledgePanel.h"
#include "ObjectPanel.h"
//#include "ObjectReportPanel.h"
//#include "ObjectKnowledgePanel.h"
//#include "PopulationPanel.h"
//#include "PopulationKnowledgePanel.h"
//#include "AgentMaintenancePanel.h"
//#include "AgentMedicalPanel.h"
//#include "AgentSupplyPanel.h"

// -----------------------------------------------------------------------------
// Name: InfoPanel constructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
InfoPanel::InfoPanel( QWidget* pParent, Controller& controller, ActionController& actionController )
    : QWidgetStack                 ( pParent )
    , bAgentVisible_               ( false )
    , bAgentKnowledgeVisible_      ( false )
    , bObjectKnowledgeVisible_     ( false )
    , bPopulationKnowledgeVisible_ ( false )
    , bObjectVisible_              ( false )
    , bLogisiticVisible_           ( false )
	, bPopulationVisible_		   ( false )
{
    this->setMinimumSize( 1, 1 );

    pStatePanel_               = new AgentStatePanel( this, controller, actionController );
//    pResourcesPanel_           = new AgentResourcesPanel( this );
//    pAgentKnowledgePanel_      = new AgentKnowledgePanel( this );
//    pObjectPanel_              = new ObjectPanel( this, controller, actionController );
//    pObjectReportPanel_        = new ObjectReportPanel( this );
//    pObjectKnowledgePanel_     = new ObjectKnowledgePanel( this );
//    pAgentMaintenancePanel_    = new AgentMaintenancePanel( this );
//    pAgentMedicalPanel_        = new AgentMedicalPanel( this );
//    pAgentSupplyPanel_         = new AgentSupplyPanel( this );
//    pReportPanel_              = new ReportPanel( this );
//	  pPopulationPanel_		   = new PopulationPanel( this );
//    pPopulationReportPanel_    = new ReportPanel( this );
//    pPopulationKnowledgePanel_ = new PopulationKnowledgePanel( this );

    pTabWidget_ = new QTabWidget( this );
    pTabWidget_->insertTab( pStatePanel_, tr( "Etat" ), 0 );
//    pTabWidget_->insertTab( pObjectPanel_, tr( "Objet" ), 0 );

    addWidget  ( pTabWidget_ );
    raiseWidget( pTabWidget_ );
    pTabWidget_->setCurrentPage( 0 );
}

// -----------------------------------------------------------------------------
// Name: InfoPanel destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
InfoPanel::~InfoPanel()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: InfoPanel::sizeHint
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
QSize InfoPanel::sizeHint() const
{
    return QSize( 230, 340 );
}
