// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Missions.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Gui_Tools.h"
#include "ADN_Models_Data.h"
#include "ADN_Models_GUI.h"
#include "ADN_Workspace.h"
#include "ADN_ListView_Models.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_enums.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_Models_Data::MissionInfos MissionInfos;

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions constructor
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_ListView_Missions::ADN_ListView_Missions( E_EntityType eEntityType, QWidget* parent )
    : ADN_ListView  ( parent, "ADN_ListView_Missions", tools::translate( "ADN_ListView_Missions", "Missions" ) )
    , eEntityType_  ( eEntityType )
{
    pConnector_.reset( new ADN_Connector_ListView< MissionInfos >( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions destructor
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_ListView_Missions::~ADN_ListView_Missions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions::ConnectItem
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
void ADN_ListView_Missions::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Models_GUI::eNbrMissionGuiElements );
    MissionInfos* pInfos = static_cast< MissionInfos* >( pCurData_ );
    vItemConnectors_[ADN_Models_GUI::eOrders]->Connect( &pInfos->vOrders_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions::OnContextMenu
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
void ADN_ListView_Missions::OnContextMenu( const QPoint& pt )
{
    ADN_Gui_Tools::GenerateStandardEditionDialog< ADN_Missions_ABC, ADN_Models_Data::MissionInfos >(
        *this, pt, std::string( ADN_Tr::ConvertFromEntityType( eEntityType_, ENT_Tr::eToSim ) + "-list" ).c_str(), tools::translate( "ADN_ListView_Missions", "Missions" ),
        ADN_Tr::ConvertFromEntityType( eEntityType_ ).c_str(), ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( static_cast< E_MissionType >( eEntityType_ ) ) );
}
