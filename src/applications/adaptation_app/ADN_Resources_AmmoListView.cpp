// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Resources_AmmoListView.h"
#include "ADN_ActiveProtections_Data.h"
#include "ADN_Categories_Data.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Resources_Wizard.h"
#include "ADN_Resources_GUI.h"
#include "ADN_Objects_Data.h"
#include "ADN_WeaponFilter.h"
#include "ADN_Weapons_Data_WeaponInfos.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"
#include "ENT/ENT_Tr.h"
#include <boost/bind.hpp>

typedef ADN_Resources_Data::AmmoCategoryInfo AmmoCategoryInfo;
typedef ADN_Resources_Data::ResourceInfos ResourceInfos;
typedef ADN_Resources_Data::CategoryInfo CategoryInfo;

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AmmoListView constructor
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Resources_AmmoListView::ADN_Resources_AmmoListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Resources_AmmoListView", ENT_Tr::ConvertFromDotationCategory( sword::category_ammunition, ENT_Tr::eToTr ).c_str() )
{
    pConnector_.reset( new ADN_Connector_ListView<AmmoCategoryInfo>( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AmmoListView destructor
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Resources_AmmoListView::~ADN_Resources_AmmoListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AmmoListView::ConnectItem
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Resources_AmmoListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    AmmoCategoryInfo* pInfos = static_cast< AmmoCategoryInfo* >( pCurData_ );
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Resources_GUI::eNbrAmmoGuiElements );

    vItemConnectors_[ADN_Resources_GUI::eAmmoName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eEMAT6Code]->Connect( &pInfos->strCodeEMAT6_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eEMAT8Code]->Connect( &pInfos->strCodeEMAT8_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eLFRILCode]->Connect( &pInfos->strCodeLFRIL_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eNNOCode]->Connect( &pInfos->strCodeNNO_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eType]->Connect( &pInfos->nType_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eNature]->Connect( &pInfos->ptrResourceNature_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eLogisticSupplyClass]->Connect( &pInfos->ptrLogisticSupplyClass_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eAmmoPackageNbr    ]->Connect( &pInfos->rNbrInPackage_ , bConnect );
    vItemConnectors_[ADN_Resources_GUI::eAmmoPackageWeight ]->Connect( &pInfos->rPackageWeight_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eAmmoPackageVolume ]->Connect( &pInfos->rPackageVolume_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eAttritions]->Connect( &pInfos->attritions_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eUrbanAttritions]->Connect( &pInfos->modifUrbanBlocks_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eIsIED]->Connect( &pInfos->bIED_, bConnect );

    vItemConnectors_[ADN_Resources_GUI::eExplosivePresent]->Connect( &pInfos->indirectAmmoInfos_.bExplosive_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eSmokePresent]->Connect( &pInfos->indirectAmmoInfos_.bSmoke_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eFlarePresent]->Connect( &pInfos->indirectAmmoInfos_.bFlare_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eEffectPresent]->Connect( &pInfos->indirectAmmoInfos_.bEffect_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eIntervention]->Connect( &pInfos->indirectAmmoInfos_.nIntervention_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eDispersionX]->Connect( &pInfos->indirectAmmoInfos_.rDispersionX_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eDispersionY]->Connect( &pInfos->indirectAmmoInfos_.rDispersionY_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eDetectionRange]->Connect( &pInfos->indirectAmmoInfos_.rDetectionRange_, bConnect );

    vItemConnectors_[ADN_Resources_GUI::eModifStances]->Connect( &pInfos->indirectAmmoInfos_.vModifStance_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eNeutralizationRatio]->Connect( &pInfos->indirectAmmoInfos_.rNeutralizationRatio_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eExplosiveDispersion]->Connect( &pInfos->indirectAmmoInfos_.rExplosiveDispersion_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eFlareDeployTime]->Connect( &pInfos->indirectAmmoInfos_.flareDeployTime_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eFlareLifetime]->Connect( &pInfos->indirectAmmoInfos_.flareLifeTime_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eFlareDispersion]->Connect( &pInfos->indirectAmmoInfos_.rFlareDispersion_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eSmokeDeployTime]->Connect( &pInfos->indirectAmmoInfos_.smokeDeployTime_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eSmokeLifetime]->Connect( &pInfos->indirectAmmoInfos_.smokeLifeTime_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eSmokeDispersion]->Connect( &pInfos->indirectAmmoInfos_.rSmokeDispersion_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eMineNumber]->Connect( &pInfos->indirectAmmoInfos_.nMineNumber_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eEffectType]->Connect( &pInfos->indirectAmmoInfos_.objectType_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eEffectLifetime]->Connect( &pInfos->indirectAmmoInfos_.effectLifeTime_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eNetworkUsable]->Connect( &pInfos->bNetworkUsable_, bConnect );

    vItemConnectors_[ADN_Resources_GUI::eArmor]->Connect( &ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Armors_Data >( eArmors ).GetArmorsInfos(), bConnect );
    vItemConnectors_[ADN_Resources_GUI::eMaterial]->Connect( &pInfos->modifUrbanBlocks_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eAttritionGraph]->Connect( &pInfos->attritions_, bConnect );

    // Connect those at the end so that the items in the associated group boxes
    // are correctly enabled / disabled according to those values.
    static ADN_WeaponFilter direct;
    direct.Initialize( *pInfos, true );
    vItemConnectors_[ADN_Resources_GUI::eDirect]->Connect( &direct, bConnect );
    static ADN_WeaponFilter indirect;
    indirect.Initialize( *pInfos, false );
    vItemConnectors_[ADN_Resources_GUI::eIndirect]->Connect( &indirect, bConnect );

    vItemConnectors_[ADN_Resources_GUI::eRange]->Connect( &pInfos->fRange_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eMaintainIllumination]->Connect( &pInfos->bMaintainIllumination_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eIlluminating]->Connect( &pInfos->bIlluminating_, bConnect );

    vItemConnectors_[ADN_Resources_GUI::eMaintainGuidance]->Connect( &pInfos->bMaintainGuidance_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eGuidanceRange]->Connect( &pInfos->fGuidanceRange_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eGuided]->Connect( &pInfos->bGuided_, bConnect );

    if( bConnect )
        ADN_Workspace::GetWorkspace().GetResources().GetGui().InitializeSimulationCombos();
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AmmoListView::OnContextMenu
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Resources_AmmoListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ResourceInfos& dotation = ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( sword::category_ammunition );
    ADN_Resources_Wizard wizard( dotation, ADN_Workspace::GetWorkspace().GetResources().GetData().GetResources(), ENT_Tr::ConvertFromDotationCategory( sword::category_ammunition, ENT_Tr::eToTr ).c_str(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        AmmoCategoryInfo* pCastData = static_cast< AmmoCategoryInfo* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eActiveProtections ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().GetActiveProtectionsThatUse( *pCastData ), eActiveProtections );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eWeapons ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponThatUse( *pCastData ), eWeapons );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eObjects ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectsThatUse( *pCastData ), eObjects );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ), eEquipments );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AmmoListView::GetToolTipFor
// Created: SBO 2005-09-06
// -----------------------------------------------------------------------------
std::string ADN_Resources_AmmoListView::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    AmmoCategoryInfo* pCastData = static_cast< AmmoCategoryInfo* >( pData );
    assert( pCastData != 0 );

    std::string result;
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eActiveProtections ).c_str(), ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().GetActiveProtectionsThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eWeapons ).c_str(), ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eObjects ).c_str(), ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectsThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(), ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ), result );

    if( result.empty() )
        result = tools::translate( "ADN_Resources_AmmoListView", "<b>Unused</b>" ).toStdString();
    return result;
}
