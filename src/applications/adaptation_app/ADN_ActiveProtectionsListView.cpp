// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ActiveProtectionsListView.h"
#include "ADN_ActiveProtections_Data.h"
#include "ADN_ActiveProtections_Gui.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tools.h"
#include "ADN_ObjectCreator_ABC.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_ActiveProtections_Data::ActiveProtectionsInfos ActiveProtectionsInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AmmoListView constructor
// Created: FDS 2010-02-25
// -----------------------------------------------------------------------------
ADN_ActiveProtectionsListView::ADN_ActiveProtectionsListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_ActiveProtectionsListView", ADN_Tr::ConvertFromWorkspaceElement( eActiveProtections ).c_str() )
{
    pConnector_.reset( new ADN_Connector_ListView<ActiveProtectionsInfos>( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AmmoListView destructor
// Created: FDS 2010-02-25
// -----------------------------------------------------------------------------
ADN_ActiveProtectionsListView::~ADN_ActiveProtectionsListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtectionsListView::ConnectItem
// Created: FDS 2010-02-25
// -----------------------------------------------------------------------------
void ADN_ActiveProtectionsListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ActiveProtectionsInfos* pInfos = (ActiveProtectionsInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_ActiveProtections_GUI::eNbrActiveProtectionsGuiElements );
    vItemConnectors_[ADN_ActiveProtections_GUI::eActiveProtectionName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_ActiveProtections_GUI::eActiveProtectionCoeffiscient]->Connect( &pInfos->coefficient_, bConnect );
    vItemConnectors_[ADN_ActiveProtections_GUI::eActiveProtectionHardKill]->Connect( &pInfos->hardKill_, bConnect );
    vItemConnectors_[ADN_ActiveProtections_GUI::eActiveProtectionDotation]->Connect( &pInfos->ptr_, bConnect );
    vItemConnectors_[ADN_ActiveProtections_GUI::eActiveProtectionUsage]->Connect( &pInfos->usage_, bConnect );
    vItemConnectors_[ADN_ActiveProtections_GUI::eActiveProtectionWeapons]->Connect( &pInfos->weapons_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtectionsListView::OnContextMenu
// Created: FDS 2010-02-25
// -----------------------------------------------------------------------------
void ADN_ActiveProtectionsListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ActiveProtectionsInfos > wizard( ADN_Tr::ConvertFromWorkspaceElement( eActiveProtections ).c_str(), ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().GetActiveProtectionsInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        ActiveProtectionsInfos* pCastData = static_cast< ActiveProtectionsInfos* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ), eEquipments );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtectionsListView::GetToolTipFor
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
std::string ADN_ActiveProtectionsListView::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    ActiveProtectionsInfos* pCastData = static_cast< ActiveProtectionsInfos* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                            ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ) );
}
