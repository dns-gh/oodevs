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
#include "ADN_ListView_Categories_LogisticSupplyClass.h"
#include "ADN_Categories_AttritionEffect_Table.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Tr.h"
#include "ADN_TimeField.h"

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
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_GUI destructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_GUI::~ADN_Categories_GUI()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_GUI::Build
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_Categories_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    ADN_EditLine_ABC* pEdit = 0;

    /////////////////
    // Armors
    Q3GroupBox* pArmorGroup = new Q3VGroupBox( tr( "Armor classes" ) );
    Q3HBox* pArmorListViewGroup = new Q3HBox( pArmorGroup );

    // Armors listview
    T_ConnectorVector vArmorInfosConnectors( eNbrArmorGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    pListArmor_ = new ADN_ListView_Categories_Armor( pArmorListViewGroup );
    pListArmor_->setObjectName( strClassName_ + "_Armors" );
    connect( pListArmor_, SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
    static_cast< ADN_Connector_Vector_ABC* >( &pListArmor_->GetConnector() )->Connect( &data_.GetArmorsInfos() );

    // Armor info
    Q3VGroupBox* pArmorInfoGroup = new Q3VGroupBox( tr( "Armor class" ), pArmorGroup );

    QWidget* pHolder = builder.AddFieldHolder( pArmorInfoGroup );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Name" ), vArmorInfosConnectors[ eArmorName ], 0, eVarName );

    pComboType_ = builder.AddEnumField< E_ProtectionType >( pHolder, tr( "Type" ), vArmorInfosConnectors[ eArmorType ], ADN_Tr::ConvertFromProtectionType );
    connect( pComboType_, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged( int ) ) );

    Q3GroupBox* pArmorNeutralizationGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Neutralization" ), pArmorInfoGroup );
    builder.AddField< ADN_TimeField >( pArmorNeutralizationGroup, tr( "Average time" ), vArmorInfosConnectors[ eNeutralizationAverage ] );
    builder.AddField< ADN_TimeField >( pArmorNeutralizationGroup, tr( "Variance" ), vArmorInfosConnectors[ eNeutralizationVariance ] );

    pArmorBreakdownGroup_ = new Q3GroupBox( 3, Qt::Horizontal, tr( "Breakdowns" ), pArmorInfoGroup );
    builder.AddField< ADN_EditLine_Double >( pArmorBreakdownGroup_, tr( "Maintenance support needed" ), vArmorInfosConnectors[ eBreakdownEVA ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Double >( pArmorBreakdownGroup_, tr( "On site fixable" ), vArmorInfosConnectors[ eBreakdownNEVA ], tr( "%" ), ePercentage );

    pAttritionEffectGroup_ = new Q3VGroupBox( tr( "Attrition effects on humans" ), pArmorGroup );
    pAttritionEffects_ = new ADN_Categories_AttritionEffect_Table( pAttritionEffectGroup_ );
    pAttritionEffects_->setObjectName( strClassName_ + "_Attrition" );
    vArmorInfosConnectors[ eAttritionEffects ] = &pAttritionEffects_->GetConnector();

    Q3VBox* pBox = new Q3VBox();
    ///////////////////
    // Sizes
    Q3GroupBox* pGroup = new Q3VGroupBox( tr( "Sizes" ), pBox );
    Q3HBox* pGroupSize = new Q3HBox( pGroup );

    // sizes list
    T_ConnectorVector vSizeInfosConnectors( eNbrSizeGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    pListSize_ = new ADN_ListView_Categories_Size( pGroupSize );
    pListSize_->setObjectName( strClassName_ + "_Sizes" );
    connect( pListSize_, SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
    static_cast< ADN_Connector_Vector_ABC* >( &pListSize_->GetConnector() )->Connect( &data_.GetSizesInfos() );

    // size
    pGroup = new Q3VGroupBox( tr( "Size" ), pGroup );
    pGroup->setInsideMargin( 20 );
    pGroup->setInsideSpacing( 20 );
    pEdit = new ADN_EditLine_String( pGroup );
    pEdit->setObjectName( strClassName_ + "_Size" );
    vSizeInfosConnectors[ eSizeName ]=&pEdit->GetConnector();

    ///////////////////
    // Dotation Natures
    Q3GroupBox* pNatureGroup = new Q3VGroupBox( tr( "Resource natures" ), pBox );
    Q3HBox*     pNatureHBox  = new Q3HBox( pNatureGroup );

    // dotation natures list
    T_ConnectorVector vDotationNatureInfosConnectors( eNbrDotationNatureGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    pListDotationNature_ = new ADN_ListView_Categories_DotationNature( pNatureHBox );
    pListDotationNature_->setObjectName( strClassName_ + "_Dotations" );
    connect( pListDotationNature_, SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
    static_cast< ADN_Connector_Vector_ABC* >( &pListDotationNature_->GetConnector() )->Connect( &data_.GetDotationNaturesInfos() );

    // size
    pNatureGroup = new Q3VGroupBox( tr( "Nature" ), pNatureGroup );
    pNatureGroup->setInsideMargin( 20 );
    pNatureGroup->setInsideSpacing( 20 );
    pEdit = new ADN_EditLine_String( pNatureGroup );
    pEdit->setObjectName( strClassName_ + "_Dotation" );
    vDotationNatureInfosConnectors[ eDotationNatureName ] = &pEdit->GetConnector();

    ///////////////////
    // Logistic resource categories
    Q3GroupBox* pLogResourceCatGroup = new Q3VGroupBox( tr( "Logistic resource categories" ), pBox );
    Q3HBox*     pLogResourceCatHBox  = new Q3HBox( pLogResourceCatGroup );

    // dotation natures list
    T_ConnectorVector vLogisticSupplyClassesConnectors( eNbrLogisticSupplyClassGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    pListLogisticSupplyClasses_ = new ADN_ListView_Categories_LogisticSupplyClass( pLogResourceCatHBox );
    pListLogisticSupplyClasses_->setObjectName( strClassName_ + "_LogisticSupplies" );
    connect( pListLogisticSupplyClasses_, SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
    static_cast< ADN_Connector_Vector_ABC* >( &pListLogisticSupplyClasses_->GetConnector() )->Connect( &data_.GetLogisticSupplyClasses() );

    // size
    pLogResourceCatGroup = new Q3VGroupBox( tr( "Logistic resource category" ), pLogResourceCatGroup );
    pLogResourceCatGroup->setInsideMargin( 20 );
    pLogResourceCatGroup->setInsideSpacing( 20 );
    pEdit = new ADN_EditLine_String( pLogResourceCatGroup );
    pEdit->setObjectName( strClassName_ + "_LogisticSupply" );
    vLogisticSupplyClassesConnectors[ eLogisticSupplyClassName ] = &pEdit->GetConnector();

    // set auto connectors
    pListArmor_->SetItemConnectors( vArmorInfosConnectors );
    pListSize_->SetItemConnectors( vSizeInfosConnectors );
    pListDotationNature_->SetItemConnectors( vDotationNatureInfosConnectors );
    pListLogisticSupplyClasses_->SetItemConnectors( vLogisticSupplyClassesConnectors );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QHBoxLayout* pContentLayout = new QHBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pArmorGroup );
    pContentLayout->addWidget( pBox );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent );
    pMainWidget_->setObjectName( strClassName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_GUI::OnTypeChanged
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
void ADN_Categories_GUI::OnTypeChanged( int index )
{
    if( pComboType_->GetEnumIndexFromGUI( index ) == eProtectionType_Human )
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
