//*****************************************************************************
//
// $Created: JDY 03-07-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Launchers.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:04 $
// $Revision: 7 $
// $Workfile: ADN_ListView_Launchers.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListView_Launchers.h"
#include "moc_ADN_ListView_Launchers.cpp"
#include "ADN_Weapons_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tools.h"
#include "ADN_Launchers_GUI.h"
#include "ADN_Tr.h"
#include "ADN_WeaponFilter.h"
#include "ADN_Wizard.h"
#include <boost/bind.hpp>

typedef ADN_Launchers_Data::LauncherInfos LauncherInfos;

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Launchers constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Launchers::ADN_ListView_Launchers( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_ListView_Launchers", ADN_Tr::ConvertFromWorkspaceElement( eLaunchers ).c_str() )
{
    // Connector creation.
    pConnector_ = new ADN_Connector_ListView<LauncherInfos>( *this );

    this->SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Launchers destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Launchers::~ADN_ListView_Launchers()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Launchers::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListView_Launchers::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    LauncherInfos* pInfos = static_cast< LauncherInfos* >( pCurData_ );
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Launchers_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_Launchers_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Launchers_GUI::ePhModifiers]->Connect( &pInfos->tabModifs_, bConnect );

    // Connect those at the end so that the items in the associated group boxes
    // are correctly enabled / disabled according to those values.
    static ADN_WeaponFilter direct;
    direct.Initialize( *pInfos, true );
    vItemConnectors_[ADN_Launchers_GUI::eDirect]->Connect( &direct, bConnect );
    static ADN_WeaponFilter indirect;
    indirect.Initialize( *pInfos, false );
    vItemConnectors_[ADN_Launchers_GUI::eIndirect]->Connect( &indirect, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Launchers::OnContextMenu
// Created: APE 2005-01-28
// -----------------------------------------------------------------------------
void ADN_ListView_Launchers::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< LauncherInfos > wizard( ADN_Tr::ConvertFromWorkspaceElement( eLaunchers ).c_str(), ADN_Workspace::GetWorkspace().GetLaunchers().GetData().GetLaunchersInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        LauncherInfos* pCastData = static_cast< LauncherInfos* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                      ADN_Tr::ConvertFromWorkspaceElement( eWeapons ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponThatUse( *pCastData ), eWeapons );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Launchers::GetToolTipFor
// Created: SBO 2005-09-06
// -----------------------------------------------------------------------------
std::string ADN_ListView_Launchers::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    LauncherInfos* pCastData = static_cast< LauncherInfos* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eWeapons ).c_str(),
                            ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponThatUse( *pCastData ) );
}
