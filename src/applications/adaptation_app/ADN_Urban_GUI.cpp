// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Urban_GUI.h"

#include "moc_ADN_Urban_GUI.cpp"

#include "ADN_App.h"
#include "ADN_AttritionInfos.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_Urban_Data.h"
#include "ADN_ListView_Urban_Type.h"
#include "ADN_ListView_UrbanAccommodation_Type.h"
#include "ADN_ListView_UrbanInfrastructure_Type.h"
#include "ADN_ListView_UrbanMaterial_Type.h"
#include "ADN_Urban_AttritionTable.h"
#include "ADN_GuiBuilder.h"
#include "ADN_GroupBox.h"
#include "ADN_Tr.h"
#include "ADN_TimeField.h"
#include "ADN_Equipement_Data.h"

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

    ADN_EditLine_ABC* pEdit = 0;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0, "Urban main widget" );

    // horizontal layout
    QHBoxLayout* pLayout = new QHBoxLayout( pMainWidget_ );
    pLayout->setMargin( 20 );
    pLayout->setSpacing( 20 );
    pLayout->setAutoAdd( true );

    QGroupBox* pBox = new QGroupBox( 2, Qt::Horizontal, pMainWidget_ );

    ///////////////////
    // Materials
    QGroupBox* pGroupMaterials = new QVGroupBox( tr( "Materials" ), pBox );
    QHBox* pGroupMaterial = new QHBox(pGroupMaterials);

    // materials list
    T_ConnectorVector    vMaterialInfosConnectors(eNbrUrbanMaterialGuiElements,( ADN_Connector_ABC* ) 0 );
    pListMaterial_= new ADN_ListView_UrbanMaterial_Type( pGroupMaterial, "Material" );
    static_cast<ADN_Connector_Vector_ABC*>( &pListMaterial_->GetConnector() )->Connect( &data_.GetMaterialsInfos() );

    // material
    pGroupMaterials = new QVGroupBox( tr( "Material" ),pGroupMaterials);
    pGroupMaterials->setInsideMargin(20);
    pGroupMaterials->setInsideSpacing(20);
    pEdit = new ADN_EditLine_String( pGroupMaterials );
    vMaterialInfosConnectors[eUrbanMaterialName] = &pEdit->GetConnector();

    pAttritionTable_ = new ADN_Urban_AttritionTable( pGroupMaterials );
    vMaterialInfosConnectors[eUrbanMaterialAttrition] = &pAttritionTable_->GetConnector();

    ///////////////////
    // Facades
    QGroupBox* pGroupFacades = new QVGroupBox( tr( "Facades" ), pBox );
    QHBox* pGroupFacade = new QHBox(pGroupFacades);

    // facades list
    T_ConnectorVector    vFacadeInfosConnectors(eNbrUrbanGuiElements,(ADN_Connector_ABC*)0 );
    pListFacade_= new ADN_ListView_Urban_Type( pGroupFacade, "Facade" );
    static_cast<ADN_Connector_Vector_ABC*>( &pListFacade_->GetConnector() )->Connect( &data_.GetFacadesInfos() );

    // facade
    pGroupFacades = new QVGroupBox( tr( "Facade" ),pGroupFacades);
    pGroupFacades->setInsideMargin(20);
    pGroupFacades->setInsideSpacing(20);
    pEdit = new ADN_EditLine_String(pGroupFacades);
    vFacadeInfosConnectors[eUrbanName]=&pEdit->GetConnector();

    ///////////////////
    // RoofShapes
    QGroupBox* pGroupRoofShapes = new QVGroupBox( tr( "RoofShapes" ), pBox );
    QHBox* pGroupRoofShape = new QHBox(pGroupRoofShapes);

    // roofshapes list
    T_ConnectorVector    vRoofShapeInfosConnectors(eNbrUrbanGuiElements,(ADN_Connector_ABC*)0 );
    pListRoofShape_= new ADN_ListView_Urban_Type( pGroupRoofShape, "RoofShape" );
    static_cast<ADN_Connector_Vector_ABC*>( &pListRoofShape_->GetConnector() )->Connect( &data_.GetRoofShapesInfos() );

    // roofsape
    pGroupRoofShapes = new QVGroupBox( tr( "RoofShape" ),pGroupRoofShapes);
    pGroupRoofShapes->setInsideMargin(20);
    pGroupRoofShapes->setInsideSpacing(20);
    pEdit = new ADN_EditLine_String(pGroupRoofShapes);
    vRoofShapeInfosConnectors[eUrbanName]=&pEdit->GetConnector();

    ///////////////////
    // Accommodations
    QGroupBox* pGroupAccommodations = new QVGroupBox( tr( "Accommodations" ), pBox );
    QHBox* pGroupAccommodation = new QHBox( pGroupAccommodations );

    ADN_GuiBuilder builder;

    // accommodations list
    T_ConnectorVector    vAccommodationInfosConnectors(eNbrUrbanAccommodationGuiElements,(ADN_Connector_ABC*)0 );
    pListAccommodation_= new ADN_ListView_UrbanAccommodation_Type( pGroupAccommodation, "Accommodation" );
    static_cast<ADN_Connector_Vector_ABC*>( &pListAccommodation_->GetConnector() )->Connect( &data_.GetAccommodationsInfos() );

    // accommodation
    pGroupAccommodations = new QHGroupBox( tr( "Accommodation" ),pGroupAccommodations);
    pGroupAccommodations->setInsideMargin(20);
    pGroupAccommodations->setInsideSpacing(20);
    QWidget* pHolder = builder.AddFieldHolder( pGroupAccommodations );

    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Type" ),  vAccommodationInfosConnectors[ eUrbanAccommodationName ] );
    builder.AddField< ADN_EditLine_Double >( pHolder, tr( "Capacity" ), vAccommodationInfosConnectors[ eUrbanAccommodationCapacity ] );

    ///////////////////
    // Infrastructures
    QGroupBox* pGroupInfrastructures = new QVGroupBox( tr( "Infrastructures" ), pBox );
    QHBox* pGroupInfrastructure = new QHBox( pGroupInfrastructures );


    // infrastructures list
    T_ConnectorVector    vInfrastructureInfosConnectors(eNbrUrbanInfrastructureGuiElements,(ADN_Connector_ABC*)0 );
    pListInfrastructure_= new ADN_ListView_UrbanInfrastructure_Type( pGroupInfrastructure, "Infrastructure" );
    static_cast<ADN_Connector_Vector_ABC*>( &pListInfrastructure_->GetConnector() )->Connect( &data_.GetInfrastructuresInfos() );

    // infrastructures
    pGroupInfrastructures = new QHGroupBox( tr( "Infrastructure" ),pGroupInfrastructures);
    pGroupInfrastructures->setInsideMargin(20);
    pGroupInfrastructures->setInsideSpacing(20);
    pHolder = builder.AddFieldHolder( pGroupInfrastructures );

    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Type" ),  vInfrastructureInfosConnectors[ eUrbanInfrastructureName ] );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Symbol" ), vInfrastructureInfosConnectors[ eUrbanInfrastructureSymbol ] );

    ADN_GroupBox* medical = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Medical" ), pGroupInfrastructures );
    {
        vInfrastructureInfosConnectors[ eMedicalCapacityPresent ] = & medical->GetConnector();
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Doctor night rate" ), vInfrastructureInfosConnectors[ eMedicalCapacity_NightRate ], tr( "" ) );
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Emergency doctor rate" ), vInfrastructureInfosConnectors[ eMedicalCapacity_EmergencyDoctorRate ], tr( "" ) );
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Emergency bed rate" ), vInfrastructureInfosConnectors[ eMedicalCapacity_EmergencyBedRate ], tr( "" ) );
    }

    // set auto connectors
    pListMaterial_->SetItemConnectors( vMaterialInfosConnectors );
    pListFacade_  ->SetItemConnectors( vFacadeInfosConnectors );
    pListRoofShape_->SetItemConnectors( vRoofShapeInfosConnectors );
    pListAccommodation_->SetItemConnectors( vAccommodationInfosConnectors );
    pListInfrastructure_->SetItemConnectors( vInfrastructureInfosConnectors );
}
