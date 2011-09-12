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
#include "ADN_ResourceNetworks_Data.h"
#include "ADN_ResourceNetworks_GUI.h"
#include "ADN_ResourceNetworks_Wizard.h"
#include "ADN_Tr.h"

typedef ADN_ResourceNetworks_Data::ResourceNetworkInfos ResourceNetworkInfos;

// -----------------------------------------------------------------------------
// Name: ADN_ListView_ResourceNetworks constructor
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
ADN_ListView_ResourceNetworks::ADN_ListView_ResourceNetworks( QWidget* pParent, const char* szName, Qt::WFlags f )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tools::translate( "ADN_ListView_ResourceNetworks", "Resource networks" ) );
    setResizeMode( Q3ListView::AllColumns );
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
    ADN_ResourceNetworks_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
