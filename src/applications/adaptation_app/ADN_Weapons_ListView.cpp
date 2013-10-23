// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapons_ListView.cpp $
// $Author: Nld $
// $Modtime: 3/05/05 9:59 $
// $Revision: 10 $
// $Workfile: ADN_Weapons_ListView.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Weapons_ListView.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Weapons_Data.h"
#include "ADN_Weapons_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Wizard.h"
#include "ADN_Weapons_WizardPage.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_Weapons_Data_WeaponInfos WeaponInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Weapons_ListView::ADN_Weapons_ListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Weapons_ListView", ADN_Tr::ConvertFromWorkspaceElement( eWeapons ).c_str() )
{
    pConnector_.reset( new ADN_Connector_ListView< WeaponInfos >( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_ListView destructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Weapons_ListView::~ADN_Weapons_ListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_ListView::ConnectItem
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
void ADN_Weapons_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    WeaponInfos* pInfos = (WeaponInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Weapons_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_Weapons_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eLauncher]->Connect( &pInfos->ptrLauncher_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eAmmo]->Connect( &pInfos->ptrAmmunition_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eRoundsPerBurst]->Connect( &pInfos->nRoundsPerBurst_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eBurstDuration]->Connect( &pInfos->burstDuration_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eRoundsPerReload]->Connect( &pInfos->nRoundsPerReload_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eReloadDuration]->Connect( &pInfos->reloadDuration_, bConnect );

    // Order is important here.
    vItemConnectors_[ADN_Weapons_GUI::ePhsGraph]->Connect( &pInfos->phs_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::ePhs]->Connect( &pInfos->phs_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eAverageSpeed]->Connect( &pInfos->rAverageSpeed_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eMinRange]->Connect( &pInfos->rMinRange_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eMaxRange]->Connect( &pInfos->rMaxRange_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eDirect]->Connect( &pInfos->bDirect_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eIndirect]->Connect( &pInfos->bIndirect_, bConnect );

    vItemConnectors_[ADN_Weapons_GUI::eSimulation]->Connect( &pInfos->bSimulation_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eFirePosture]->Connect( &pInfos->nFirePosture_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eTargetPosture]->Connect( &pInfos->nTargetPosture_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eExperience]->Connect( &pInfos->nExperience_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eTiredness]->Connect( &pInfos->nTiredness_, bConnect );
    vItemConnectors_[ADN_Weapons_GUI::eStress]->Connect( &pInfos->nStress_, bConnect );

}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_ListView::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Weapons_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< WeaponInfos, ADN_Weapons_WizardPage > wizard( ADN_Tr::ConvertFromWorkspaceElement( eWeapons ).c_str(), ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        WeaponInfos* pCastData = static_cast< WeaponInfos* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                      ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ), eEquipments );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_ListView::GetToolTipFor
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
std::string ADN_Weapons_ListView::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    WeaponInfos* pCastData = static_cast< WeaponInfos* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                            ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ) );
}
