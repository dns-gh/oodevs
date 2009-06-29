// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_FragOrderTypes.h"
#include "ADN_Missions_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tools.h"
#include "ADN_Missions_GUI.h"
#include "ADN_FragOrder_Wizard.h"

#include <qpopupmenu.h>

typedef ADN_Missions_Data::FragOrder FragOrder;

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes constructor
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
ADN_ListView_FragOrderTypes::ADN_ListView_FragOrderTypes( ADN_Missions_Data::T_FragOrder_Vector& orders, QWidget* parent /*= 0*/, const char* szName /*= 0*/ )
    : ADN_ListView( parent, szName )
    , orders_( orders )
{
    addColumn( tr( "Fragmentary order" ) );
    setResizeMode( QListView::AllColumns );

    pConnector_ = new ADN_Connector_ListView< FragOrder >( *this );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes destructor
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
ADN_ListView_FragOrderTypes::~ADN_ListView_FragOrderTypes()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes::ConnectItem
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
void ADN_ListView_FragOrderTypes::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    FragOrder* pInfos = (FragOrder*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Missions_GUI::eNbrGuiElements );
    vItemConnectors_[ADN_Missions_GUI::eName]                            ->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDoctrineDescription]             ->Connect( &pInfos->doctrineDescription_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eUsageDescription]                ->Connect( &pInfos->usageDescription_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eParameters]                      ->Connect( &pInfos->parameters_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eFragOrderAvailableForAllMissions]->Connect( &pInfos->isAvailableForAllMissions_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eFragOrderAvailableWithoutMission]->Connect( &pInfos->isAvailableWithoutMission_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes::OnContextMenu
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
void ADN_ListView_FragOrderTypes::OnContextMenu( const QPoint& pt )
{
    QPopupMenu popupMenu( this );
    ADN_FragOrder_Wizard wizard( orders_, this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
