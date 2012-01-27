// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_GUI.h"
#include "ADN_Missions_Data.h"
#include "ADN_MissionTypes_Table.h"
#include "ADN_GuiBuilder.h"
#include "ADN_ListView_MissionTypes.h"
#include "ADN_ListView_FragOrderTypes.h"
#include "ADN_MissionParameters_Table.h"
#include "ADN_MissionParameterValues_Table.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_ComboBox_Drawings.h"
#include "ADN_Drawings_Data.h"
#include "ADN_SearchListView.h"

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Missions_GUI::ADN_Missions_GUI( ADN_Missions_Data& data )
    : ADN_GUI_ABC( "ADN_Missions_GUI" )
    , data_      ( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Missions_GUI::~ADN_Missions_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    // Tab management
    QTabWidget* pTabWidget = new QTabWidget( pMainWidget_ );
    pTabWidget->addTab( BuildUnitMissions(), tr( "Unit missions" ) );
    pTabWidget->addTab( BuildAutomatMissions(), tr( "Automat missions" ) );
    pTabWidget->addTab( BuildPopulationMissions(), tr( "Crowd missions" ) );
    pTabWidget->addTab( BuildFragOrders(), tr( "Fragmentary orders" ) );

    // Main widget
    pMainWidget_ = new QWidget();
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_ );
    pMainLayout->setSpacing( 10 );
    pMainLayout->setMargin( 10 );
    pMainLayout->addWidget( pTabWidget );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildMissions( QWidget*& pContent, ADN_Missions_Data::T_Mission_Vector& missions, ADN_Models_Data::ModelInfos::E_ModelEntityType eEntityType )
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    ADN_GuiBuilder builder;
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vInfosConnectors[eName] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Doctrine description" ), vInfosConnectors[eDoctrineDescription] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Usage description" ), vInfosConnectors[eUsageDescription] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Type" ), vInfosConnectors[eDiaType] );
    if( eEntityType == ADN_Models_Data::ModelInfos::eAutomat )
    {
        builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "MRT Behavior" ), vInfosConnectors[eMRTBehavior] );
        builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "CDT Behavior" ), vInfosConnectors[eCDTBehavior] );
    }
    else
        builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Behavior" ), vInfosConnectors[eBehavior] );

    QComboBox* combo = builder.AddField< ADN_ComboBox_Drawings< ADN_Drawings_Data::DrawingInfo > >( pInfoHolder, tr( "Symbol" ), vInfosConnectors[ eSymbol ] );
    combo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );

    // Parameters
    QGroupBox* pParametersGroup = new QGroupBox( tr( "Parameters" ) );
    ADN_MissionParameters_Table* paramList = new ADN_MissionParameters_Table();
    vInfosConnectors[eParameters] = &paramList->GetConnector();
    Q3GroupBox* pEnum = new Q3GroupBox( 1, Qt::Horizontal, tr( "Enumeration values" ) );
    ADN_MissionParameterValues_Table* valueList = new ADN_MissionParameterValues_Table( pEnum, "" );
    vInfosConnectors[eParameterValues] = &valueList->GetConnector();

    Q3GroupBox* pChoice = new Q3GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ) );
    ADN_MissionTypes_Table* choiceList = new ADN_MissionTypes_Table( pChoice, "" );
    vInfosConnectors[eChoiceValues] = &choiceList->GetConnector();

    // Connect the gui to the data.
    paramList->SetItemConnectors( vInfosConnectors );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Parameters layout
    QGridLayout* parameterLayout = new QGridLayout( pParametersGroup, 2, 2 );
    parameterLayout->setMargin( 10 );
    parameterLayout->setSpacing( 10 );
    parameterLayout->addWidget( paramList, 0, 0, 1, 2 );
    parameterLayout->addWidget( pEnum );
    parameterLayout->addWidget( pChoice );

    // Content layout
    pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( pParametersGroup );

    // List view
    ADN_SearchListView< ADN_ListView_MissionTypes >* pSearchListView = new ADN_SearchListView< ADN_ListView_MissionTypes >( eEntityType, missions, missions, vInfosConnectors );
    connect( pSearchListView->GetListView(), SIGNAL( UsersListRequested( const ADN_UsedByInfos& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_UsedByInfos& ) ) );

    // Main page
    return CreateScrollArea( *pContent, pSearchListView );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildUnitMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildUnitMissions()
{
    return BuildMissions( pUnitMissionsWidget_, data_.unitMissions_, ADN_Models_Data::ModelInfos::ePawn );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildAutomatMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildAutomatMissions()
{
    return BuildMissions( pAutomatMissionsWidget_, data_.automatMissions_, ADN_Models_Data::ModelInfos::eAutomat );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildPopulationMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildPopulationMissions()
{
    return BuildMissions( pPopulationMissionsWidget_, data_.populationMissions_, ADN_Models_Data::ModelInfos::ePopulation );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildFragOrders
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildFragOrders()
{
    ADN_GuiBuilder builder;
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    // Content
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vInfosConnectors[eName] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Doctrine description" ), vInfosConnectors[eDoctrineDescription] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Usage description" ), vInfosConnectors[eUsageDescription] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Type" ), vInfosConnectors[eDiaType] );
    QCheckBox* available = builder.AddField< ADN_CheckBox >( pInfoHolder, tr( "Available without mission" ) , vInfosConnectors[eFragOrderAvailableWithoutMission] );

    // Parameters
    QGroupBox* pParametersGroup = new QGroupBox( tr( "Parameters" ) );
    ADN_MissionParameters_Table* paramList = new ADN_MissionParameters_Table();
    vInfosConnectors[eParameters] = &paramList->GetConnector();
    Q3GroupBox* pEnum = new Q3GroupBox( 1, Qt::Horizontal, tr( "Enumeration values" ) );
    ADN_MissionParameterValues_Table* valueList = new ADN_MissionParameterValues_Table( pEnum, "" );
    vInfosConnectors[eParameterValues] = &valueList->GetConnector();

    Q3GroupBox* pChoice = new Q3GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ) );
    ADN_MissionTypes_Table* choiceList = new ADN_MissionTypes_Table( pChoice, "" );
    vInfosConnectors[eChoiceValues] = &choiceList->GetConnector();

    // Connect the gui to the data.
    paramList->SetItemConnectors( vInfosConnectors );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Parameters layout
    QGridLayout* parameterLayout = new QGridLayout( pParametersGroup, 2, 2 );
    parameterLayout->setMargin( 10 );
    parameterLayout->setSpacing( 10 );
    parameterLayout->addWidget( paramList, 0, 0, 1, 2 );
    parameterLayout->addWidget( pEnum );
    parameterLayout->addWidget( pChoice );

    // Content layout
    pFragOrderWidget_ = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pFragOrderWidget_ );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( pParametersGroup );

    // List view
    ADN_SearchListView< ADN_ListView_FragOrderTypes >* pSearchListView = new ADN_SearchListView< ADN_ListView_FragOrderTypes >( data_.fragOrders_, data_.fragOrders_, vInfosConnectors );
    connect( available, SIGNAL( toggled ( bool ) ), pSearchListView->GetListView(), SLOT( OnToogled( bool ) ) );

    // Main page
    return CreateScrollArea( *pFragOrderWidget_, pSearchListView );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::Enable
// Created: JSR 2010-05-21
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::Enable( bool enable )
{
    pUnitMissionsWidget_->setEnabled( enable );
    pAutomatMissionsWidget_->setEnabled( enable );
    pPopulationMissionsWidget_->setEnabled( enable );
    pFragOrderWidget_->setEnabled( enable );
}
