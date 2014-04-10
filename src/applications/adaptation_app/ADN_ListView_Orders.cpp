// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Orders.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:04 $
// $Revision: 7 $
// $Workfile: ADN_ListView_Orders.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Orders.h"

#include "ADN_Connector_ListView.h"
#include "ADN_Gui_Tools.h"
#include "ADN_Missions_ABC.h"
#include "ADN_Missions_Data.h"
#include "ADN_Models_Data.h"
#include "ADN_Workspace.h"
#include "ADN_WorkspaceElement.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders constructor
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_ListView_Orders::ADN_ListView_Orders( QWidget* parent )
    : ADN_ListView( parent,
                    "ADN_ListView_Orders",
                    QString::fromStdString( ENT_Tr::ConvertFromMissionType( eMissionType_FragOrder ) ) )
{
    pConnector_.reset( new ADN_Connector_ListView< ADN_Models_OrderInfos >( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders destructor
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_ListView_Orders::~ADN_ListView_Orders()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders::OnContextMenu
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
void ADN_ListView_Orders::OnContextMenu( const QPoint& pt )
{
    ADN_Gui_Tools::GenerateStandardEditionDialog< ADN_Missions_ABC, ADN_Models_OrderInfos >(
        *this,
        pt,
        objectName() + "-edition-dialog",
        Title(),
        Title(),
        ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_FragOrder ) );
}
