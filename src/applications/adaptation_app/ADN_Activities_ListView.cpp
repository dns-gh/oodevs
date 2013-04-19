// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Activities_ListView.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Missions_GUI.h"
#include "ADN_Activities_Data.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"

typedef ADN_Activities_Data::ActivityInfos Activity;

// -----------------------------------------------------------------------------
// Name: ADN_Activities_ListView constructor
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_ListView::ADN_Activities_ListView( QWidget* pParent /* = 0 */, const char* szName /* = 0 */, Qt::WFlags f /* = 0 */ )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tools::translate( "ADN_Activities_ListView", "Activities" ) );
    setResizeMode( Q3ListView::AllColumns );
    // Connector creation
    pConnector_ = new ADN_Connector_ListView< Activity >( *this );
    SetDeletionEnabled( true );
}
// -----------------------------------------------------------------------------
// Name: ADN_Activities_ListView destructor
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_ListView::~ADN_Activities_ListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_ListView::ConnectItem
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Activities_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    Activity* pInfos = ( Activity* )pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Missions_GUI::eNbrActivityGuiElements );

    vItemConnectors_[ADN_Missions_GUI::eActivityName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eActivityDescription]->Connect( &pInfos->description_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eActivityMelee]->Connect( &pInfos->meleeActivity_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::ePackage]->Connect( &pInfos->package_, bConnect );

}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_ListView::OnContextMenu
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Activities_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< Activity > wizard( tr( "Activities" ), ADN_Workspace::GetWorkspace().GetMissions().GetData().activitiesData_->GetActivities(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
