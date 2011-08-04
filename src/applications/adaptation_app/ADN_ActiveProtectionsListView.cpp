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
#include "ADN_Connector_ListView.h"
#include "ADN_Tools.h"
#include "ADN_ObjectCreator_ABC.h"
#include "ADN_ActiveProtections_Wizard.h"

typedef ADN_ActiveProtections_Data::ActiveProtectionsInfos ActiveProtectionsInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AmmoListView constructor
// Created: FDS 2010-02-25
// -----------------------------------------------------------------------------
ADN_ActiveProtectionsListView::ADN_ActiveProtectionsListView( QWidget* pParent, const char* szName, Qt::WFlags f )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "active Protection" ) );
    setResizeMode( Q3ListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView<ActiveProtectionsInfos>( *this );
    this->SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AmmoListView destructor
// Created: FDS 2010-02-25
// -----------------------------------------------------------------------------
ADN_ActiveProtectionsListView::~ADN_ActiveProtectionsListView()
{
    delete pConnector_;
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
    vItemConnectors_[ADN_ActiveProtections_GUI::eActiveProtectionDotation]->Connect( &pInfos->ptrAmmunition_, bConnect );
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
    ADN_ActiveProtections_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
