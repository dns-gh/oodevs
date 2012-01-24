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
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Models_Data.h"
#include "ADN_Models_GUI.h"
#include "ADN_Workspace.h"
#include "ADN_ListView_Models.h"
#include "ADN_Mission_ConfigurationDlg.h"
#include "ADN_Mission_CheckItem.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"

typedef ADN_Models_Data::MissionInfos MissionInfos;

//-----------------------------------------------------------------------------
// Internal List View connector
//-----------------------------------------------------------------------------
class ADN_CLV_Missions : public ADN_Connector_ListView_ABC
{
public:
    ADN_CLV_Missions(ADN_ListView_Missions& list)
        : ADN_Connector_ListView_ABC(list) {}

    virtual ~ADN_CLV_Missions() {}

    ADN_ListViewItem* CreateItem(void * obj)
    {
        ADN_ListViewItem *pItem = new ADN_ListViewItem(&list_,obj,1);
        pItem->Connect(0,&static_cast<MissionInfos*>(obj)->strName_);
        return pItem;
    }
private:
    ADN_CLV_Missions& operator=( const ADN_CLV_Missions& );
};


// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions constructor
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_ListView_Missions::ADN_ListView_Missions( ADN_Models_Data::ModelInfos::E_ModelEntityType eEntityType, QWidget * parent /* = 0*/, const char * name , Qt::WFlags f )
    : ADN_ListView  ( parent, name, f )
    , eEntityType_  ( eEntityType )
    , currentMissions_( 0 )
{
    // add one column
    addColumn( tools::translate( "ADN_ListView_Missions", "Missions"));
    setResizeMode(Q3ListView::AllColumns);

    // connector creation
    pConnector_=new ADN_CLV_Missions(*this);

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

    if( eEntityType_ == ADN_Models_Data::ModelInfos::ePawn )
    {
        pParent = new Q3CheckListItem( pMissionList, tools::translate( "ADN_ListView_Missions", "Unit" ), Q3CheckListItem::CheckBoxController );
        FillList( pParent, ADN_Workspace::GetWorkspace().GetMissions().GetData().GetUnitMissions() );
    }
    else if( eEntityType_ == ADN_Models_Data::ModelInfos::eAutomat )
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
ADN_ListViewItem* ADN_ListView_Missions::FindItem( const std::string& strMissionName )
{
    ADN_ListViewItem* pItem = static_cast< ADN_ListViewItem* >( firstChild() );
    while( pItem != 0 )
    {
        if( strMissionName == pItem->text( 0 ).ascii() )
            return pItem;

        pItem = static_cast< ADN_ListViewItem* >( pItem->nextSibling() );
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
