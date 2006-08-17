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

#include "ADN_pch.h"
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

#include <qheader.h>
#include <qpopmenu.h>

typedef ADN_Models_Data::MissionInfos MissionInfos;

//-----------------------------------------------------------------------------
// Internal List View connector
//-----------------------------------------------------------------------------
class ADN_CLV_Missions
: public ADN_Connector_ListView_ABC
{

public:

    ADN_CLV_Missions(ADN_ListView_Missions& list)
    : ADN_Connector_ListView_ABC(list)
    {}

    virtual ~ADN_CLV_Missions()
    {}

    ADN_ListViewItem* CreateItem(void * obj)
    {
        // create new list item
        ADN_ListViewItem *pItem                 = new ADN_ListViewItem(&list_,obj,1);

        // connect list item with object's name
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
ADN_ListView_Missions::ADN_ListView_Missions( ADN_Models_Data::ModelInfos::E_ModelEntityType eEntityType, ADN_ListView_Models* pList, QWidget * parent /*= 0*/, const char * name , WFlags f )
: ADN_ListView  (parent,name,f)
, pLVModels_    ( pList )
, eEntityType_  ( eEntityType )
{
    // add one column
    addColumn( tr( "Missions"));
    setResizeMode(QListView::AllColumns);

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
    QPopupMenu menu;
    menu.insertItem( tr( "Configure missions" ), 1 );

    if( menu.exec( pt ) != 1 )
        return;

    ADN_Mission_ConfigurationDlg cfgDlg( this );

    QListView* pMissionList = cfgDlg.GetMissionList();

    // Unit missions
    if( eEntityType_ == ADN_Models_Data::ModelInfos::ePawn )
    {
        // first, the missions' pion
        QCheckListItem* pPion = new QCheckListItem( pMissionList, "Pion", QCheckListItem::CheckBoxController );
        pPion->setOpen( true );

        FillList( pPion, eSMission_Pion_Common_Debut, eSMission_Pion_Common_Fin );

        // ABC
        QCheckListItem* pSubPion0 = new QCheckListItem( pPion, "ABC", QCheckListItem::CheckBoxController );
        FillList( pSubPion0, eSMission_Pion_ABC_Debut, eSMission_Pion_ABC_Fin );

        //ASY
        QCheckListItem* pSubPion00 = new QCheckListItem( pPion, "ASY", QCheckListItem::CheckBoxController );
        FillList( pSubPion00, eSMission_Pion_ASY_Debut, eSMission_Pion_ASY_Fin );

        // INF
        QCheckListItem* pSubPion1 = new QCheckListItem( pPion, "INF", QCheckListItem::CheckBoxController );
        FillList( pSubPion1, eSMission_Pion_INF_Debut, eSMission_Pion_INF_Fin );

        // GEN
        QCheckListItem* pSubPion2 = new QCheckListItem( pPion, "GEN", QCheckListItem::CheckBoxController );
        FillList( pSubPion2, eSMission_Pion_GEN_Debut, eSMission_Pion_GEN_Fin );

        // 3D
        QCheckListItem* pSubPion3 = new QCheckListItem( pPion, "3D", QCheckListItem::CheckBoxController );
        FillList( pSubPion3, eSMission_Pion_3D_Debut, eSMission_Pion_3D_Fin );

        // ALAT
        QCheckListItem* pSubPion4 = new QCheckListItem( pPion, "ALAT", QCheckListItem::CheckBoxController );
        FillList( pSubPion4, eSMission_Pion_ALAT_Debut, eSMission_Pion_ALAT_Fin );

        // LOG
        QCheckListItem* pSubPion5 = new QCheckListItem( pPion, "LOG", QCheckListItem::CheckBoxController );
        FillList( pSubPion5, eSMission_Pion_LOG_Debut, eSMission_Pion_LOG_Fin );

        // RENS
        QCheckListItem* pSubPion6 = new QCheckListItem( pPion, "RENS", QCheckListItem::CheckBoxController );
        FillList( pSubPion6, eSMission_Pion_RENS_Debut, eSMission_Pion_RENS_Fin );

        // NBC
        QCheckListItem* pSubPion7 = new QCheckListItem( pPion, "NBC", QCheckListItem::CheckBoxController );
        FillList( pSubPion7, eSMission_Pion_NBC_Debut, eSMission_Pion_NBC_Fin );

        // MILICE
        QCheckListItem* pSubPion8 = new QCheckListItem( pPion, "MILICE", QCheckListItem::CheckBoxController );
        FillList( pSubPion8, eSMission_Pion_MILICE_Debut, eSMission_Pion_MILICE_Fin );

        // JOINT
        QCheckListItem* pSubPion9 = new QCheckListItem( pPion, "JOINT", QCheckListItem::CheckBoxController );
        FillList( pSubPion9, eSMission_Pion_JOINT_Debut, eSMission_Pion_JOINT_Fin );

        int nResult = cfgDlg.exec();

        if( nResult != QDialog::Accepted )
            return;

        ApplyModifications( pPion );
        ApplyModifications( pSubPion0 );
        ApplyModifications( pSubPion00 );
        ApplyModifications( pSubPion1 );
        ApplyModifications( pSubPion2 );
        ApplyModifications( pSubPion3 );
        ApplyModifications( pSubPion4 );
        ApplyModifications( pSubPion5 );
        ApplyModifications( pSubPion6 );
        ApplyModifications( pSubPion7 );
        ApplyModifications( pSubPion8 );
        ApplyModifications( pSubPion9 );
    }
    else if( eEntityType_ == ADN_Models_Data::ModelInfos::eAutomat )
    {
        // Automate missions
        QCheckListItem* pAutomate = new QCheckListItem( pMissionList, "Automate", QCheckListItem::CheckBoxController );
        pAutomate->setOpen( true );

        FillList( pAutomate, eSMission_Automate_Common_Debut, eSMission_Automate_Common_Fin );

        // ABC
        QCheckListItem* pSubAutomate0 = new QCheckListItem( pAutomate, "ABC", QCheckListItem::CheckBoxController );
        FillList( pSubAutomate0, eSMission_Automate_ABC_Debut, eSMission_Automate_ABC_Fin );

        // ASY
        QCheckListItem* pSubAutomate00 = new QCheckListItem( pAutomate, "ASY", QCheckListItem::CheckBoxController );
        FillList( pSubAutomate00, eSMission_Automate_ASY_Debut, eSMission_Automate_ASY_Fin );
        
        // INF
        QCheckListItem* pSubAutomate1 = new QCheckListItem( pAutomate, "INF", QCheckListItem::CheckBoxController );
        FillList( pSubAutomate1, eSMission_Automate_INF_Debut, eSMission_Automate_INF_Fin );

        // GEN
        QCheckListItem* pSubAutomate2 = new QCheckListItem( pAutomate, "GEN", QCheckListItem::CheckBoxController );
        FillList( pSubAutomate2, eSMission_Automate_GEN_Debut, eSMission_Automate_GEN_Fin );

        // 3D
        QCheckListItem* pSubAutomate3 = new QCheckListItem( pAutomate, "3D", QCheckListItem::CheckBoxController );
        FillList( pSubAutomate3, eSMission_Automate_3D_Debut, eSMission_Automate_3D_Fin );

        // ALAT
        QCheckListItem* pSubAutomate4 = new QCheckListItem( pAutomate, "ALAT", QCheckListItem::CheckBoxController );
        FillList( pSubAutomate4, eSMission_Automate_ALAT_Debut, eSMission_Automate_ALAT_Fin );

        // LOG
        QCheckListItem* pSubAutomate5 = new QCheckListItem( pAutomate, "LOG", QCheckListItem::CheckBoxController );
        FillList( pSubAutomate5, eSMission_Automate_LOG_Debut, eSMission_Automate_LOG_Fin );

        // RENS
        QCheckListItem* pSubAutomate6 = new QCheckListItem( pAutomate, "RENS", QCheckListItem::CheckBoxController );
        FillList( pSubAutomate6, eSMission_Automate_RENS_Debut, eSMission_Automate_RENS_Fin );

        // NBC
        QCheckListItem* pSubAutomate7 = new QCheckListItem( pAutomate, "NBC", QCheckListItem::CheckBoxController );
        FillList( pSubAutomate7, eSMission_Automate_NBC_Debut, eSMission_Automate_NBC_Fin );

        // MILICE
        QCheckListItem* pSubAutomate8 = new QCheckListItem( pAutomate, "MILICE", QCheckListItem::CheckBoxController );
        FillList( pSubAutomate8, eSMission_Automate_MILICE_Debut, eSMission_Automate_MILICE_Fin );

        // JOINT
        QCheckListItem* pSubAutomate9 = new QCheckListItem( pAutomate, "JOINT", QCheckListItem::CheckBoxController );
        FillList( pSubAutomate9, eSMission_Automate_JOINT_Debut, eSMission_Automate_JOINT_Fin );

        int nResult = cfgDlg.exec();

        if( nResult != QDialog::Accepted )
            return;

        ApplyModifications( pAutomate );
        ApplyModifications( pSubAutomate0 );
        ApplyModifications( pSubAutomate00 );
        ApplyModifications( pSubAutomate1 );
        ApplyModifications( pSubAutomate2 );
        ApplyModifications( pSubAutomate3 );
        ApplyModifications( pSubAutomate4 );
        ApplyModifications( pSubAutomate5 );
        ApplyModifications( pSubAutomate6 );
        ApplyModifications( pSubAutomate7 );
        ApplyModifications( pSubAutomate8 );
        ApplyModifications( pSubAutomate9 );
    }
    // population
    else
    {
        // first, the missions' population
        QCheckListItem* pPopulation = new QCheckListItem( pMissionList, "Population", QCheckListItem::CheckBoxController );
        pPopulation->setOpen( true );

        FillList( pPopulation, eSMission_Population_Common_Debut, eSMission_Population_Common_Fin );

        int nResult = cfgDlg.exec();

        if( nResult != QDialog::Accepted )
            return;

        ApplyModifications( pPopulation );
    }

}


// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions::FillList
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
void ADN_ListView_Missions::FillList( QCheckListItem* pParent, E_SMission eStart, E_SMission eEnd )
{
    for( int n = eStart + 1; n < eEnd; ++n )
    {
        std::string strMissionName = ADN_Tr::ConvertFromSMission( (E_SMission)n, ENT_Tr_ABC::eToTr );
        ADN_ListViewItem* pMission = FindItem( strMissionName );

        if( pMission == 0 )
        {
            new ADN_Mission_CheckItem( pParent, (E_SMission)n );
        }
        else
        {
            new ADN_Mission_CheckItem( pParent, (E_SMission)n, static_cast< MissionInfos* >( pMission->GetData() ) );
        }
    }
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
void ADN_ListView_Missions::ApplyModifications( QCheckListItem* pStart )
{
    QListViewItem* pItem = pStart->firstChild();
    while( pItem != 0 )
    {
        if( pItem->rtti() == 1 && static_cast< QCheckListItem* >( pItem )->type() == QCheckListItem::CheckBox )
        {
            ADN_Mission_CheckItem* pMissionItem = static_cast< ADN_Mission_CheckItem* >( pItem );
            if( pMissionItem->isOn() )
            {
                if( pMissionItem->GetMissionPtr() == 0 )
                {
                    // create a new mission
                    MissionInfos* pNewInfo = new MissionInfos();
                    pNewInfo->nMissionType_ = pMissionItem->GetMission();
                    pNewInfo->strName_ = ADN_Tr::ConvertFromSMission( (E_SMission)(pMissionItem->GetMission()), ENT_Tr_ABC::eToTr );

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
