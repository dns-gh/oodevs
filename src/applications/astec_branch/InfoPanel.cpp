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
#include "InfoPanel_ABC.h"

#include "MainWindow.h"
#include "AgentStatePanel.h"
#include "ReportPanel.h"
#include "AgentResourcesPanel.h"
#include "AgentKnowledgePanel.h"
#include "ObjectPanel.h"
//#include "ObjectReportPanel.h"
#include "ObjectKnowledgePanel.h"
#include "PopulationPanel.h"
#include "PopulationKnowledgePanel.h"
#include "AgentMaintenancePanel.h"
#include "AgentMedicalPanel.h"
#include "AgentSupplyPanel.h"

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
    pStatePanel_               = new AgentStatePanel(  this, controller, actionController );
    pResourcesPanel_           = new AgentResourcesPanel( this, controller, actionController );
    pAgentKnowledgePanel_      = new AgentKnowledgePanel( this, controller, actionController );
    pObjectPanel_              = new ObjectPanel( this, controller, actionController );
//    pObjectReportPanel_        = new ObjectReportPanel( this );
    pObjectKnowledgePanel_     = new ObjectKnowledgePanel( this, controller, actionController );
    pAgentMaintenancePanel_    = new AgentMaintenancePanel( this, controller, actionController );
    pAgentMedicalPanel_        = new AgentMedicalPanel( this , controller, actionController );
    pAgentSupplyPanel_         = new AgentSupplyPanel( this, controller, actionController );
    pReportPanel_              = new ReportPanel( this, controller, actionController );
    pPopulationPanel_		   = new PopulationPanel( this, controller, actionController );
    pPopulationKnowledgePanel_ = new PopulationKnowledgePanel( this, controller, actionController );

    pTabWidget_ = new QTabWidget( this );

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
// Name: InfoPanel::Add
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void InfoPanel::Add( QWidget* widget, const QString& name )
{
    if( pTabWidget_->indexOf( widget ) != -1 )
        return;
    int index = pTabWidget_->currentPageIndex();
    pTabWidget_->insertTab( widget, name );
    pTabWidget_->setCurrentPage( index );
}

// -----------------------------------------------------------------------------
// Name: InfoPanel::Remove
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void InfoPanel::Remove( QWidget* widget )
{
    pTabWidget_->removePage( widget );
}

// -----------------------------------------------------------------------------
// Name: InfoPanel::sizeHint
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
QSize InfoPanel::sizeHint() const
{
    return QSize( 230, 340 );
}
