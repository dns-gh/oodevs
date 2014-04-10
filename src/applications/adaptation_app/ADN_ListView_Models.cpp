//*****************************************************************************
//
// $Created: JDY 03-07-25 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Models.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:04 $
// $Revision: 7 $
// $Workfile: ADN_ListView_Models.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListView_Models.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Workspace.h"
#include "ADN_Models_GUI.h"
#include "ADN_Models_Wizard.h"
#include "ADN_Units_Data.h"
#include "ADN_Automata_Data.h"
#include "ADN_Crowds_Data.h"
#include "ADN_enums.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_Models_ModelInfos ModelInfos;

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Models constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Models::ADN_ListView_Models( QWidget* pParent, E_EntityType eEntityType )
    : ADN_ListView( pParent, "ADN_ListView_Models", ADN_Tr::ConvertFromWorkspaceElement( eModels ).c_str() )
    , eEntityType_( eEntityType )
{
    pConnector_.reset( new ADN_Connector_ListView<ModelInfos>( *this ) );
    SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Models destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Models::~ADN_ListView_Models()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Models::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListView_Models::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ModelInfos* pInfos = (ModelInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Models_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_Models_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Models_GUI::eDiaType]->Connect( &pInfos->strDiaType_, bConnect );
    vItemConnectors_[ADN_Models_GUI::eFile]->Connect( &pInfos->strFile_, bConnect );
    vItemConnectors_[ADN_Models_GUI::eMissions]->Connect( &pInfos->vMissions_, bConnect );
    vItemConnectors_[ADN_Models_GUI::eFragOrders]->Connect( &pInfos->vFragOrders_, bConnect );
    vItemConnectors_[ADN_Models_GUI::eMasalife]->Connect( &pInfos->isMasalife_, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Models::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_ListView_Models::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Models_Wizard wizard( eEntityType_, ADN_Tr::ConvertFromWorkspaceElement( eModels ).c_str(),
                              ADN_Workspace::GetWorkspace().GetModels().GetData().GetModels( eEntityType_ ), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        ModelInfos* pCastData = static_cast< ModelInfos* >( pCurData_ );
        assert( pCastData != 0 );
        if( eEntityType_ == eEntityType_Pawn )
            FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                          ADN_Tr::ConvertFromWorkspaceElement( eUnits ).c_str(),
                                          ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsThatUse( *pCastData ), eUnits );
        else if( eEntityType_ == eEntityType_Automat )
            FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                          ADN_Tr::ConvertFromWorkspaceElement( eAutomata ).c_str(),
                                          ADN_Workspace::GetWorkspace().GetAutomata().GetData().GetAutomataThatUse( *pCastData ), eAutomata );
        else if( eEntityType_ == eEntityType_Population )
            FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                          ADN_Tr::ConvertFromWorkspaceElement( eCrowds ).c_str(),
                                          ADN_Workspace::GetWorkspace().GetCrowds().GetData().GetCrowdsThatUse( *pCastData ), eCrowds );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Models::GetToolTipFor
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
std::string ADN_ListView_Models::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    ModelInfos* pCastData = static_cast< ModelInfos* >( pData );
    assert( pCastData != 0 );
    if( eEntityType_ == eEntityType_Pawn )
        return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eUnits ).c_str(),
                                ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsThatUse( *pCastData ) );
    else if( eEntityType_ == eEntityType_Automat )
        return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eAutomata ).c_str(),
                                ADN_Workspace::GetWorkspace().GetAutomata().GetData().GetAutomataThatUse( *pCastData ) );
    else if( eEntityType_ == eEntityType_Population )
        return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eCrowds ).c_str(),
                                ADN_Workspace::GetWorkspace().GetCrowds().GetData().GetCrowdsThatUse( *pCastData ) );
    return "";
}
