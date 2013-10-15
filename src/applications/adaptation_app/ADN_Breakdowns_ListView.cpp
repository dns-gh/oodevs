// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Breakdowns_ListView.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_Breakdowns_GUI.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_Breakdowns_Data::BreakdownInfo BreakdownInfo;

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Breakdowns_ListView::ADN_Breakdowns_ListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Breakdowns_ListView", ADN_Tr::ConvertFromWorkspaceElement( eBreakdowns ).c_str() )
{
    // Connector creation
    pConnector_ = new ADN_Connector_ListView< BreakdownInfo >( *this );
    SetDeletionEnabled( true );
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
    ADN_Wizard< BreakdownInfo > wizard( ADN_Tr::ConvertFromWorkspaceElement( eBreakdowns ).c_str(), ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().GetBreakdowns(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        BreakdownInfo* pCastData = static_cast< BreakdownInfo* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                      ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(), ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ), eEquipments );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_ListView::GetToolTipFor
// Created: ABR 2012-07-25
// -----------------------------------------------------------------------------
std::string ADN_Breakdowns_ListView::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    BreakdownInfo* pCastData = static_cast< BreakdownInfo* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                            ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ) );
}
