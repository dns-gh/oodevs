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
#include "ADN_Armors_Data.h"
#include "ADN_Armors_GUI.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_Armors_Data::ArmorInfos ArmorInfos;

class ADN_CLV_Categories_Armor : public ADN_Connector_ListView< ArmorInfos >
{
public:
    ADN_CLV_Categories_Armor( ADN_ListView_Categories_Armor& list )
        : ADN_Connector_ListView< ArmorInfos >( list )
    {}

    virtual ~ADN_CLV_Categories_Armor()
    {}

    ADN_StandardItem* CreateItem( void* obj )
    {
        ArmorInfos* pInfos = static_cast< ArmorInfos*>( obj );
        if( pInfos->GetType() == eProtectionType_Crowd )
            return 0;
        ADN_StandardItem *pItem = new ADN_StandardItem( obj );
        pItem->Connect( &pInfos->strName_ );
        return pItem;
    }

private:
    ADN_CLV_Categories_Armor& operator=( const ADN_CLV_Categories_Armor& );
};


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Armor constructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_ListView_Categories_Armor::ADN_ListView_Categories_Armor( QWidget* parent )
    : ADN_ListView( parent, "ADN_ListView_Categories_Armor", tools::translate( "ADN_ListView_Categories_Armor", "Armor-Plating" ) )
{
    pConnector_.reset( new ADN_CLV_Categories_Armor( *this ) );
    SetDeletionEnabled( true, false );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Armor destructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_ListView_Categories_Armor::~ADN_ListView_Categories_Armor()
{
    // NOTHING
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
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Armors_GUI::eNbrArmorGuiElements );

    vItemConnectors_[ADN_Armors_GUI::eArmorName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Armors_GUI::eArmorType]->Connect( &pInfos->nType_, bConnect );
    vItemConnectors_[ADN_Armors_GUI::eNeutralizationAverage]->Connect( &pInfos->neutralizationAverageTime_, bConnect );
    vItemConnectors_[ADN_Armors_GUI::eNeutralizationVariance]->Connect( &pInfos->neutralizationVariance_, bConnect );
    vItemConnectors_[ADN_Armors_GUI::eBreakdownEVA]->Connect( &pInfos->rBreakdownEVA_, bConnect );
    vItemConnectors_[ADN_Armors_GUI::eBreakdownNEVA]->Connect( &pInfos->rBreakdownNEVA_, bConnect );
    vItemConnectors_[ADN_Armors_GUI::eAttritionEffects]->Connect( &pInfos->vAttritionEffects_, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Armor::OnContextMenu
// Created: JDY 03-07-28
//-----------------------------------------------------------------------------
void ADN_ListView_Categories_Armor::OnContextMenu( const QPoint& pt)
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ArmorInfos > wizard( tools::translate( "ADN_ListView_Categories_Armor", "Armor-Plating" ), ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Armors_Data >( eArmors ).GetArmorsInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        ArmorInfos* pCastData = static_cast< ArmorInfos* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ), eEquipments );
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
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                            ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ) );
}
