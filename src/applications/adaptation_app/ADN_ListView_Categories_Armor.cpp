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

#include <Qt3Support/q3popupmenu.h>

#include "ADN_Connector_ListView.h"
#include "ADN_Categories_Data.h"
#include "ADN_Categories_GUI.h"
#include "ADN_GuiTools.h"


typedef helpers::ArmorInfos ArmorInfos;


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Armor constructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_ListView_Categories_Armor::ADN_ListView_Categories_Armor(QWidget * parent, const char * name, Qt::WFlags f)
    : ADN_ListView(parent,name,f)
{

    // add one column && disable sort
    addColumn( tr( "Armor-Plating" ) );
    setSorting( -1, true );
    setResizeMode( Q3ListView::AllColumns );

    // connector creation
    pConnector_ = new ADN_Connector_ListView<ArmorInfos>(*this);
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
    Q3PopupMenu * pMenu=new Q3PopupMenu( this );
    pMenu->insertItem( tr( "New Armor-Plating" ), 0  );
    pMenu->insertItem( tr( "Delete Armor-Plating" ), 1 );
    pMenu->setItemEnabled( 1, pCurData_ != 0 );
    int nMenu=pMenu->exec( pt );
    switch ( nMenu )
    {
        case 0:
        {
            // create new sensor & add it to list
            ArmorInfos* pNewInfo=new ArmorInfos();

            ADN_Connector_Vector_ABC* pCList = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
            pCList->AddItem( pNewInfo );

            // Put the  new item at the top of the list (to be coherent with the application)
            int pos= FindNdx( pNewInfo );
            while( pos != 0 )
            {
                static_cast<ADN_Connector_Vector_ABC*>(&GetConnector())->SwapItem( pos - 1, pos );
                --pos;
            }

            // set current item
            setCurrentItem(FindItem(pNewInfo));
            break;
        }
        case 1:
        {
            ArmorInfos* pCurArmor= ( ArmorInfos* ) pCurData_;

            static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem(pCurArmor);
            break;
        }
        default:
            break;
    }
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
