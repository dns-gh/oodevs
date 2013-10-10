// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Templates.h"
#include "ADN_Tr.h"
#include "ADN_Urban_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Urban_GUI.h"
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Templates constructor
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
ADN_ListView_Templates::ADN_ListView_Templates( QWidget* pParent )
    :  ADN_ListView( pParent, "Template", tools::translate( "ADN_ListView_Templates", "Template" ) )
{
    pConnector_ = new ADN_Connector_ListView< ADN_Urban_Data::UrbanTemplateInfos >( *this );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Templates destructor
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
ADN_ListView_Templates::~ADN_ListView_Templates()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Templates::ConnectItem
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
void ADN_ListView_Templates::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ADN_Urban_Data::UrbanTemplateInfos* pInfos = ( ADN_Urban_Data::UrbanTemplateInfos* )pCurData_;

    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Urban_GUI::eNbrUrbanUsageGuiElements );

    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageName ]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageColor ]->Connect( &pInfos->color_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageAlpha ]->Connect( &pInfos->alpha_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageHeight ]->Connect( &pInfos->height_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageFloorNumber ]->Connect( &pInfos->floor_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageParkingFloors ]->Connect( &pInfos->parking_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageOccupation ]->Connect( &pInfos->occupation_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageTrafficability ]->Connect( &pInfos->trafficability_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageMaterial ]->Connect( &pInfos->ptrMaterial_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageRoofShape ]->Connect( &pInfos->ptrRoofShape_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageRole ]->Connect( &pInfos->usages_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Templates::OnContextMenu
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
void ADN_ListView_Templates::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ADN_Urban_Data::UrbanTemplateInfos > wizard( tools::translate( "ADN_ListView_Templates", "Templates" ), ADN_Workspace::GetWorkspace().GetUrban().GetData().GetTemplatesInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
