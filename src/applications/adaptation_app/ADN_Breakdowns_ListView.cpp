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

#include "adaptation_app_pch.h"
#include "ADN_Breakdowns_ListView.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_Breakdowns_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Breakdown_Wizard.h"

typedef ADN_Breakdowns_Data::BreakdownInfo BreakdownInfo;


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Breakdowns_ListView::ADN_Breakdowns_ListView( QWidget* pParent, const char* szName, Qt::WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Breakdowns" ) );
    setResizeMode( Q3ListView::AllColumns );

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
    vItemConnectors_[ADN_Breakdowns_GUI::eNTI]->Connect( &pInfos->nNTI_, bConnect );
    vItemConnectors_[ADN_Breakdowns_GUI::eRepairTime]->Connect( &pInfos->repairTime_, bConnect );
    vItemConnectors_[ADN_Breakdowns_GUI::eRepairTimeVariance]->Connect( &pInfos->repairTimeVariance_, bConnect );
    vItemConnectors_[ADN_Breakdowns_GUI::eParts]->Connect( &pInfos->vRepairParts_, bConnect );
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_ListView::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Breakdowns_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Breakdown_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
