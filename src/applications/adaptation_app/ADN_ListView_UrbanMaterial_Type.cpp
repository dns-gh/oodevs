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
#include "ADN_ListView_UrbanMaterial_Type.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Urban_Data.h"
#include "ADN_Urban_GUI.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanMaterial_Type constructor
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
ADN_ListView_UrbanMaterial_Type::ADN_ListView_UrbanMaterial_Type( QWidget* parent )
    : ADN_ListView( parent, "Material", tools::translate( "ADN_ListView_UrbanMaterial_Type", "Material" ) )
{
    pConnector_ = new ADN_Connector_ListView< ADN_Urban_Data::UrbanMaterialInfos >( *this );
    this->SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanMaterial_Type destructor
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
ADN_ListView_UrbanMaterial_Type::~ADN_ListView_UrbanMaterial_Type()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanMaterial_Type::ConnectItem
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
void ADN_ListView_UrbanMaterial_Type::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ADN_Urban_Data::UrbanMaterialInfos* pInfos = ( ADN_Urban_Data::UrbanMaterialInfos* )pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Urban_GUI::eNbrUrbanMaterialGuiElements );

    vItemConnectors_[ADN_Urban_GUI::eUrbanMaterialName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Urban_GUI::eUrbanMaterialAttrition]->Connect( &pInfos->vAttritionInfos_, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanMaterial_Type::OnContextMenu
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
void  ADN_ListView_UrbanMaterial_Type::OnContextMenu( const QPoint& pt)
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ADN_Urban_Data::UrbanMaterialInfos > wizard( tools::translate( "ADN_ListView_UrbanMaterial_Type", "Materials" ), ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanMaterial_Type::GetToolTipFor
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
std::string ADN_ListView_UrbanMaterial_Type::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    ADN_Urban_Data::UrbanMaterialInfos* pCastData = static_cast< ADN_Urban_Data::UrbanMaterialInfos* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( tools::translate( "ADN_ListView_RoofShapes", "Templates" ),
                            ADN_Workspace::GetWorkspace().GetUrban().GetData().GetUrbanTemplateThatUse( *pCastData ) );
}
