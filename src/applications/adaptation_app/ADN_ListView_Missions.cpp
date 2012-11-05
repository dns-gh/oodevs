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
#include "ADN_Mission_CheckItem.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_enums.h"

typedef ADN_Models_Data::MissionInfos MissionInfos;

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
    pConnector_=new ADN_Connector_ListView< MissionInfos >(*this);

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

    MissionInfos* pInfos = (MissionInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Models_GUI::eNbrMissionGuiElements );

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

    Q3ListView* pMissionList = cfgDlg.GetMissionList();
    Q3CheckListItem* pParent = 0;

    if( eEntityType_ == eEntityType_Pawn )
    {
        pParent = new Q3CheckListItem( pMissionList, tools::translate( "ADN_ListView_Missions", "Unit" ), Q3CheckListItem::CheckBoxController );
        FillList( pParent, ADN_Workspace::GetWorkspace().GetMissions().GetData().GetUnitMissions() );
    }
    else if( eEntityType_ == eEntityType_Automat )
    {
        pParent = new Q3CheckListItem( pMissionList, tools::translate( "ADN_ListView_Missions", "Automate" ), Q3CheckListItem::CheckBoxController );
        FillList( pParent, ADN_Workspace::GetWorkspace().GetMissions().GetData().GetAutomatMissions() );
    }
    else
    {
        pParent = new Q3CheckListItem( pMissionList, tools::translate( "ADN_ListView_Missions", "Crowd" ), Q3CheckListItem::CheckBoxController );
        FillList( pParent, ADN_Workspace::GetWorkspace().GetMissions().GetData().GetPopulationMissions() );
    }
    if( cfgDlg.exec() != QDialog::Accepted )
        return;
    ApplyModifications( pParent );
    currentMissions_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions::FillList
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
void ADN_ListView_Missions::FillList( Q3CheckListItem* pParent, ADN_Missions_Data::T_Mission_Vector& missions )
{
    currentMissions_ = &missions;
    pParent->setOpen( true );
    unsigned int n = 0;
    for( ADN_Missions_Data::IT_Mission_Vector it = missions.begin(); it != missions.end(); ++it, ++n )
        if( ADN_ListViewItem* pMission = FindItem( (*it)->strName_.GetData().c_str() ) )
            new ADN_Mission_CheckItem( pParent, **it, static_cast< MissionInfos* >( pMission->GetData() ) );
        else
            new ADN_Mission_CheckItem( pParent, **it );
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
void ADN_ListView_Missions::ApplyModifications( Q3CheckListItem* pStart )
{
    if( !currentMissions_ )
        return;
    Q3ListViewItem* pItem = pStart->firstChild();
    while( pItem != 0 )
    {
        if( pItem->rtti() == 1 && static_cast< Q3CheckListItem* >( pItem )->type() == Q3CheckListItem::CheckBox )
        {
            ADN_Mission_CheckItem* pMissionItem = static_cast< ADN_Mission_CheckItem* >( pItem );
            if( pMissionItem->isOn() )
            {
                if( pMissionItem->GetMissionPtr() == 0 )
                {
                    // create a new mission
                    MissionInfos* pNewInfo = new MissionInfos( *currentMissions_ );
                    pNewInfo->mission_ = &pMissionItem->GetMission();
                    pNewInfo->strName_ = pNewInfo->mission_.GetData()->strName_.GetData();

                    ADN_Connector_Vector_ABC* pCList = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
                    pCList->AddItem( pNewInfo );
                }
            }
            else
            {
                if( pMissionItem->GetMissionPtr() != 0 )
                {
                    // delete the mission
                    static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pMissionItem->GetMissionPtr() );
                }
            }
        }

        pItem = pItem->nextSibling();
    }
}
