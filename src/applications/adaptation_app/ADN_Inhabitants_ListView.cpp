// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Inhabitants_ListView.h"
#include "ADN_Inhabitants_Data.h"
#include "ADN_Inhabitants_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"

typedef ADN_Inhabitants_Data::InhabitantsInfos PeopleInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_ListView constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
ADN_Inhabitants_ListView::ADN_Inhabitants_ListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Inhabitants_ListView", ADN_Tr::ConvertFromWorkspaceElement( eInhabitants ).c_str() )
{
    pConnector_.reset( new ADN_Connector_ListView< PeopleInfos >( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_ListView destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
ADN_Inhabitants_ListView::~ADN_Inhabitants_ListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_ListView::ConnectItem
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void ADN_Inhabitants_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    PeopleInfos* pInfos = static_cast< PeopleInfos* >( pCurData_ );
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Inhabitants_GUI::eNbrGuiElements );
    vItemConnectors_[ ADN_Inhabitants_GUI::eName ]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ ADN_Inhabitants_GUI::eModel ]->Connect( &pInfos->ptrModel_, bConnect );
    vItemConnectors_[ ADN_Inhabitants_GUI::eAngryCrowdMission ]->Connect( &pInfos->strAngryCrowdMission_, bConnect );
    vItemConnectors_[ ADN_Inhabitants_GUI::eMale ]->Connect( &pInfos->repartition_.male_, bConnect );
    vItemConnectors_[ ADN_Inhabitants_GUI::eFemale ]->Connect( &pInfos->repartition_.female_, bConnect );
    vItemConnectors_[ ADN_Inhabitants_GUI::eChildren ]->Connect( &pInfos->repartition_.children_, bConnect );
    vItemConnectors_[ ADN_Inhabitants_GUI::eLossOnFire ]->Connect( &pInfos->securityLossOnFire_, bConnect );
    vItemConnectors_[ ADN_Inhabitants_GUI::eHealthNeed ]->Connect( &pInfos->healthNeed_, bConnect );
    vItemConnectors_[ ADN_Inhabitants_GUI::eGainPerHour ]->Connect( &pInfos->securityGainPerHour_, bConnect );
    vItemConnectors_[ ADN_Inhabitants_GUI::eTransferTime ]->Connect( &pInfos->transferTime_, bConnect );
    vItemConnectors_[ ADN_Inhabitants_GUI::eSchedule ]->Connect( &pInfos->schedule_, bConnect );
    vItemConnectors_[ ADN_Inhabitants_GUI::eConsumptions ]->Connect( &pInfos->consumptions_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_ListView::OnContextMenu
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void ADN_Inhabitants_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< PeopleInfos > wizard( ADN_Tr::ConvertFromWorkspaceElement( eInhabitants ).c_str(), ADN_Workspace::GetWorkspace().GetInhabitants().GetData().GetInhabitants(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
