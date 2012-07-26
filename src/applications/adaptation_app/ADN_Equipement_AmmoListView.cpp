// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_AmmoListView.cpp $
// $Author: Nld $
// $Modtime: 20/07/05 14:21 $
// $Revision: 10 $
// $Workfile: ADN_Equipement_AmmoListView.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipement_AmmoListView.h"
#include "ADN_Weapons_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Equipement_Wizard.h"
#include "ADN_Equipement_GUI.h"
#include "ADN_WeaponFilter.h"
#include "ADN_Tr.h"
#include <boost/bind.hpp>

typedef ADN_Equipement_Data::AmmoCategoryInfo AmmoCategoryInfo;
typedef ADN_Equipement_Data::ResourceInfos ResourceInfos;
typedef ADN_Equipement_Data::CategoryInfo CategoryInfo;

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AmmoListView constructor
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Equipement_AmmoListView::ADN_Equipement_AmmoListView( QWidget* pParent, const char* szName, Qt::WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tools::translate( "ADN_Equipement_AmmoListView", "Ammunition" ) );
    setResizeMode( Q3ListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView<AmmoCategoryInfo>( *this );

    this->SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AmmoListView destructor
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Equipement_AmmoListView::~ADN_Equipement_AmmoListView()
{
    delete pConnector_;
}

namespace
{
    bool Matches( bool direct, const ADN_Weapons_Data::WeaponInfos& weapon, const AmmoCategoryInfo* pAmmo )
    {
        if( weapon.ptrAmmunition_.GetData() != pAmmo )
            return false;
        if( direct )
            return weapon.bDirect_.GetData();
        return weapon.bIndirect_.GetData();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AmmoListView::ConnectItem
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Equipement_AmmoListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    AmmoCategoryInfo* pInfos = (AmmoCategoryInfo*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Equipement_GUI::eNbrAmmoGuiElements );

    vItemConnectors_[ADN_Equipement_GUI::eAmmoName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eEMAT6Code]->Connect( &pInfos->strCodeEMAT6_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eEMAT8Code]->Connect( &pInfos->strCodeEMAT8_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eLFRILCode]->Connect( &pInfos->strCodeLFRIL_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eNNOCode]->Connect( &pInfos->strCodeNNO_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eType]->Connect( &pInfos->nType_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eNature]->Connect( &pInfos->ptrResourceNature_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eLogisticSupplyClass]->Connect( &pInfos->ptrLogisticSupplyClass_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eAmmoPackageNbr    ]->Connect( &pInfos->rNbrInPackage_ , bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eAmmoPackageWeight ]->Connect( &pInfos->rPackageWeight_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eAmmoPackageVolume ]->Connect( &pInfos->rPackageVolume_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eAttritions]->Connect( &pInfos->attritions_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eUrbanAttritions]->Connect( &pInfos->modifUrbanBlocks_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eIsIED]->Connect( &pInfos->bIED_, bConnect );

    vItemConnectors_[ADN_Equipement_GUI::eExplosivePresent]->Connect( &pInfos->indirectAmmoInfos_.bExplosive_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eSmokePresent]->Connect( &pInfos->indirectAmmoInfos_.bSmoke_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eFlarePresent]->Connect( &pInfos->indirectAmmoInfos_.bFlare_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eMinePresent]->Connect( &pInfos->indirectAmmoInfos_.bMine_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eEffectPresent]->Connect( &pInfos->indirectAmmoInfos_.bEffect_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eIntervention]->Connect( &pInfos->indirectAmmoInfos_.nIntervention_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eDispersionX]->Connect( &pInfos->indirectAmmoInfos_.rDispersionX_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eDispersionY]->Connect( &pInfos->indirectAmmoInfos_.rDispersionY_, bConnect );

    vItemConnectors_[ADN_Equipement_GUI::eModifStances]->Connect( &pInfos->indirectAmmoInfos_.vModifStance_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eNeutralizationRatio]->Connect( &pInfos->indirectAmmoInfos_.rNeutralizationRatio_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eDispersionFactor]->Connect( &pInfos->indirectAmmoInfos_.rDispersionFactor_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eFlareDeployTime]->Connect( &pInfos->indirectAmmoInfos_.flareDeployTime_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eFlareLifetime]->Connect( &pInfos->indirectAmmoInfos_.flareLifeTime_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eSmokeDeployTime]->Connect( &pInfos->indirectAmmoInfos_.smokeDeployTime_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eSmokeLifetime]->Connect( &pInfos->indirectAmmoInfos_.smokeLifeTime_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eMineNumber]->Connect( &pInfos->indirectAmmoInfos_.nMineNumber_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eEffectType]->Connect( &pInfos->indirectAmmoInfos_.objectType_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eEffectLifetime]->Connect( &pInfos->indirectAmmoInfos_.effectLifeTime_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eNetworkUsable]->Connect( &pInfos->bNetworkUsable_, bConnect );

    vItemConnectors_[ADN_Equipement_GUI::eArmor]->Connect( &ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos(), bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eMaterial]->Connect( &pInfos->modifUrbanBlocks_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eAttritionGraph]->Connect( &pInfos->attritions_, bConnect );

    // Connect those at the end so that the items in the associated group boxes
    // are correctly enabled / disabled according to those values.
    static ADN_WeaponFilter direct;
    direct.Initialize( &pInfos->bDirect_, boost::bind( &Matches, true, _1, pInfos ) );
    vItemConnectors_[ADN_Equipement_GUI::eDirect]->Connect( &direct, bConnect );
    static ADN_WeaponFilter indirect;
    indirect.Initialize( &pInfos->bIndirect_, boost::bind( &Matches, false, _1, pInfos ) );
    vItemConnectors_[ADN_Equipement_GUI::eIndirect]->Connect( &indirect, bConnect );

    vItemConnectors_[ADN_Equipement_GUI::eRange]->Connect( &pInfos->fRange_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eMaintainIllumination]->Connect( &pInfos->bMaintainIllumination_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eIlluminating]->Connect( &pInfos->bIlluminating_, bConnect );

    vItemConnectors_[ADN_Equipement_GUI::eMaintainGuidance]->Connect( &pInfos->bMaintainGuidance_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eGuidanceRange]->Connect( &pInfos->fGuidanceRange_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eGuided]->Connect( &pInfos->bGuided_, bConnect );

    if( bConnect )
        ADN_Workspace::GetWorkspace().GetEquipements().GetGui().InitializeSimulationCombos();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AmmoListView::OnContextMenu
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Equipement_AmmoListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ResourceInfos& dotation = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Munition );
    ADN_Equipement_Wizard wizard( dotation, this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        AmmoCategoryInfo* pCastData = static_cast< AmmoCategoryInfo* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), tools::translate( "ADN_Equipement_AmmoListView", "Weapon systems" ), ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponThatUse( *pCastData ), eWeapons );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AmmoListView::GetToolTipFor
// Created: SBO 2005-09-06
// -----------------------------------------------------------------------------
std::string ADN_Equipement_AmmoListView::GetToolTipFor( Q3ListViewItem& item )
{
    void* pData = static_cast< ADN_ListViewItem& >( item ).GetData();
    AmmoCategoryInfo* pCastData = static_cast< AmmoCategoryInfo* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eWeapons ).c_str(),
                            ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponThatUse( *pCastData ) );
}
