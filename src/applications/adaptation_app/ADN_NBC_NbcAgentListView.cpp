// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_NBC_NbcAgentListView.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:04 $
// $Revision: 12 $
// $Workfile: ADN_NBC_NbcAgentListView.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_NBC_NbcAgentListView.h"

#include <qpopmenu.h>

#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_Connector_ListView.h"
#include "ADN_NBC_Wizard.h"
#include "ADN_Tools.h"
#include "ADN_NBC_GUI.h"


typedef ADN_NBC_Datas::NbcAgentInfos NbcAgentInfos;


// -----------------------------------------------------------------------------
// Name: ADN_NBC_NbcAgentListView constructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_NbcAgentListView::ADN_NBC_NbcAgentListView( QWidget* pParent, const char* szName, WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // add one column
    addColumn( tr( "NBC Agents") );
    setResizeMode(QListView::AllColumns);

    // connector creation
    pConnector_ = new ADN_Connector_ListView<NbcAgentInfos>(*this);

    this->setSorting( -1 );
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
    vItemConnectors_[ADN_NBC_GUI::eNbrOk]->Connect( &pInfos->rNbAlivedHumans_, bConnect );
    vItemConnectors_[ADN_NBC_GUI::eNbrHurt1]->Connect( &pInfos->rNbHurtedHumans1_, bConnect );
    vItemConnectors_[ADN_NBC_GUI::eNbrHurt2]->Connect( &pInfos->rNbHurtedHumans2_, bConnect );
    vItemConnectors_[ADN_NBC_GUI::eNbrHurt3]->Connect( &pInfos->rNbHurtedHumans3_, bConnect );
    vItemConnectors_[ADN_NBC_GUI::eNbrHurt4]->Connect( &pInfos->rNbHurtedHumansE_, bConnect );
    vItemConnectors_[ADN_NBC_GUI::eNbrDead]->Connect( &pInfos->rNbDeadHumans_, bConnect );
    vItemConnectors_[ADN_NBC_GUI::eLifetime]->Connect( &pInfos->lifeTime_, bConnect );
    vItemConnectors_[ADN_NBC_GUI::eSpreadAngle]->Connect( &pInfos->rSpreadAngle_, bConnect );
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_NbcAgentListView::OnContextMenu
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
void ADN_NBC_NbcAgentListView::OnContextMenu( const QPoint& pt )
{
    QPopupMenu popupMenu( this );
    ADN_NBC_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );  

    ADN_NBC_Datas::T_NbcAgentInfos_Vector& vNbcAgents = ADN_Workspace::GetWorkspace().GetNbc().GetData().GetNbcAgentVector();
    std::sort( vNbcAgents.begin(), vNbcAgents.end(), ADN_Tools::NameSort<NbcAgentInfos>() );
}
