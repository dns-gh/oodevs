// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Packages_ListView.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Missions_GUI.h"
#include "ADN_Activities_Data.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"

typedef ADN_Activities_Data::PackageInfos Package;

// -----------------------------------------------------------------------------
// Name: ADN_Packages_ListView constructor
// Created: NPT 2013-04-17
// -----------------------------------------------------------------------------
ADN_Packages_ListView::ADN_Packages_ListView( QWidget* pParent /*= 0*/, const char* szName /*= 0*/, Qt::WFlags f /*= 0*/ )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Activities" ) );
    setResizeMode( Q3ListView::AllColumns );
    // Connector creation
    pConnector_ = new ADN_Connector_ListView< Package >( *this );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Packages_ListView destructor
// Created: NPT 2013-04-17
// -----------------------------------------------------------------------------
ADN_Packages_ListView::~ADN_Packages_ListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Packages_ListView::ConnectItem
// Created: NPT 2013-04-17
// -----------------------------------------------------------------------------
void ADN_Packages_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    Package* pInfos = ( Package* ) pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Missions_GUI::eNbrPackageGuiElements );
    vItemConnectors_[ ADN_Missions_GUI::ePackageName ]->Connect( &pInfos->strName_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Packages_ListView::OnContextMenu
// Created: NPT 2013-04-17
// -----------------------------------------------------------------------------
void ADN_Packages_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< Package > wizard( tr( "Packages" ), ADN_Workspace::GetWorkspace().GetMissions().GetData().activitiesData_->GetPackages(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
