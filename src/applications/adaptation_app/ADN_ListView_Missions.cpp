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
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_ListView_Missions.cpp $
// $Author: Nld $
// $Modtime: 29/04/05 16:26 $
// $Revision: 7 $
// $Workfile: ADN_ListView_Missions.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Missions.h"
#include "ADN_App.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Models_Data.h"
#include "ADN_Models_GUI.h"
#include "ADN_Workspace.h"
#include "ADN_ListView_Models.h"
#include "ADN_Mission_ConfigurationDlg.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_enums.h"

typedef ADN_Models_Data::MissionInfos MissionInfos;

Q_DECLARE_METATYPE( ADN_Missions_Mission* )
Q_DECLARE_METATYPE( MissionInfos* )

#define MissionTypeRole ( Qt::UserRole )
#define MissionModelRole ( Qt::UserRole + 1 )

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions constructor
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_ListView_Missions::ADN_ListView_Missions( E_EntityType eEntityType, QWidget* parent )
    : ADN_ListView  ( parent, "ADN_ListView_Missions", tools::translate( "ADN_ListView_Missions", "Missions") )
    , eEntityType_  ( eEntityType )
    , currentMissions_( 0 )
{
    // connector creation
    pConnector_ = new ADN_Connector_ListView< MissionInfos >(*this);

    this->SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions destructor
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_ListView_Missions::~ADN_ListView_Missions()
{
    delete pConnector_;
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
    if( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Normal )
        return;

    Q3PopupMenu menu;
    menu.insertItem( tools::translate( "ADN_ListView_Missions", "Configure missions" ), 1 );

    if( menu.exec( pt ) != 1 )
        return;

    ADN_Mission_ConfigurationDlg cfgDlg( this );

    QTreeWidget* pMissionList = cfgDlg.GetMissionList();
    QTreeWidgetItem* item = new QTreeWidgetItem( pMissionList );
    item->setFlags( item->flags() |  Qt::ItemIsTristate );

    if( eEntityType_ == eEntityType_Pawn )
    {
        item->setText( 0, tools::translate( "ADN_ListView_Missions", "Unit" ) );
        FillList( item, ADN_Workspace::GetWorkspace().GetMissions().GetData().GetUnitMissions() );
    }
    else if( eEntityType_ == eEntityType_Automat )
    {
        item->setText( 0, tools::translate( "ADN_ListView_Missions", "Automate" ) );
        FillList( item, ADN_Workspace::GetWorkspace().GetMissions().GetData().GetAutomatMissions() );
    }
    else
    {
        item->setText( 0, tools::translate( "ADN_ListView_Missions", "Crowd" ) );
        FillList( item, ADN_Workspace::GetWorkspace().GetMissions().GetData().GetPopulationMissions() );
    }
    pMissionList->expandAll();
    pMissionList->sortItems( 0, Qt::AscendingOrder );
    if( cfgDlg.exec() != QDialog::Accepted )
        return;
    ApplyModifications( item );
    currentMissions_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions::FillList
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
void ADN_ListView_Missions::FillList( QTreeWidgetItem* pParent, ADN_Missions_Data::T_Mission_Vector& missions )
{
    currentMissions_ = &missions;
    for( ADN_Missions_Data::IT_Mission_Vector it = missions.begin(); it != missions.end(); ++it )
    {
        ADN_Missions_Mission* mission = *it;
        QTreeWidgetItem* item = new QTreeWidgetItem( pParent );
        item->setFlags( item->flags() | Qt::ItemIsUserCheckable );
        item->setText( 0, mission->strName_.GetData().c_str() );
        item->setData( 0, MissionTypeRole, QVariant::fromValue( mission ) );
        if( ADN_ListViewItem* pMission = FindItem( mission->strName_.GetData().c_str() ) )
        {
            item->setCheckState( 0, Qt::Checked );
            item->setData( 0, MissionModelRole, QVariant::fromValue( static_cast< MissionInfos* >( pMission->GetData() ) ) );
        }
        else
        {
            item->setCheckState( 0, Qt::Unchecked );
            item->setData( 0, MissionModelRole, QVariant::fromValue( static_cast< MissionInfos* >( 0 ) ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions::FindItem
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
ADN_StandardItem* ADN_ListView_Missions::FindItem( const std::string& strMissionName )
{
    const int rowCount = dataModel_.rowCount();
    for( int row = 0; row < rowCount; ++row )
    {
        ADN_StandardItem* pItem = static_cast< ADN_StandardItem* >( dataModel_.item( row ) );
        if( strMissionName == pItem->text().toStdString() )
            return pItem;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions::ApplyModifications
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
void ADN_ListView_Missions::ApplyModifications( QTreeWidgetItem* pStart )
{
    if( !currentMissions_ )
        return;
    for( int i = 0; i < pStart->childCount(); ++i )
    {
        QTreeWidgetItem* item = pStart->child( i );
        MissionInfos* infos = item->data( 0, MissionModelRole ).value< MissionInfos* >();
        if( item->checkState( 0 ) == Qt::Checked )
        {
            if( infos == 0 )
            {
                // create a new mission
                MissionInfos* pNewInfo = new MissionInfos( *currentMissions_ );
                pNewInfo->mission_ = item->data( 0, MissionTypeRole ).value< ADN_Missions_Mission* >();
                pNewInfo->strName_ = pNewInfo->mission_.GetData()->strName_.GetData();
                static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->AddItem( pNewInfo );
            }
        }
        else
        {
            if( infos != 0 )
            {
                // delete the mission
                static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( infos );
            }
        }
    }
}
