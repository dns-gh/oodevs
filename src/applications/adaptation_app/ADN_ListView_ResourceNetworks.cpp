// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_ResourceNetworks.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Inhabitants_Data.h"
#include "ADN_ResourceNetworks_Data.h"
#include "ADN_ResourceNetworks_GUI.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_ResourceNetworks_Data::ResourceNetworkInfos ResourceNetworkInfos;

// -----------------------------------------------------------------------------
// Name: ADN_ListView_ResourceNetworks constructor
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
ADN_ListView_ResourceNetworks::ADN_ListView_ResourceNetworks( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_ListView_ResourceNetworks", ADN_Tr::ConvertFromWorkspaceElement( eResourceNetworks ).c_str() )
{
    // Connector creation
    pConnector_ = new ADN_Connector_ListView< ResourceNetworkInfos >(*this);
    this->SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_ResourceNetworks destructor
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
ADN_ListView_ResourceNetworks::~ADN_ListView_ResourceNetworks()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_ResourceNetworks::ConnectItem
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
void ADN_ListView_ResourceNetworks::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    ResourceNetworkInfos* pInfos = static_cast< ResourceNetworkInfos* >( pCurData_ );
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_ResourceNetworks_GUI::eNbrGuiElements );
    vItemConnectors_[ ADN_ResourceNetworks_GUI::eName ]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ ADN_ResourceNetworks_GUI::eCategory ]->Connect( &pInfos->ptrCategory_, bConnect );
    vItemConnectors_[ ADN_ResourceNetworks_GUI::eProduction ]->Connect( &pInfos->nProduction_, bConnect );
    vItemConnectors_[ ADN_ResourceNetworks_GUI::eColor ]->Connect( &pInfos->strColor_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_ResourceNetworks::OnContextMenu
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
void ADN_ListView_ResourceNetworks::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ResourceNetworkInfos > wizard( ADN_Tr::ConvertFromWorkspaceElement( eResourceNetworks ).c_str(), ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        ResourceNetworkInfos* pCastData = static_cast< ResourceNetworkInfos* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eInhabitants ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetInhabitants().GetData().GetInhabitantsThatUse( *pCastData ), eInhabitants );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_ResourceNetworks::GetToolTipFor
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
std::string ADN_ListView_ResourceNetworks::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    ResourceNetworkInfos* pCastData = static_cast< ResourceNetworkInfos* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eInhabitants ).c_str(),
                            ADN_Workspace::GetWorkspace().GetInhabitants().GetData().GetInhabitantsThatUse( *pCastData ) );
}
