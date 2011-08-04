// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_MissionTypes.h"
#include "ADN_Missions_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tools.h"
#include "ADN_Missions_GUI.h"
#include "ADN_Mission_Wizard.h"

typedef ADN_Missions_Data::Mission Mission;

// -----------------------------------------------------------------------------
// Name: ADN_ListView_MissionTypes constructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_ListView_MissionTypes::ADN_ListView_MissionTypes( ADN_Missions_Data::T_Mission_Vector& missions, QWidget* parent /* = 0*/, const char* szName /* = 0*/ )
    : ADN_ListView( parent, szName )
    , missions_( missions )
{
    addColumn( tr( "Missions" ) );
    setResizeMode( Q3ListView::AllColumns );

    pConnector_ = new ADN_Connector_ListView< Mission >( *this );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_MissionTypes destructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_ListView_MissionTypes::~ADN_ListView_MissionTypes()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_MissionTypes::ConnectItem
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_ListView_MissionTypes::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    Mission* pInfos = (Mission*)pCurData_;
//    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Missions_GUI::eNbrGuiElements );
    vItemConnectors_[ADN_Missions_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDoctrineDescription]->Connect( &pInfos->doctrineDescription_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eUsageDescription]->Connect( &pInfos->usageDescription_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eParameters]->Connect( &pInfos->parameters_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDiaType]->Connect( &pInfos->diaType_, bConnect );
    if( vItemConnectors_[ADN_Missions_GUI::eBehavior] )
        vItemConnectors_[ADN_Missions_GUI::eBehavior]->Connect( &pInfos->diaBehavior_, bConnect );
    if( vItemConnectors_[ADN_Missions_GUI::eMRTBehavior] )
    {
        vItemConnectors_[ADN_Missions_GUI::eMRTBehavior]->Connect( &pInfos->mrtDiaBehavior_, bConnect );
        vItemConnectors_[ADN_Missions_GUI::eCDTBehavior]->Connect( &pInfos->cdtDiaBehavior_, bConnect );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_MissionTypes::OnContextMenu
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_ListView_MissionTypes::OnContextMenu( const QPoint& pt )
{
    if( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin )
    {
        Q3PopupMenu popupMenu( this );
        ADN_Mission_Wizard wizard( missions_, this );
        FillContextMenuWithDefault( popupMenu, wizard );
        popupMenu.exec( pt );
    }
}
