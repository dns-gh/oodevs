//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_ListView_Categories_Armor.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 10:37 $
// $Revision: 12 $
// $Workfile: ADN_ListView_Categories_Armor.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Categories_Armor.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Categories_Data.h"
#include "ADN_Categories_GUI.h"
#include "ADN_MainWindow.h"
#include "ADN_GuiTools.h"
#include "ADN_Composantes_Data.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"

typedef helpers::ArmorInfos ArmorInfos;

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Armor constructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_ListView_Categories_Armor::ADN_ListView_Categories_Armor( QWidget* parent )
    : ADN_ListView( parent, "ADN_ListView_Categories_Armor", tools::translate( "ADN_ListView_Categories_Armor", "Armor-Plating" ) )
{
    pConnector_ = new ADN_Connector_ListView< ArmorInfos >( *this );
    this->SetDeletionEnabled( true, false );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Armor destructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_ListView_Categories_Armor::~ADN_ListView_Categories_Armor()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Armor::ConnectItem
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_ListView_Categories_Armor::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ArmorInfos* pInfos = (ArmorInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Categories_GUI::eNbrArmorGuiElements );

    vItemConnectors_[ADN_Categories_GUI::eArmorName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Categories_GUI::eArmorType]->Connect( &pInfos->nType_, bConnect );
    vItemConnectors_[ADN_Categories_GUI::eNeutralizationAverage]->Connect( &pInfos->neutralizationAverageTime_, bConnect );
    vItemConnectors_[ADN_Categories_GUI::eNeutralizationVariance]->Connect( &pInfos->neutralizationVariance_, bConnect );
    vItemConnectors_[ADN_Categories_GUI::eBreakdownEVA]->Connect( &pInfos->rBreakdownEVA_, bConnect );
    vItemConnectors_[ADN_Categories_GUI::eBreakdownNEVA]->Connect( &pInfos->rBreakdownNEVA_, bConnect );
    vItemConnectors_[ADN_Categories_GUI::eAttritionEffects]->Connect( &pInfos->vAttritionEffects_, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Armor::OnContextMenu
// Created: JDY 03-07-28
//-----------------------------------------------------------------------------
void ADN_ListView_Categories_Armor::OnContextMenu( const QPoint& pt)
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ArmorInfos > wizard( tools::translate( "ADN_ListView_Categories_Armor", "Armor-Plating" ), ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        ArmorInfos* pCastData = static_cast< ArmorInfos* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eComposantes ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetComposantesThatUse( *pCastData ), eComposantes );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Armor::CreateDefaultAttritionHumanEffect
// Created: HBD 2010-05-06
// -----------------------------------------------------------------------------
void ADN_ListView_Categories_Armor::CreateDefaultAttritionHumanEffect()
{
    if( pCurData_ )
    {
        ArmorInfos* pCurArmor = ( ArmorInfos* ) pCurData_;
        if( !pCurArmor->vAttritionEffects_.size() )
        {
            pCurArmor->CreateDefaultAttrition();
            void* pData = pCurData_;
            SetCurrentItem( ( void* ) 0 );
            SetCurrentItem( pData );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Armor::GetToolTipFor
// Created: ABR 2012-07-25
// -----------------------------------------------------------------------------
std::string ADN_ListView_Categories_Armor::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    ArmorInfos* pCastData = static_cast< ArmorInfos* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eComposantes ).c_str(),
                            ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetComposantesThatUse( *pCastData ) );
}
