// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-18 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Breakdowns_ListView.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 10:05 $
// $Revision: 2 $
// $Workfile: ADN_Breakdowns_ListView.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Breakdowns_ListView.h"

#include <qpopmenu.h>

#include "ADN_Breakdowns_Data.h"
#include "ADN_Breakdowns_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Breakdown_Wizard.h"


typedef ADN_Breakdowns_Data::BreakdownInfo BreakdownInfo;


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Breakdowns_ListView::ADN_Breakdowns_ListView( NTIInfo& ntiInfo, QWidget* pParent, const char* szName, WFlags f )
: ADN_ListView( pParent, szName, f )
, ntiInfo_    ( ntiInfo )
{
    // Add one column.
    addColumn( tr( "Breakdowns" ) );
    setResizeMode( QListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView<BreakdownInfo>(*this);
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_ListView destructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Breakdowns_ListView::~ADN_Breakdowns_ListView()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_ListView::ConnectItem
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
void ADN_Breakdowns_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    BreakdownInfo* pInfos = (BreakdownInfo*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Breakdowns_GUI::eNbrBreakdownGuiElements );

    vItemConnectors_[ADN_Breakdowns_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Breakdowns_GUI::eType]->Connect( &pInfos->nType_, bConnect );
    vItemConnectors_[ADN_Breakdowns_GUI::eId]->Connect( &pInfos->nId_, bConnect );
    vItemConnectors_[ADN_Breakdowns_GUI::eRepairTime]->Connect( &pInfos->rRepairTime_, bConnect );
    vItemConnectors_[ADN_Breakdowns_GUI::eRepairTimeVariance]->Connect( &pInfos->rRepairTimeVariance_, bConnect );
    vItemConnectors_[ADN_Breakdowns_GUI::eParts]->Connect( &pInfos->vRepairParts_, bConnect );
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_ListView::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Breakdowns_ListView::OnContextMenu( const QPoint& pt )
{
    QPopupMenu popupMenu( this );
    ADN_Breakdown_Wizard wizard( ntiInfo_, this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
