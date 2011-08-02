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
#include "ADN_ListView_Urban_Type.h"
#include "ADN_ListView_UrbanAccommodation_Type.h"
#include "ADN_ListView_UrbanInfrastructure_Type.h"
#include "ADN_ListView_UrbanMaterial_Type.h"
#include "ADN_Urban_AttritionTable.h"
#include "ADN_GuiBuilder.h"
#include "ADN_GroupBox.h"
#include "ADN_Tr.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Equipement_Data.h"

#pragma warning( push, 0 )
#include <Qt3Support/q3frame.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <Qt3Support/q3vgroupbox.h>
#include <Qt3Support/q3hgroupbox.h>
#include <Qt3Support/q3hbox.h>
#include <Qt3Support/q3vbox.h>
#include <QtGui/qpushbutton.h>
#include <Qt3Support/q3whatsthis.h>
#pragma warning( pop )
//-----------------------------------------------------------------------------
// Name: ADN_Urban_GUI constructor
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
ADN_Urban_GUI::ADN_Urban_GUI( ADN_Urban_Data& data )
    : ADN_GUI_ABC( "ADN_Urban_GUI" )
    , data_          ( data )
    , pListMaterial_ ( 0 )
    , pListFacade_   ( 0 )
    , pListRoofShape_( 0 )
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
    assert( pMainWidget_ == 0 );

    // Create the main widget.
    pMainWidget_ = new QWidget( 0, "Urban main widget" );

    // horizontal layout
    Q3HBoxLayout* pLayout = new Q3HBoxLayout( pMainWidget_ );
    pLayout->setMargin( 20 );
    pLayout->setSpacing( 20 );
    pLayout->setAutoAdd( true );

    Q3GroupBox* pBox = new Q3GroupBox( 2, Qt::Horizontal, pMainWidget_ );

    ///////////////////
    // Materials
    Q3GroupBox* pGroupMaterials = new Q3VGroupBox( tr( "Materials" ), pBox );
    Q3HBox* pGroupMaterial = new Q3HBox(pGroupMaterials);

    // materials list
    T_ConnectorVector vMaterialInfosConnectors( eNbrUrbanMaterialGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    pListMaterial_= new ADN_ListView_UrbanMaterial_Type( pGroupMaterial, "Material" );
    static_cast< ADN_Connector_Vector_ABC* >( &pListMaterial_->GetConnector() )->Connect( &data_.GetMaterialsInfos() );

    // material
    pGroupMaterials = new Q3VGroupBox( tr( "Material" ),pGroupMaterials);
    pGroupMaterials->setInsideMargin( 20 );
    pGroupMaterials->setInsideSpacing( 20 );
    ADN_EditLine_ABC* pEdit = new ADN_EditLine_String( pGroupMaterials );
    vMaterialInfosConnectors[ eUrbanMaterialName ] = &pEdit->GetConnector();

    pAttritionTable_ = new ADN_Urban_AttritionTable( pGroupMaterials );
    vMaterialInfosConnectors[ eUrbanMaterialAttrition ] = &pAttritionTable_->GetConnector();

    ///////////////////
    // Facades
    Q3GroupBox* pGroupFacades = new Q3VGroupBox( tr( "Facades" ), pBox );
    Q3HBox* pGroupFacade = new Q3HBox(pGroupFacades);

    // facades list
    T_ConnectorVector vFacadeInfosConnectors( eNbrUrbanGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    pListFacade_= new ADN_ListView_Urban_Type( pGroupFacade, "Facade" );
    static_cast< ADN_Connector_Vector_ABC* >( &pListFacade_->GetConnector() )->Connect( &data_.GetFacadesInfos() );

    // facade
    pGroupFacades = new Q3VGroupBox( tr( "Facade" ),pGroupFacades);
    pGroupFacades->setInsideMargin(20);
    pGroupFacades->setInsideSpacing(20);
    pEdit = new ADN_EditLine_String(pGroupFacades);
    vFacadeInfosConnectors[ eUrbanName ] = &pEdit->GetConnector();

    ///////////////////
    // RoofShapes
    Q3GroupBox* pGroupRoofShapes = new Q3VGroupBox( tr( "RoofShapes" ), pBox );
    Q3HBox* pGroupRoofShape = new Q3HBox(pGroupRoofShapes);

    // roofshapes list
    T_ConnectorVector vRoofShapeInfosConnectors( eNbrUrbanGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    pListRoofShape_= new ADN_ListView_Urban_Type( pGroupRoofShape, "RoofShape" );
    static_cast< ADN_Connector_Vector_ABC* >( &pListRoofShape_->GetConnector() )->Connect( &data_.GetRoofShapesInfos() );

    // roofsape
    pGroupRoofShapes = new Q3VGroupBox( tr( "RoofShape" ),pGroupRoofShapes);
    pGroupRoofShapes->setInsideMargin(20);
    pGroupRoofShapes->setInsideSpacing(20);
    pEdit = new ADN_EditLine_String(pGroupRoofShapes);
    vRoofShapeInfosConnectors[ eUrbanName ] = &pEdit->GetConnector();

    ///////////////////
    // Accommodations
    Q3GroupBox* pGroupAccommodations = new Q3VGroupBox( tr( "Activities" ), pBox );
    ADN_GuiBuilder builder;
    QWidget* pHolder = builder.AddFieldHolder( pGroupAccommodations );

    builder.AddField< ADN_EditLine_Double >( pHolder, tr( "Default nominal capacity" ), data_.defaultNominalCapacity_, tr( "persons/m2" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pHolder, tr( "Default maximal capacity" ), data_.defaultMaxCapacity_, tr( "persons/m2" ), eGreaterZero );

    Q3HBox* pGroupAccommodation = new Q3HBox( pGroupAccommodations );

    // accommodations list
    T_ConnectorVector vAccommodationInfosConnectors( eNbrUrbanAccommodationGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    pListAccommodation_= new ADN_ListView_UrbanAccommodation_Type( pGroupAccommodation, "Activity" );
    static_cast< ADN_Connector_Vector_ABC* >( &pListAccommodation_->GetConnector() )->Connect( &data_.GetAccommodationsInfos() );

    // accommodation
    pGroupAccommodations = new Q3HGroupBox( tr( "Activity" ),pGroupAccommodations);
    pGroupAccommodations->setInsideMargin(20);
    pGroupAccommodations->setInsideSpacing(20);
    pHolder = builder.AddFieldHolder( pGroupAccommodations );

    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Type" ),  vAccommodationInfosConnectors[ eUrbanAccommodationName ] );
    builder.AddField< ADN_EditLine_Double >( pHolder, tr( "Nominal capacity" ), vAccommodationInfosConnectors[ eUrbanAccommodationNominalCapacity ], tr( "persons/m2" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pHolder, tr( "Maximal capacity" ), vAccommodationInfosConnectors[ eUrbanAccommodationMaxCapacity ], tr( "persons/m2" ), eGreaterZero );

    ///////////////////
    // Infrastructures
    Q3GroupBox* pGroupInfrastructures = new Q3VGroupBox( tr( "Infrastructures" ), pBox );
    Q3HBox* pGroupInfrastructure = new Q3HBox( pGroupInfrastructures );

    // infrastructures list
    T_ConnectorVector vInfrastructureInfosConnectors( eNbrUrbanInfrastructureGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    pListInfrastructure_= new ADN_ListView_UrbanInfrastructure_Type( pGroupInfrastructure, "Infrastructure" );
    static_cast< ADN_Connector_Vector_ABC* >( &pListInfrastructure_->GetConnector() )->Connect( &data_.GetInfrastructuresInfos() );

    // infrastructures
    pGroupInfrastructures = new Q3HGroupBox( tr( "Infrastructure" ),pGroupInfrastructures);
    pGroupInfrastructures->setInsideMargin(20);
    pGroupInfrastructures->setInsideSpacing(20);
    pHolder = builder.AddFieldHolder( pGroupInfrastructures );

    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Type" ),  vInfrastructureInfosConnectors[ eUrbanInfrastructureName ] );
    builder.AddField< ADN_ComboBox_Vector< ADN_Symbols_Data::SymbolsInfra > >( pHolder, tr( "Symbol" ), vInfrastructureInfosConnectors[ eUrbanInfrastructureSymbol ] );

    ADN_GroupBox* medical = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Medical" ), pGroupInfrastructures );
    {
        vInfrastructureInfosConnectors[ eMedicalCapacityPresent ] = &medical->GetConnector();
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Doctor night rate" ), vInfrastructureInfosConnectors[ eMedicalCapacity_NightRate ], 0, eGreaterEqualZero );
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Emergency doctor rate" ), vInfrastructureInfosConnectors[ eMedicalCapacity_EmergencyDoctorRate ], 0, eGreaterEqualZero );
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Emergency bed rate" ), vInfrastructureInfosConnectors[ eMedicalCapacity_EmergencyBedRate ], 0, eGreaterEqualZero );
    }

    // set auto connectors
    pListMaterial_->SetItemConnectors( vMaterialInfosConnectors );
    pListFacade_->SetItemConnectors( vFacadeInfosConnectors );
    pListRoofShape_->SetItemConnectors( vRoofShapeInfosConnectors );
    pListAccommodation_->SetItemConnectors( vAccommodationInfosConnectors );
    pListInfrastructure_->SetItemConnectors( vInfrastructureInfosConnectors );
}
