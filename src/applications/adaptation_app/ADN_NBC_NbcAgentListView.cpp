// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_NBC_NbcAgentListView.h"
#include "ADN_Workspace.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tools.h"
#include "ADN_NBC_Datas.h"
#include "ADN_NBC_GUI.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"

typedef ADN_NBC_Datas::NbcAgentInfos NbcAgentInfos;

// -----------------------------------------------------------------------------
// Name: ADN_NBC_NbcAgentListView constructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_NbcAgentListView::ADN_NBC_NbcAgentListView( QWidget* pParent )
: ADN_ListView( pParent, "ADN_NBC_NbcAgentListView", ADN_Tr::ConvertFromWorkspaceElement( eNBC ).c_str() )
{
    // connector creation
    pConnector_ = new ADN_Connector_ListView<NbcAgentInfos>(*this);

    this->SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_NbcAgentListView destructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_NbcAgentListView::~ADN_NBC_NbcAgentListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_NbcAgentListView::ConnectItem
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
void ADN_NBC_NbcAgentListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    NbcAgentInfos* pInfos = (NbcAgentInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_NBC_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_NBC_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_NBC_GUI::eLiquidGroup]->Connect( &pInfos->liquidInfos_, bConnect );
    vItemConnectors_[ADN_NBC_GUI::eGazGroup]->Connect( &pInfos->gazInfos_.intoxInfos_, bConnect );
    vItemConnectors_[ADN_NBC_GUI::eGazLifetime]->Connect( &pInfos->gazInfos_.lifeTime_, bConnect );
    vItemConnectors_[ADN_NBC_GUI::eGazSpreadAngle]->Connect( &pInfos->gazInfos_.rSpreadAngle_, bConnect );
    vItemConnectors_[ADN_NBC_GUI::eGazGroupPresent]->Connect( &pInfos->bGazPresent_, bConnect );
    vItemConnectors_[ADN_NBC_GUI::eLiquidGroupPresent]->Connect( &pInfos->bLiquidPresent_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_NbcAgentListView::OnContextMenu
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
void ADN_NBC_NbcAgentListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< NbcAgentInfos > wizard( ADN_Tr::ConvertFromWorkspaceElement( eNBC ).c_str(), ADN_Workspace::GetWorkspace().GetNbc().GetData().GetNbcAgentVector(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
