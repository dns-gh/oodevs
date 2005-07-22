// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
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

#include "ADN_pch.h"
#include "ADN_Equipement_AmmoListView.h"

#include <qpopupmenu.h>

#include "ADN_Connector_ListView.h"
#include "ADN_Equipement_Wizard.h"
#include "ADN_Equipement_GUI.h"

typedef ADN_Equipement_Data::AmmoCategoryInfo AmmoCategoryInfo;
typedef ADN_Equipement_Data::DotationInfos DotationInfos;
typedef ADN_Equipement_Data::CategoryInfo CategoryInfo;

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AmmoListView constructor
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Equipement_AmmoListView::ADN_Equipement_AmmoListView( QWidget* pParent, const char* szName, WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Ammunition" ) );
    setResizeMode( QListView::AllColumns );

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
    vItemConnectors_[ADN_Equipement_GUI::eAmmoPackageNbr    ]->Connect( &pInfos->rNbrInPackage_ , bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eAmmoPackageWeight ]->Connect( &pInfos->rPackageWeight_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eAmmoPackageVolume ]->Connect( &pInfos->rPackageVolume_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eTrancheD]->Connect( &pInfos->bTrancheD_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eAttritions]->Connect( &pInfos->attritions_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eIndirectType]->Connect( &pInfos->indirectAmmoInfos_.nIndirectType_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eIntervention]->Connect( &pInfos->indirectAmmoInfos_.nIntervention_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eDispersionX]->Connect( &pInfos->indirectAmmoInfos_.rDispersionX_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eDispersionY]->Connect( &pInfos->indirectAmmoInfos_.rDispersionY_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eNeutralizationRatio]->Connect( &pInfos->indirectAmmoInfos_.rNeutralizationRatio_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eDeployTime]->Connect( &pInfos->indirectAmmoInfos_.rDeployTime_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eLifetime]->Connect( &pInfos->indirectAmmoInfos_.rLifeTime_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eMineNumber]->Connect( &pInfos->indirectAmmoInfos_.nMineNumber_, bConnect );

    // Connect those at the end so that the items in the associated group boxes
    // are correctly enabled / disabled according to those values.
    vItemConnectors_[ADN_Equipement_GUI::eDirect]->Connect( &pInfos->bDirect_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eIndirect]->Connect( &pInfos->bIndirect_, bConnect );
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AmmoListView::OnContextMenu
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Equipement_AmmoListView::OnContextMenu( const QPoint& pt )
{
    QPopupMenu popupMenu( this );
    DotationInfos& dotation = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eTypeDotation_Munition );
    ADN_Equipement_Wizard wizard( dotation, this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
