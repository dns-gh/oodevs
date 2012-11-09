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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_GenericListView.cpp $
// $Author: Nld $
// $Modtime: 20/07/05 14:20 $
// $Revision: 11 $
// $Workfile: ADN_Equipement_GenericListView.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipement_GenericListView.h"

#include "ADN_ActiveProtections_Data.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Composantes_Data.h"
#include "ADN_FireClass_Data.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Equipement_GUI.h"
#include "ADN_Enums.h"
#include "ADN_Equipement_Wizard.h"
#include "ADN_Objects_Data.h"
#include "ADN_ResourceNetworks_Data.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Equipement_Data::ResourceInfos ResourceInfos;
typedef ADN_Equipement_Data::CategoryInfo CategoryInfo;

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GenericListView constructor
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Equipement_GenericListView::ADN_Equipement_GenericListView( E_DotationFamily nType, QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Equipement_GenericListView", ENT_Tr::ConvertFromDotationFamily( nType, ENT_Tr::eToTr ).c_str() )
    , nType_      ( nType )
{
    // Connector creation
    pConnector_ = new ADN_Connector_ListView<CategoryInfo>( *this );
    this->SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GenericListView destructor
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
ADN_Equipement_GenericListView::~ADN_Equipement_GenericListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GenericListView::ConnectItem
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Equipement_GenericListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    CategoryInfo* pInfos = (CategoryInfo*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Equipement_GUI::eNbrGenericGuiElements );

    vItemConnectors_[ADN_Equipement_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::ePackageNbr    ]->Connect( &pInfos->rNbrInPackage_ , bConnect );
    vItemConnectors_[ADN_Equipement_GUI::ePackageWeight ]->Connect( &pInfos->rPackageWeight_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::ePackageVolume ]->Connect( &pInfos->rPackageVolume_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eGenEMAT6Code]->Connect( &pInfos->strCodeEMAT6_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eGenEMAT8Code]->Connect( &pInfos->strCodeEMAT8_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eGenLFRILCode]->Connect( &pInfos->strCodeLFRIL_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eGenNNOCode]->Connect( &pInfos->strCodeNNO_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eGenNature]->Connect( &pInfos->ptrResourceNature_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eGenLogisticSupplyClass]->Connect( &pInfos->ptrLogisticSupplyClass_, bConnect );
    vItemConnectors_[ADN_Equipement_GUI::eGenNetworkUsable]->Connect( &pInfos->bNetworkUsable_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GenericListView::OnContextMenu
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Equipement_GenericListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ResourceInfos& dotation = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( nType_ );
    ADN_Equipement_Wizard wizard( dotation, ENT_Tr::ConvertFromDotationFamily( nType_, ENT_Tr::eToTr ).c_str(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        CategoryInfo* pCastData = static_cast< CategoryInfo* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eActiveProtections ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().GetActiveProtectionsThatUse( *pCastData ), eActiveProtections );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eFireClasses ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetFireClasses().GetData().GetFireThatUse( *pCastData ), eFireClasses );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eBreakdowns ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().GetBreakdownsThatUse( *pCastData ), eBreakdowns );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eObjects ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectsThatUse( *pCastData ), eObjects );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eComposantes ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetComposantesThatUse( *pCastData ), eComposantes );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eResourceNetworks ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksThatUse( *pCastData ), eResourceNetworks );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GenericListView::GetToolTipFor
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
std::string ADN_Equipement_GenericListView::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    CategoryInfo* pCastData = static_cast< CategoryInfo* >( pData );
    assert( pCastData != 0 );

    std::string result;
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eActiveProtections ).c_str(), ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().GetActiveProtectionsThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eFireClasses ).c_str(), ADN_Workspace::GetWorkspace().GetFireClasses().GetData().GetFireThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eBreakdowns ).c_str(), ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().GetBreakdownsThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eObjects ).c_str(), ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectsThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eComposantes ).c_str(), ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetComposantesThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eResourceNetworks ).c_str(), ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksThatUse( *pCastData ), result );

    if( result.empty() )
        result = tr( "<b>Unused</b>" ).toStdString();
    return result;
}
