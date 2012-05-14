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
#include "ADN_ArmorCategory_Wizard.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Categories_Data.h"
#include "ADN_Categories_GUI.h"
#include "ADN_GuiTools.h"
#include "ADN_Composantes_Data.h"
#include "ADN_Tr.h"

typedef helpers::ArmorInfos ArmorInfos;

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Armor constructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_ListView_Categories_Armor::ADN_ListView_Categories_Armor(QWidget * parent, const char * name, Qt::WFlags f)
    : ADN_ListView(parent,name,f)
{

    // add one column && disable sort
    addColumn( tools::translate( "ADN_ListView_Categories_Armor", "Armor-Plating" ) );
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
    Q3PopupMenu popupMenu( this );
    ADN_ArmorCategory_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
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
// Name: ADN_ListView_Categories_Armor::ContextMenuDelete
// Created: LGY 2012-05-14
// -----------------------------------------------------------------------------
void ADN_ListView_Categories_Armor::ContextMenuDelete()
{
    if( pCurData_ == 0 || !bDeletionEnabled_ )
        return;

    ArmorInfos* pInfos = (ArmorInfos*)pCurData_;
    if( pInfos == 0 )
        return;

    if( IsReferenced( pInfos->strName_.GetData() ) )
    {
        if( !ADN_GuiTools::MultiRefWarning() )
            return;
    }
    else if( !ADN_GuiTools::DeletionWarning() )
        return;
    static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pInfos );
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Armor::IsReferenced
// Created: LGY 2012-05-14
// -----------------------------------------------------------------------------
bool ADN_ListView_Categories_Armor::IsReferenced( const std::string& name ) const
{
    ADN_Composantes_Data::T_ComposanteInfos_Vector& vAllComposantes = ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetComposantes();
    for( ADN_Composantes_Data::T_ComposanteInfos_Vector::const_iterator it = vAllComposantes.begin(); it != vAllComposantes.end(); ++it )
        if( (*it)->ptrArmor_.GetData()->strName_.GetData() == name )
            return true;

    return false;
}
