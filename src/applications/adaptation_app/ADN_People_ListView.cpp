// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_People_ListView.h"
#include "ADN_People_Data.h"
#include "ADN_People_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_People_Wizard.h"

typedef ADN_People_Data::PeopleInfos PeopleInfos;

// -----------------------------------------------------------------------------
// Name: ADN_People_ListView constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
ADN_People_ListView::ADN_People_ListView( QWidget* pParent, const char* szName, Qt::WFlags f )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Population" ) );
    setResizeMode( Q3ListView::AllColumns );
    // Connector creation
    pConnector_ = new ADN_Connector_ListView< PeopleInfos >( *this );
    this->SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_People_ListView destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
ADN_People_ListView::~ADN_People_ListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_People_ListView::ConnectItem
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void ADN_People_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    PeopleInfos* pInfos = static_cast< PeopleInfos* >( pCurData_ );
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_People_GUI::eNbrGuiElements );
    vItemConnectors_[ ADN_People_GUI::eName ]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ ADN_People_GUI::eModel ]->Connect( &pInfos->ptrModel_, bConnect );
    vItemConnectors_[ ADN_People_GUI::eAngryCrowdMission ]->Connect( &pInfos->strAngryCrowdMission_, bConnect );
    vItemConnectors_[ ADN_People_GUI::eMale ]->Connect( &pInfos->repartition_.male_, bConnect );
    vItemConnectors_[ ADN_People_GUI::eFemale ]->Connect( &pInfos->repartition_.female_, bConnect );
    vItemConnectors_[ ADN_People_GUI::eChildren ]->Connect( &pInfos->repartition_.children_, bConnect );
    vItemConnectors_[ ADN_People_GUI::eLossOnFire ]->Connect( &pInfos->securityLossOnFire_, bConnect );
    vItemConnectors_[ ADN_People_GUI::eHealthNeed ]->Connect( &pInfos->healthNeed_, bConnect );
    vItemConnectors_[ ADN_People_GUI::eGainPerHour ]->Connect( &pInfos->securityGainPerHour_, bConnect );
    vItemConnectors_[ ADN_People_GUI::eTransferTime ]->Connect( &pInfos->transferTime_, bConnect );
    vItemConnectors_[ ADN_People_GUI::eConsumptions ]->Connect( &pInfos->consumptions_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_People_ListView::OnContextMenu
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void ADN_People_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_People_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
