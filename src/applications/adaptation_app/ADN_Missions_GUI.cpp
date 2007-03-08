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
#include "ADN_GuiBuilder.h"
#include "ADN_ListView_MissionTypes.h"
#include "ADN_ListView_FragOrderTypes.h"
#include "ADN_MissionParameters_Table.h"
#include "ADN_MissionParameterValues_Table.h"
#include "ADN_ComboBox_Vector.h"

#include <qtabwidget.h>
#include <qlayout.h>
#include <qvgroupbox.h>

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
    pTabWidget->addTab( BuildPopulationMissions( pTabWidget ), tr( "Population missions" ) );
    pTabWidget->addTab( BuildFragOrders( pTabWidget ), tr( "Fragmentary orders" ) );

    QGridLayout* pMainLayout = new QGridLayout( pMainWidget_, 1, 1, 10, 10 );
    pMainLayout->addWidget( pTabWidget, 0, 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildMissions( QWidget* parent, ADN_Missions_Data::T_Mission_Vector& missions )
{
    ADN_GuiBuilder builder;

    // Create the main widget
    QWidget* mainWidget = new QWidget( parent );

    // Missions listview
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );
    ADN_ListView_MissionTypes* listMissions = new ADN_ListView_MissionTypes( missions, mainWidget );
    listMissions->GetConnector().Connect( &missions );

    // Mission data
    QGroupBox* pGroup = new QVGroupBox( tr( "Mission" ), mainWidget );
    QWidget* pParamHolder = builder.AddFieldHolder( pGroup );
    builder.AddField<ADN_EditLine_String>( pParamHolder, tr( "Name" ), vInfosConnectors[eName] );

    QGroupBox* pParameters = new QGroupBox( 2, Qt::Horizontal, tr( "Parameters" ), pGroup );
    ADN_MissionParameters_Table* paramList = new ADN_MissionParameters_Table( pParameters );
    vInfosConnectors[eParameters] = &paramList->GetConnector();
    QGroupBox* pEnum = new QGroupBox( 1, Qt::Horizontal, tr( "Enumeration values" ), pParameters );
    ADN_MissionParameterValues_Table* valueList = new ADN_MissionParameterValues_Table( pEnum, "" );
    vInfosConnectors[eParameterValues] = &valueList->GetConnector();
    
    // Connect the gui to the data.
    paramList->SetItemConnectors( vInfosConnectors );
    listMissions->SetItemConnectors( vInfosConnectors );

    // Layout
    QBoxLayout* pLayout = new QHBoxLayout( mainWidget, 5, 5 );
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
    return BuildMissions( parent, data_.unitMissions_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildAutomatMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildAutomatMissions( QWidget* parent )
{
    return BuildMissions( parent, data_.automatMissions_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildPopulationMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildPopulationMissions( QWidget* parent )
{
    return BuildMissions( parent, data_.populationMissions_ );    
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
    QGroupBox* pGroup = new QVGroupBox( tr( "Fragmentary orders" ), mainWidget );
    QWidget* pParamHolder = builder.AddFieldHolder( pGroup );
    builder.AddField< ADN_EditLine_String >( pParamHolder, tr( "Name" ), vInfosConnectors[eName] );
    builder.AddField< ADN_CheckBox >( pParamHolder, tr( "Available for all missions" ), vInfosConnectors[eFragOrderAvailableForAllMissions] );
    builder.AddField< ADN_CheckBox >( pParamHolder, tr( "Available without mission" ) , vInfosConnectors[eFragOrderAvailableWithoutMission] );

    QGroupBox* pParameters = new QGroupBox( 2, Qt::Horizontal, tr( "Parameters" ), pGroup );
    ADN_MissionParameters_Table* paramList = new ADN_MissionParameters_Table( pParameters );
    vInfosConnectors[eParameters] = &paramList->GetConnector();
    QGroupBox* pEnum = new QGroupBox( 1, Qt::Horizontal, tr( "Enumeration values" ), pParameters );
    ADN_MissionParameterValues_Table* valueList = new ADN_MissionParameterValues_Table( pEnum, "" );
    vInfosConnectors[eParameterValues] = &valueList->GetConnector();

    // Connect the gui to the data.
    paramList->SetItemConnectors( vInfosConnectors );
    listFragOrders->SetItemConnectors( vInfosConnectors );

    // Layout
    QBoxLayout* pLayout = new QHBoxLayout( mainWidget, 5, 5 );
    pLayout->addWidget( listFragOrders, 1 );
    pLayout->addWidget( pGroup, 5 );
    return mainWidget;   
}