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

#include <QtGui/qtabwidget.h>
#include <QtGui/qlayout.h>
#include <Qt3Support/q3vgroupbox.h>
//Added by qt3to4:
#include <Qt3Support/q3gridlayout.h>

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

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );
    QTabWidget* pTabWidget = new QTabWidget( pMainWidget_ );

    pTabWidget->addTab( BuildUnitMissions( pTabWidget ), tr( "Unit missions" ) );
    pTabWidget->addTab( BuildAutomatMissions( pTabWidget ), tr( "Automat missions" ) );
    pTabWidget->addTab( BuildPopulationMissions( pTabWidget ), tr( "Crowd missions" ) );
    pTabWidget->addTab( BuildFragOrders( pTabWidget ), tr( "Fragmentary orders" ) );

    Q3GridLayout* pMainLayout = new Q3GridLayout( pMainWidget_, 1, 1, 10, 10 );
    pMainLayout->addWidget( pTabWidget, 0, 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildMissions( Q3GroupBox*& pGroup, QWidget* parent, ADN_Missions_Data::T_Mission_Vector& missions, bool automat )
{
    ADN_GuiBuilder builder;

    // Create the main widget
    QWidget* mainWidget = new QWidget( parent );

    // Missions listview
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );
    ADN_ListView_MissionTypes* listMissions = new ADN_ListView_MissionTypes( missions, mainWidget );
    listMissions->GetConnector().Connect( &missions );

    // Mission data
    pGroup = new Q3VGroupBox( tr( "Mission" ), mainWidget );
    QWidget* pParamHolder = builder.AddFieldHolder( pGroup );
    builder.AddField< ADN_EditLine_String >( pParamHolder, tr( "Name" ), vInfosConnectors[eName] );
    builder.AddField< ADN_EditLine_String >( pParamHolder, tr( "Doctrine description" ), vInfosConnectors[eDoctrineDescription] );
    builder.AddField< ADN_EditLine_String >( pParamHolder, tr( "Usage description" ), vInfosConnectors[eUsageDescription] );
    builder.AddField< ADN_EditLine_String >( pParamHolder, tr( "Type" ), vInfosConnectors[eDiaType] );
    if( automat )
    {
        builder.AddField< ADN_EditLine_String >( pParamHolder, tr( "MRT Behavior" ), vInfosConnectors[eMRTBehavior] );
        builder.AddField< ADN_EditLine_String >( pParamHolder, tr( "CDT Behavior" ), vInfosConnectors[eCDTBehavior] );
    }
    else
        builder.AddField< ADN_EditLine_String >( pParamHolder, tr( "Behavior" ), vInfosConnectors[eBehavior] );

    Q3GroupBox* pParameters = new Q3GroupBox( 2, Qt::Horizontal, tr( "Parameters" ), pGroup );
    ADN_MissionParameters_Table* paramList = new ADN_MissionParameters_Table( pParameters );
    vInfosConnectors[eParameters] = &paramList->GetConnector();
    Q3GroupBox* pEnum = new Q3GroupBox( 1, Qt::Horizontal, tr( "Enumeration values" ), pParameters );
    ADN_MissionParameterValues_Table* valueList = new ADN_MissionParameterValues_Table( pEnum, "" );
    vInfosConnectors[eParameterValues] = &valueList->GetConnector();

    Q3GroupBox* pChoice = new Q3GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ), pParameters );
    ADN_MissionTypes_Table* choiceList = new ADN_MissionTypes_Table( pChoice, "" );
    vInfosConnectors[eChoiceValues] = &choiceList->GetConnector();

    // Connect the gui to the data.
    paramList->SetItemConnectors( vInfosConnectors );
    listMissions->SetItemConnectors( vInfosConnectors );

    // Layout
    Q3BoxLayout* pLayout = new Q3HBoxLayout( mainWidget, 5, 5 );
    pLayout->addWidget( listMissions, 1 );
    pLayout->addWidget( pGroup, 5 );

    return mainWidget;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildUnitMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildUnitMissions( QWidget* parent )
{
    return BuildMissions( pUnitMissionsGroup_, parent, data_.unitMissions_, false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildAutomatMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildAutomatMissions( QWidget* parent )
{
    return BuildMissions( pAutomatMissionsGroup_, parent, data_.automatMissions_, true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildPopulationMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildPopulationMissions( QWidget* parent )
{
    return BuildMissions( pPopulationMissionsGroup_, parent, data_.populationMissions_, false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildFragOrders
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildFragOrders( QWidget* parent )
{
    ADN_GuiBuilder builder;

    // Create the main widget
    QWidget* mainWidget = new QWidget( parent );

    // FragOrders listview
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );
    ADN_ListView_FragOrderTypes* listFragOrders = new ADN_ListView_FragOrderTypes( data_.fragOrders_, mainWidget );
    listFragOrders->GetConnector().Connect( &data_.fragOrders_ );

    // Mission data
    pFragOrderGroup_ = new Q3VGroupBox( tr( "Fragmentary orders" ), mainWidget );
    QWidget* pParamHolder = builder.AddFieldHolder( pFragOrderGroup_ );
    builder.AddField< ADN_EditLine_String >( pParamHolder, tr( "Name" ), vInfosConnectors[eName] );
    builder.AddField< ADN_EditLine_String >( pParamHolder, tr( "Doctrine description" ), vInfosConnectors[eDoctrineDescription] );
    builder.AddField< ADN_EditLine_String >( pParamHolder, tr( "Usage description" ), vInfosConnectors[eUsageDescription] );
    builder.AddField< ADN_EditLine_String >( pParamHolder, tr( "Type" ), vInfosConnectors[eDiaType] );
    QCheckBox* available = builder.AddField< ADN_CheckBox >( pParamHolder, tr( "Available without mission" ) , vInfosConnectors[eFragOrderAvailableWithoutMission] );
    connect( available, SIGNAL( toggled ( bool ) ), listFragOrders, SLOT( OnToogled( bool ) ) );

    Q3GroupBox* pParameters = new Q3GroupBox( 2, Qt::Horizontal, tr( "Parameters" ), pFragOrderGroup_ );
    ADN_MissionParameters_Table* paramList = new ADN_MissionParameters_Table( pParameters );
    vInfosConnectors[eParameters] = &paramList->GetConnector();
    Q3GroupBox* pEnum = new Q3GroupBox( 1, Qt::Horizontal, tr( "Enumeration values" ), pParameters );
    ADN_MissionParameterValues_Table* valueList = new ADN_MissionParameterValues_Table( pEnum, "" );
    vInfosConnectors[eParameterValues] = &valueList->GetConnector();

    Q3GroupBox* pChoice = new Q3GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ), pParameters );
    ADN_MissionTypes_Table* choiceList = new ADN_MissionTypes_Table( pChoice, "" );
    vInfosConnectors[eChoiceValues] = &choiceList->GetConnector();

    // Connect the gui to the data.
    paramList->SetItemConnectors( vInfosConnectors );
    listFragOrders->SetItemConnectors( vInfosConnectors );

    // Layout
    Q3BoxLayout* pLayout = new Q3HBoxLayout( mainWidget, 5, 5 );
    pLayout->addWidget( listFragOrders, 1 );
    pLayout->addWidget( pFragOrderGroup_, 5 );

    return mainWidget;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::Enable
// Created: JSR 2010-05-21
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::Enable( bool enable )
{
    pUnitMissionsGroup_->setEnabled( enable );
    pAutomatMissionsGroup_->setEnabled( enable );
    pPopulationMissionsGroup_->setEnabled( enable );
    pFragOrderGroup_->setEnabled( enable );
}
