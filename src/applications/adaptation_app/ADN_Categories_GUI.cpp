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
#include "ADN_TimeField.h"

//-----------------------------------------------------------------------------
// Name: ADN_Categories_GUI constructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_GUI::ADN_Categories_GUI( ADN_Categories_Data& data )
    : ADN_GUI_ABC( eCategories )
    , data_( data )
    , pListArmor_( 0 )
    , pListSize_( 0 )
    , pListDotationNature_( 0 )
    , pListLogisticSupplyClasses_( 0 )
    , pAttritionEffects_( 0 )
    , pComboType_( 0 )
    , pArmorBreakdownGroup_( 0 )
    , pAttritionEffectGroup_( 0 )
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

    T_ConnectorVector vArmorInfosConnectors( eNbrArmorGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    T_ConnectorVector vSizeInfosConnectors( eNbrSizeGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    T_ConnectorVector vDotationNatureInfosConnectors( eNbrDotationNatureGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    T_ConnectorVector vLogisticSupplyClassesConnectors( eNbrLogisticSupplyClassGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    /////////////////
    // Armors
    Q3VGroupBox* pArmorGroup = new Q3VGroupBox( tr( "Armor classes" ) );
    {
        builder.PushSubName( "armors" );

        // Armors list view
        pListArmor_ = builder.AddWidget< ADN_ListView_Categories_Armor >( "list", pArmorGroup );
        connect( pListArmor_, SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
        static_cast< ADN_Connector_Vector_ABC* >( &pListArmor_->GetConnector() )->Connect( &data_.GetArmorsInfos() );

        // Armor info
        Q3VGroupBox* pArmorInfoGroup = new Q3VGroupBox( tr( "Armor class" ), pArmorGroup );

        QWidget* pHolder = builder.AddFieldHolder( pArmorInfoGroup );
        ADN_EditLine_ABC* nameField = builder.AddField< ADN_EditLine_String >( pHolder, "name", tr( "Name" ), vArmorInfosConnectors[ eArmorName ], 0, eVarName );
        nameField->ConnectWithRefValidity( data_.GetArmorsInfos() );

        pComboType_ = builder.AddEnumField( pHolder, "type", tr( "Type" ), vArmorInfosConnectors[ eArmorType ] );
        connect( pComboType_, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged( int ) ) );

        Q3GroupBox* pArmorNeutralizationGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Neutralization" ), pArmorInfoGroup );
        builder.AddField< ADN_TimeField >( pArmorNeutralizationGroup, "average-time", tr( "Average time" ), vArmorInfosConnectors[ eNeutralizationAverage ] );
        builder.AddField< ADN_TimeField >( pArmorNeutralizationGroup, "variance", tr( "Variance" ), vArmorInfosConnectors[ eNeutralizationVariance ] );

        pArmorBreakdownGroup_ = new Q3GroupBox( 3, Qt::Horizontal, tr( "Breakdowns" ), pArmorInfoGroup );
        builder.AddField< ADN_EditLine_Double >( pArmorBreakdownGroup_, "maintenance-support-needed", tr( "Maintenance support needed" ), vArmorInfosConnectors[ eBreakdownEVA ], tr( "%" ), ePercentage );
        builder.AddField< ADN_EditLine_Double >( pArmorBreakdownGroup_, "on-site-fixable", tr( "On site fixable" ), vArmorInfosConnectors[ eBreakdownNEVA ], tr( "%" ), ePercentage );

        pAttritionEffectGroup_ = new Q3VGroupBox( tr( "Attrition effects on humans" ), pArmorGroup );

        new ADN_Categories_AttritionEffect_Table( builder.GetChildName( "attrition-table" ), vArmorInfosConnectors[ eAttritionEffects ], pAttritionEffectGroup_ );

        builder.PopSubName();
    }

    ///////////////////
    // Sizes
    Q3VGroupBox* pSizeGroup = new Q3VGroupBox( tr( "Sizes" ) );
    {
        builder.PushSubName( "sizes" );

        // sizes list view
        pListSize_ = builder.AddWidget< ADN_ListView_Categories_Size >( "list", pSizeGroup );
        connect( pListSize_, SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
        static_cast< ADN_Connector_Vector_ABC* >( &pListSize_->GetConnector() )->Connect( &data_.GetSizesInfos() );

        // size infos
        QWidget* pHolder = builder.AddFieldHolder( pSizeGroup );
        pHolder->layout()->setMargin( 0 );
        ADN_EditLine_ABC* nameField = builder.AddField< ADN_EditLine_String >( pHolder, "name", tr( "Name" ), vSizeInfosConnectors[ eSizeName ], 0, eVarName );
        nameField->ConnectWithRefValidity( data_.GetSizesInfos() );

        builder.PopSubName();
    }

    ///////////////////
    // Dotation Natures
    Q3VGroupBox* pNatureGroup = new Q3VGroupBox( tr( "Resource natures" ) );
    {
        builder.PushSubName( "sizes" );

        // dotation natures list
        pListDotationNature_ = builder.AddWidget< ADN_ListView_Categories_DotationNature >( "list", pNatureGroup );
        connect( pListDotationNature_, SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
        static_cast< ADN_Connector_Vector_ABC* >( &pListDotationNature_->GetConnector() )->Connect( &data_.GetDotationNaturesInfos() );

        // dotation nature info
        QWidget* pHolder = builder.AddFieldHolder( pNatureGroup );
        pHolder->layout()->setMargin( 0 );
        ADN_EditLine_ABC* nameField = builder.AddField< ADN_EditLine_String >( pHolder, "name", tr( "Name" ), vDotationNatureInfosConnectors[ eDotationNatureName ], 0, eVarName );
        nameField->ConnectWithRefValidity( data_.GetDotationNaturesInfos() );

        builder.PopSubName();
    }

    ///////////////////
    // Logistic resource categories
    Q3GroupBox* pLogResourceCatGroup = new Q3VGroupBox( tr( "Logistic resource categories" ) );
    {
        builder.PushSubName( "logistic-categories" );

        // Logistic resource categories list
        pListLogisticSupplyClasses_ = builder.AddWidget< ADN_ListView_Categories_LogisticSupplyClass >( "list", pLogResourceCatGroup );
        connect( pListLogisticSupplyClasses_, SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
        static_cast< ADN_Connector_Vector_ABC* >( &pListLogisticSupplyClasses_->GetConnector() )->Connect( &data_.GetLogisticSupplyClasses() );

        // Logistic resource category info
        QWidget* pHolder = builder.AddFieldHolder( pLogResourceCatGroup );
        pHolder->layout()->setMargin( 0 );
        ADN_EditLine_ABC* nameField = builder.AddField< ADN_EditLine_String >( pHolder, "name", tr( "Name" ), vLogisticSupplyClassesConnectors[ eLogisticSupplyClassName ], 0, eVarName );
        nameField->ConnectWithRefValidity( data_.GetLogisticSupplyClasses() );

        builder.PopSubName();
    }

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
    QGridLayout* pContentLayout = new QGridLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pArmorGroup, 0, 0, 3, 1 );
    pContentLayout->addWidget( pSizeGroup, 0, 1 );
    pContentLayout->addWidget( pNatureGroup, 1, 1 );
    pContentLayout->addWidget( pLogResourceCatGroup, 2, 1 );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent );
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
