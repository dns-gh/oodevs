//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Categories_Size.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:45 $
// $Revision: 6 $
// $Workfile: ADN_ListView_Categories_Size.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListView_Categories_Size.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Volumes_Data.h"
#include "ADN_Volumes_GUI.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"

typedef ADN_Volumes_Data::VolumeInfos VolumeInfos;

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Size constructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_ListView_Categories_Size::ADN_ListView_Categories_Size( QWidget* parent )
    : ADN_ListView( parent, "ADN_ListView_Categories_Size", tools::translate( "ADN_ListView_Categories_Size", "Sizes" ) )
{
    pConnector_ = new ADN_Connector_ListView< VolumeInfos >( *this );
    this->SetDeletionEnabled( true, false );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Size destructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_ListView_Categories_Size::~ADN_ListView_Categories_Size()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Size::ConnectItem
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_ListView_Categories_Size::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    VolumeInfos* pInfos = (VolumeInfos*) pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Volumes_GUI::eNbrSizeGuiElements );

    vItemConnectors_[ADN_Volumes_GUI::eSizeName]->Connect( &pInfos->strName_, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Size::OnContextMenu
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void  ADN_ListView_Categories_Size::OnContextMenu( const QPoint& pt)
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< VolumeInfos > wizard( tools::translate( "ADN_ListView_Categories_Size", "Sizes" ), ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Volumes_Data >( eVolumes ).GetVolumesInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        VolumeInfos* pCastData = static_cast< VolumeInfos* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ), eEquipments );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Size::GetToolTipFor
// Created: ABR 2012-07-25
// -----------------------------------------------------------------------------
std::string ADN_ListView_Categories_Size::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    VolumeInfos* pCastData = static_cast< VolumeInfos* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                            ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ) );
}