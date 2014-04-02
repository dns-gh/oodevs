// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Resources_GenericListView.h"
#include "ADN_ActiveProtections_Data.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_Categories_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Fires_Data.h"
#include "ADN_Resources_Data.h"
#include "ADN_Resources_GUI.h"
#include "ADN_Enums.h"
#include "ADN_Resources_Wizard.h"
#include "ADN_Objects_Data.h"
#include "ADN_ResourceNetworks_Data.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Resources_Data::ResourceInfos ResourceInfos;
typedef ADN_Resources_Data::CategoryInfo CategoryInfo;

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GenericListView constructor
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Resources_GenericListView::ADN_Resources_GenericListView( QWidget* pParent, sword::DotationCategory nType )
    : ADN_ListView( pParent, "ADN_Resources_GenericListView", ENT_Tr::ConvertFromDotationCategory( nType, ENT_Tr::eToTr ).c_str() )
    , nType_      ( nType )
{
    pConnector_.reset( new ADN_Connector_ListView<CategoryInfo>( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GenericListView destructor
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Resources_GenericListView::~ADN_Resources_GenericListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GenericListView::ConnectItem
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Resources_GenericListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    CategoryInfo* pInfos = static_cast< CategoryInfo* >( pCurData_ );

    vItemConnectors_[ADN_Resources_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::ePackageNbr    ]->Connect( &pInfos->rNbrInPackage_ , bConnect );
    vItemConnectors_[ADN_Resources_GUI::ePackageWeight ]->Connect( &pInfos->rPackageWeight_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::ePackageVolume ]->Connect( &pInfos->rPackageVolume_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eGenEMAT6Code]->Connect( &pInfos->strCodeEMAT6_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eGenEMAT8Code]->Connect( &pInfos->strCodeEMAT8_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eGenLFRILCode]->Connect( &pInfos->strCodeLFRIL_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eGenNNOCode]->Connect( &pInfos->strCodeNNO_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eGenNature]->Connect( &pInfos->ptrResourceNature_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eGenLogisticSupplyClass]->Connect( &pInfos->ptrLogisticSupplyClass_, bConnect );
    vItemConnectors_[ADN_Resources_GUI::eGenNetworkUsable]->Connect( &pInfos->bNetworkUsable_, bConnect );

    if( ADN_Resources_Data::IsMineOrExplosive( nType_ ) )
    {
        ADN_Resources_Data::AmmoCategoryInfo* pAmmoInfos = static_cast< ADN_Resources_Data::AmmoCategoryInfo* >( pCurData_ );

        vItemConnectors_[ADN_Resources_GUI::eGenType]->Connect( &pAmmoInfos->nType_, bConnect );

        vItemConnectors_[ADN_Resources_GUI::eGenAttritions]->Connect( &pAmmoInfos->attritions_, bConnect );
        vItemConnectors_[ADN_Resources_GUI::eGenUrbanAttritions]->Connect( &pAmmoInfos->modifUrbanBlocks_, bConnect );
        vItemConnectors_[ADN_Resources_GUI::eGenArmor]->Connect( &ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Armors_Data >( eArmors ).GetArmorsInfos(), bConnect );
        vItemConnectors_[ADN_Resources_GUI::eGenMaterial]->Connect( &pAmmoInfos->modifUrbanBlocks_, bConnect );
        vItemConnectors_[ADN_Resources_GUI::eGenAttritionGraph]->Connect( &pAmmoInfos->attritions_, bConnect );

        if( bConnect )
            ADN_Workspace::GetWorkspace().GetResources().GetGui().InitializeSimulationCombos();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GenericListView::OnContextMenu
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Resources_GenericListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ResourceInfos& dotation = ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( nType_ );
    ADN_Resources_Wizard wizard( dotation, ADN_Workspace::GetWorkspace().GetResources().GetData().GetResources(), ENT_Tr::ConvertFromDotationCategory( nType_, ENT_Tr::eToTr ).c_str(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        CategoryInfo* pCastData = static_cast< CategoryInfo* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eActiveProtections ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().GetActiveProtectionsThatUse( *pCastData ), eActiveProtections );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eFires ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetFires().GetData().GetFireThatUse( *pCastData ), eFires );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eBreakdowns ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().GetBreakdownsThatUse( *pCastData ), eBreakdowns );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eObjects ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectsThatUse( *pCastData ), eObjects );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ), eEquipments );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eResourceNetworks ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksThatUse( *pCastData ), eResourceNetworks );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GenericListView::GetToolTipFor
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
std::string ADN_Resources_GenericListView::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    CategoryInfo* pCastData = static_cast< CategoryInfo* >( pData );
    assert( pCastData != 0 );

    std::string result;
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eActiveProtections ).c_str(), ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().GetActiveProtectionsThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eFires ).c_str(), ADN_Workspace::GetWorkspace().GetFires().GetData().GetFireThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eBreakdowns ).c_str(), ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().GetBreakdownsThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eObjects ).c_str(), ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectsThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(), ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eResourceNetworks ).c_str(), ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksThatUse( *pCastData ), result );

    if( result.empty() )
        result = tools::translate( "ADN_Resources_GenericListView", "<b>Unused</b>" ).toStdString();
    return result;
}
