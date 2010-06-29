//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Categories_GUI.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:56 $
// $Revision: 14 $
// $Workfile: ADN_Categories_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Categories_GUI.h"

#include "moc_ADN_Categories_GUI.cpp"

#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_Categories_Data.h"
#include "ADN_ListView_Categories_Armor.h"
#include "ADN_ListView_Categories_Size.h"
#include "ADN_ListView_Categories_DotationNature.h"
#include "ADN_Categories_AttritionEffect_Table.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Tr.h"
#include "ADN_TimeField.h"

#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qpushbutton.h>
#include <qwhatsthis.h>


//-----------------------------------------------------------------------------
// Name: ADN_Categories_GUI constructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_GUI::ADN_Categories_GUI( ADN_Categories_Data& data )
: ADN_GUI_ABC( "ADN_Categories_GUI" )
, data_      ( data )
, pListArmor_( 0 )
, pListSize_ ( 0 )
, pListDotationNature_( 0 )
, pAttritionEffects_( 0 )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Categories_GUI destructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_GUI::~ADN_Categories_GUI()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Categories_GUI::Build
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_Categories_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;
    ADN_EditLine_ABC* pEdit = 0;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0, "categories main widget" );

    // horizontal layout
    QHBoxLayout* pLayout = new QHBoxLayout( pMainWidget_ );
    pLayout->setMargin( 20 );
    pLayout->setSpacing( 20 );
    pLayout->setAutoAdd( true );

    /////////////////
    // Armors
    QGroupBox* pArmorGroup = new QVGroupBox( tr( "Armor classes" ), pMainWidget_ );

    QHBox* pArmorListViewGroup = new QHBox( pArmorGroup );

    // Armors listview
    T_ConnectorVector vArmorInfosConnectors( eNbrArmorGuiElements, (ADN_Connector_ABC*)0 );
    pListArmor_ = new ADN_ListView_Categories_Armor( pArmorListViewGroup );
    static_cast<ADN_Connector_Vector_ABC*>( &pListArmor_->GetConnector() )->Connect( &data_.GetArmorsInfos() );

    // Armor info
    QVGroupBox* pArmorInfoGroup  = new QVGroupBox( tr( "Armor class" ), pArmorGroup );

    QWidget* pHolder = builder.AddFieldHolder( pArmorInfoGroup );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "Name" ), vArmorInfosConnectors[eArmorName], 0, eVarName );
    pComboType_ = builder.AddEnumField<E_ProtectionType>( pHolder, tr( "Type" ), vArmorInfosConnectors[eArmorType], ADN_Tr::ConvertFromProtectionType );

    connect( pComboType_, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged( int ) ) );

    QGroupBox* pArmorNeutralizationGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Neutralization" ), pArmorInfoGroup );
    builder.AddField<ADN_TimeField>( pArmorNeutralizationGroup, tr( "Average time" ), vArmorInfosConnectors[eNeutralizationAverage] );
    builder.AddField<ADN_TimeField>( pArmorNeutralizationGroup, tr( "Variance" ), vArmorInfosConnectors[eNeutralizationVariance] );

    pArmorBreakdownGroup_ = new QGroupBox( 3, Qt::Horizontal, tr( "Breakdowns" ), pArmorInfoGroup );
    builder.AddField<ADN_EditLine_Double>( pArmorBreakdownGroup_, tr( "Maintenance support needed" ), vArmorInfosConnectors[eBreakdownEVA], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pArmorBreakdownGroup_, tr( "On site fixable" ), vArmorInfosConnectors[eBreakdownNEVA], tr( "%" ), ePercentage );

    pAttritionEffectGroup_ = new QVGroupBox( tr( "Attrition effects on humans" ), pArmorGroup );
    pAttritionEffects_ = new ADN_Categories_AttritionEffect_Table( pAttritionEffectGroup_ );
    vArmorInfosConnectors[eAttritionEffects] = &pAttritionEffects_->GetConnector();

    QVBox* pBox = new QVBox( pMainWidget_ );

    ///////////////////
    // Sizes
    QGroupBox* pGroup = new QVGroupBox( tr( "Sizes" ), pBox );
    QHBox* pGroupSize = new QHBox(pGroup);

    // sizes list
    T_ConnectorVector    vSizeInfosConnectors(eNbrSizeGuiElements,(ADN_Connector_ABC*)0 );
    pListSize_=new ADN_ListView_Categories_Size(pGroupSize);
    static_cast<ADN_Connector_Vector_ABC*>( &pListSize_->GetConnector() )->Connect( &data_.GetSizesInfos() );
    //QWhatsThis::add( pListSize_, "Les diff�rentes cat�gories de volumes existants dans la simulation.\nCes cat�gories sont utilis�es pour caract�riser les composantes.\nElles influencent la perception des unit�s et les PHs des syst�mes d'armes." );

    // size
    pGroup = new QVGroupBox( tr( "Size" ),pGroup);
    pGroup->setInsideMargin(20);
    pGroup->setInsideSpacing(20);
    pEdit = new ADN_EditLine_String(pGroup);
    vSizeInfosConnectors[eSizeName]=&pEdit->GetConnector();

    ///////////////////
    // Dotation Natures
    QGroupBox* pNatureGroup = new QVGroupBox( tr( "Resource natures" ), pBox );
    QHBox*     pNatureHBox  = new QHBox( pNatureGroup );

    // dotation natures list
    T_ConnectorVector    vDotationNatureInfosConnectors( eNbrDotationNatureGuiElements, (ADN_Connector_ABC*)0 );
    pListDotationNature_ = new ADN_ListView_Categories_DotationNature( pNatureHBox );
    static_cast<ADN_Connector_Vector_ABC*>( &pListDotationNature_->GetConnector() )->Connect( &data_.GetDotationNaturesInfos() );
    //QWhatsThis::add( pListDotationNature_, "Les diff�rentes nature de dotations existantes dans la simulation." );

    // size
    pNatureGroup = new QVGroupBox( tr( "Nature" ), pNatureGroup );
    pNatureGroup->setInsideMargin( 20 );
    pNatureGroup->setInsideSpacing( 20 );
    pEdit = new ADN_EditLine_String( pNatureGroup );
    vDotationNatureInfosConnectors[ eDotationNatureName ] = &pEdit->GetConnector();

    // set auto connectors
    pListArmor_->SetItemConnectors( vArmorInfosConnectors );
    pListSize_->SetItemConnectors( vSizeInfosConnectors );
    pListDotationNature_->SetItemConnectors( vDotationNatureInfosConnectors );
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_GUI::OnTypeChanged
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
void ADN_Categories_GUI::OnTypeChanged( int index )
{
    if( index == eProtectionType_Human )
    {
        pArmorBreakdownGroup_->hide();
        pAttritionEffectGroup_->hide();
    }
    else
    {
        pListArmor_->CreateDefaultAttritionHumanEffect();
        pArmorBreakdownGroup_->show();
        pAttritionEffectGroup_->show();
    }
}
