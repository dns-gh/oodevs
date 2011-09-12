// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_KnowledgeGroups_ListView.h"
#include "ADN_Tools.h"
#include "ADN_KnowledgeGroups_Data.h"
#include "ADN_KnowledgeGroups_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_KnowledgeGroups_Wizard.h"
#include "ADN_Tr.h"

typedef ADN_KnowledgeGroups_Data::GroupInfo GroupInfo;

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_ListView constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_ListView::ADN_KnowledgeGroups_ListView( QWidget* pParent, const char* szName, Qt::WFlags f )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tools::translate( "ADN_KnowledgeGroups_ListView", "Knowledge groups" ) );
    setResizeMode( Q3ListView::AllColumns );
    // Connector creation
    pConnector_ = new ADN_Connector_ListView<GroupInfo>(*this);
    this->SetDeletionEnabled( true );
}


// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_ListView destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_ListView::~ADN_KnowledgeGroups_ListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_ListView::ConnectItem
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    GroupInfo* pInfos = (GroupInfo*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_KnowledgeGroups_GUI::eNbrGuiElements );
    vItemConnectors_[ADN_KnowledgeGroups_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_KnowledgeGroups_GUI::eCommunicationDelay]->Connect( &pInfos->communicationDelay_, bConnect );
    vItemConnectors_[ADN_KnowledgeGroups_GUI::eAgentMaxLifetime]->Connect( &pInfos->agentInfos_.maxLifetime_, bConnect );
    vItemConnectors_[ADN_KnowledgeGroups_GUI::eAgentMaxDistance]->Connect( &pInfos->agentInfos_.rMaxDistance_, bConnect );
    vItemConnectors_[ADN_KnowledgeGroups_GUI::eAgentHasInterpolationTime]->Connect( &pInfos->agentInfos_.bInterpolationTime_, bConnect );
    vItemConnectors_[ADN_KnowledgeGroups_GUI::eAgentInterpolationTime]->Connect( &pInfos->agentInfos_.interpolationTime_, bConnect );
    vItemConnectors_[ADN_KnowledgeGroups_GUI::ePopulationMaxLifetime]->Connect( &pInfos->populationInfos_.maxLifetime_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_ListView::OnContextMenu
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_KnowledgeGroups_Wizard wizard;
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
