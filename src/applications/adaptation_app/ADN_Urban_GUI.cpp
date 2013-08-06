// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Urban_GUI.h"
#include "moc_ADN_Urban_GUI.cpp"
#include "ADN_ListView_UrbanAccommodation_Type.h"
#include "ADN_ListView_UrbanInfrastructure_Type.h"
#include "ADN_ListView_UrbanMaterial_Type.h"
#include "ADN_Urban_AttritionTable.h"
#include "ADN_GuiBuilder.h"
#include "ADN_GroupBox.h"
#include "ADN_Tr.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Resources_Data.h"
#include "ADN_ListView_Templates.h"
#include "ADN_Template_Usages.h"
#include "ADN_ColorNameSelector.h"
#include "ADN_ListView_RoofShapes.h"

//-----------------------------------------------------------------------------
// Name: ADN_Urban_GUI constructor
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
ADN_Urban_GUI::ADN_Urban_GUI( ADN_Urban_Data& data )
    : ADN_GUI_ABC( eUrban )
    , data_          ( data )
    , pListMaterial_ ( 0 )
    , pListRoofShape_( 0 )
    , pListAccommodation_( 0 )
    , pListInfrastructure_( 0 )
    , pAttritionTable_( 0 )
    , pListTemplate_( 0 )
    , pUsages_( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_GUI destructor
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
ADN_Urban_GUI::~ADN_Urban_GUI()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_GUI::Build
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
void ADN_Urban_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vMaterialInfosConnectors( eNbrUrbanMaterialGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    T_ConnectorVector vFacadeInfosConnectors( eNbrUrbanGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    T_ConnectorVector vRoofShapeInfosConnectors( eNbrUrbanGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    T_ConnectorVector vAccommodationInfosConnectors( eNbrUrbanAccommodationGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    T_ConnectorVector vInfrastructureInfosConnectors( eNbrUrbanInfrastructureGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    T_ConnectorVector vUsageInfosConnectors( eNbrUrbanUsageGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Materials
    Q3GroupBox* pGroupMaterials = new Q3VGroupBox( tr( "Materials" ) );
    {
        builder.PushSubName( "materials" );

        // materials list
        pListMaterial_ = builder.AddWidget< ADN_ListView_UrbanMaterial_Type >( "list", pGroupMaterials );
        pListMaterial_->setFixedHeight( 180 );
        static_cast< ADN_Connector_Vector_ABC* >( &pListMaterial_->GetConnector() )->Connect( &data_.GetMaterialsInfos() );

        // material info
        QWidget* pHolder = builder.AddFieldHolder( pGroupMaterials );
        pHolder->layout()->setMargin( 0 );
        ADN_EditLine_ABC* nameField = builder.AddLocalizedField( pHolder, "name", tr( "Name" ), vMaterialInfosConnectors[ eUrbanMaterialName ] );
        nameField->ConnectWithRefValidity( data_.GetMaterialsInfos() );

        pAttritionTable_ = new ADN_Urban_AttritionTable( builder.GetChildName( "attrition-table" ), vMaterialInfosConnectors[ eUrbanMaterialAttrition ], pGroupMaterials );
        pAttritionTable_->setFixedHeight( 180 );

        builder.PopSubName();
    }

    // RoofShapes
    Q3GroupBox* pGroupRoofShapes = new Q3VGroupBox( tr( "RoofShapes" ) );
    {
        builder.PushSubName( "roof-shapes" );

        // roofshapes list
        pListRoofShape_ = builder.AddWidget< ADN_ListView_RoofShapes >( "list", pGroupRoofShapes );
        static_cast< ADN_Connector_Vector_ABC* >( &pListRoofShape_->GetConnector() )->Connect( &data_.GetRoofShapesInfos() );

        // Roofshape info
        QWidget* pHolder = builder.AddFieldHolder( pGroupRoofShapes );
        pHolder->layout()->setMargin( 0 );
        ADN_EditLine_ABC* nameField = builder.AddLocalizedField( pHolder, "name", tr( "Name" ), vRoofShapeInfosConnectors[ eUrbanName ] );
        nameField->ConnectWithRefValidity( data_.GetRoofShapesInfos() );

        builder.PopSubName();
    }

    // Accommodations
    Q3GroupBox* pGroupAccommodations = new Q3VGroupBox( tr( "Activities" ) );
    {
        builder.PushSubName( "activities" );

        QWidget* pHolder = builder.AddFieldHolder( pGroupAccommodations );

        builder.AddField< ADN_EditLine_Double >( pHolder, "default-nominal-capacity", tr( "Default nominal capacity" ), data_.defaultNominalCapacity_, tr( "persons/m2" ), eGreaterZero );
        builder.AddField< ADN_EditLine_Double >( pHolder, "default-maximal-capacity", tr( "Default maximal capacity" ), data_.defaultMaxCapacity_, tr( "persons/m2" ), eGreaterZero );

        // accommodations list
        pListAccommodation_= builder.AddWidget< ADN_ListView_UrbanAccommodation_Type >( "list", pGroupAccommodations );
        pListAccommodation_->setFixedHeight( 180 );
        static_cast< ADN_Connector_Vector_ABC* >( &pListAccommodation_->GetConnector() )->Connect( &data_.GetAccommodationsInfos() );

        // accommodation
        Q3GroupBox* pGroupAccommodation = new Q3HGroupBox( tr( "Activity" ), pGroupAccommodations);
        pGroupAccommodation->setInsideMargin( 10 );
        pGroupAccommodation->setInsideSpacing( 10 );
        pHolder = builder.AddFieldHolder( pGroupAccommodation );

        ADN_EditLine_ABC* nameField = builder.AddLocalizedField( pHolder, "name", tr( "Name" ), vAccommodationInfosConnectors[ eUrbanAccommodationName ] );
        nameField->ConnectWithRefValidity( data_.GetAccommodationsInfos() );
        builder.AddField< ADN_EditLine_Double >( pHolder, "nominal-capacity", tr( "Nominal capacity" ), vAccommodationInfosConnectors[ eUrbanAccommodationNominalCapacity ], tr( "persons/m2" ), eGreaterZero );
        builder.AddField< ADN_EditLine_Double >( pHolder, "maximal-capacity", tr( "Maximal capacity" ), vAccommodationInfosConnectors[ eUrbanAccommodationMaxCapacity ], tr( "persons/m2" ), eGreaterZero );

        builder.PopSubName();
    }

    // Infrastructures
    Q3GroupBox* pGroupInfrastructures = new Q3VGroupBox( tr( "Infrastructures" ) );
    {
        builder.PushSubName( "infrastructures" );

        // infrastructures list
        pListInfrastructure_= builder.AddWidget< ADN_ListView_UrbanInfrastructure_Type >( "list", pGroupInfrastructures );
        static_cast< ADN_Connector_Vector_ABC* >( &pListInfrastructure_->GetConnector() )->Connect( &data_.GetInfrastructuresInfos() );

        // infrastructures
        Q3GroupBox* pGroupInfrastructure = new Q3HGroupBox( tr( "Infrastructure" ),pGroupInfrastructures);
        pGroupInfrastructure->setInsideMargin( 10 );
        pGroupInfrastructure->setInsideSpacing( 10 );
        QWidget* pHolder = builder.AddFieldHolder( pGroupInfrastructure );

        ADN_EditLine_ABC* nameField = builder.AddLocalizedField( pHolder, "name", tr( "Name" ),  vInfrastructureInfosConnectors[ eUrbanInfrastructureName ] );
        nameField->ConnectWithRefValidity( data_.GetInfrastructuresInfos() );
        builder.AddField< ADN_ComboBox_Vector >( pHolder, "symbol", tr( "Symbol" ), vInfrastructureInfosConnectors[ eUrbanInfrastructureSymbol ] );
        builder.AddField< ADN_CheckBox >( pHolder, "medical", tr( "Medical" ), vInfrastructureInfosConnectors[ eMedicalCapacityPresent ] );

        builder.PopSubName();
    }

    // Templates
    Q3GroupBox* pTemplates = new Q3VGroupBox( tr( "Templates" ) );
    {
        builder.PushSubName( "templates" );

        pListTemplate_ = builder.AddWidget< ADN_ListView_Templates >( "list", pTemplates );
        pListTemplate_->setFixedHeight( 180 );
        static_cast< ADN_Connector_Vector_ABC* >( &pListTemplate_->GetConnector() )->Connect( &data_.GetTemplatesInfos() );
        Q3GroupBox* pGroupTemplate = new Q3VGroupBox( tr( "Template" ), pTemplates );
        QWidget* pHolder = builder.AddFieldHolder( pGroupTemplate );
        ADN_EditLine_ABC* nameField = builder.AddLocalizedField( pHolder, "name", tr( "Name" ), vUsageInfosConnectors[ eUrbanUsageName ] );
        nameField->ConnectWithRefValidity( data_.GetTemplatesInfos() );

        Q3HBox* pInfoBox = new Q3HBox( pGroupTemplate );
        pInfoBox->setSpacing( 10 );
        builder.AddField< ADN_ColorNameSelector >( pInfoBox, "color", tr( "Color" ), vUsageInfosConnectors[ eUrbanUsageColor ] );
        ADN_EditLine_Int* alpha = builder.AddField< ADN_EditLine_Int >( pInfoBox, "alpha", tr( "Alpha" ), vUsageInfosConnectors[ eUrbanUsageAlpha ] );
        alpha->GetValidator().setRange( 0, 255 );
        Q3GroupBox* pArchitecture = new Q3VGroupBox( tr( "Architecture" ), pGroupTemplate );
        Q3HBox* pArchitectureBox = new Q3HBox( pArchitecture );
        pArchitectureBox->setSpacing( 10 );
        builder.AddField< ADN_EditLine_Int >( pArchitectureBox, "floor-number", tr( "Floor number" ), vUsageInfosConnectors[ eUrbanUsageFloorNumber ], 0, eGreaterEqualZero );
        builder.AddField< ADN_EditLine_Int >( pArchitectureBox, "parking-floors", tr( "Parking floors" ), vUsageInfosConnectors[ eUrbanUsageParkingFloors ], 0, eGreaterEqualZero );
        builder.AddField< ADN_EditLine_Double >( pArchitectureBox, "occupation", tr( "Occupation" ), vUsageInfosConnectors[ eUrbanUsageOccupation ], 0, ePercentage );
        Q3HBox* pArchitectureBox2 = new Q3HBox( pArchitecture );
        pArchitectureBox2->setSpacing( 10 );
        builder.AddField< ADN_EditLine_Int >( pArchitectureBox2, "height", tr( "Height" ), vUsageInfosConnectors[ eUrbanUsageHeight ], 0, eGreaterEqualZero );
        builder.AddField< ADN_EditLine_Int >( pArchitectureBox2, "trafficability", tr( "Trafficability" ), vUsageInfosConnectors[ eUrbanUsageTrafficability ], 0, eGreaterEqualZero );
        Q3HBox* pArchitectureBox3 = new Q3HBox( pArchitecture );
        pArchitectureBox3->setSpacing( 10 );
        builder.AddField< ADN_ComboBox_Vector >( pArchitectureBox3, "material", tr( "Material" ), vUsageInfosConnectors[ eUrbanUsageMaterial ] );
        builder.AddField< ADN_ComboBox_Vector >( pArchitectureBox3, "roof-shape", tr( "Roof shape" ), vUsageInfosConnectors[ eUrbanUsageRoofShape ] );
        pUsages_ = new ADN_Template_Usages( builder.GetChildName( "usages-table" ), vUsageInfosConnectors[ eUrbanUsageRole ], pGroupTemplate );

        builder.PopSubName();
    }

    // set auto connectors
    pListMaterial_->SetItemConnectors( vMaterialInfosConnectors );
    pListRoofShape_->SetItemConnectors( vRoofShapeInfosConnectors );
    pListAccommodation_->SetItemConnectors( vAccommodationInfosConnectors );
    pListInfrastructure_->SetItemConnectors( vInfrastructureInfosConnectors );
    pListTemplate_->SetItemConnectors( vUsageInfosConnectors );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QGridLayout* pContentLayout = new QGridLayout( pContent, 3, 2 );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pGroupMaterials, 0, 0 );
    pContentLayout->addWidget( pGroupInfrastructures, 0, 1 );
    pContentLayout->addWidget( pGroupAccommodations, 1, 0 );
    pContentLayout->addWidget( pTemplates, 1, 1, 2, 1 );
    pContentLayout->addWidget( pGroupRoofShapes, 2, 0 );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent );
}