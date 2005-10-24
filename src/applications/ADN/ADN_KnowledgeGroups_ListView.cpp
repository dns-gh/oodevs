// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-21 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_KnowledgeGroups_ListView.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:14 $
// $Revision: 2 $
// $Workfile: ADN_KnowledgeGroups_ListView.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_KnowledgeGroups_ListView.h"

#include <qpopmenu.h>

#include "ADN_Tools.h"
#include "ADN_KnowledgeGroups_Data.h"
#include "ADN_KnowledgeGroups_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_ObjectCreator_ABC.h"

typedef ADN_KnowledgeGroups_Data::GroupInfo GroupInfo;


// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_ListView constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_ListView::ADN_KnowledgeGroups_ListView( QWidget* pParent, const char* szName, WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Knowledge groups" ) );
    setResizeMode( QListView::AllColumns );

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
    QPopupMenu popupMenu( this );
    ADN_DefaultObjectCreator<GroupInfo> objectCreator;
    FillContextMenuWithDefault( popupMenu, objectCreator );
    popupMenu.exec( pt );
}
