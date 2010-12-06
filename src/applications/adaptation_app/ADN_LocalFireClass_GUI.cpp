// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_LocalFireClass_GUI.h"
#include "moc_ADN_LocalFireClass_GUI.cpp"
#include "ADN_WeatherFireEffects_Table.h"
#include "ADN_ExtinguisherAgentInfos_Table.h"
#include "ADN_LocalFireClass_ListView.h"
#include "ADN_LocalFireClass_Data.h"
#include "ADN_GroupBox.h"
#include "ADN_GuiBuilder.h"
#include "ADN_SurfaceFireInfos_Table.h"
#include "ADN_UrbanModifiersTable.h"

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_GUI constructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_GUI::ADN_LocalFireClass_GUI( ADN_LocalFireClass_Data& data )
    : ADN_GUI_ABC( "ADN_LocalFireClass_GUI" )
    , data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_GUI destructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_GUI::~ADN_LocalFireClass_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_GUI::Build
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );

    // Local fire class list
    ADN_LocalFireClass_ListView* pLocalFireClassListView = new ADN_LocalFireClass_ListView( pMainWidget_ );
    pLocalFireClassListView->GetConnector().Connect( &data_.GetLocalFireClassesInfos() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Local fire class data
    QGroupBox* pLocalFireClassDataGroup = new QGroupBox( 2, Qt::Horizontal, tr( "Fire" ), pMainWidget_ );

    ADN_GuiBuilder builder;
    builder.AddField< ADN_EditLine_Int >( builder.AddFieldHolder( pLocalFireClassDataGroup ), tr( "Cell size (global)" ), vInfosConnectors[ eCellSize ], tr( "m" ), eGreaterZero );
    vInfosConnectors[ eCellSize ]->Connect( &data_.GetCellSize() );

    new QWidget( pLocalFireClassDataGroup );

    QWidget* pHolder = builder.AddFieldHolder( pLocalFireClassDataGroup );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Name" ), vInfosConnectors[ eName ] );
    builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Initial heat" ), vInfosConnectors[ eInitialHeat ] );
    builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Max heat" ), vInfosConnectors[ eMaxHeat ] );
    builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Increase rate" ), vInfosConnectors[ eIncreaseRate ] );
    builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Decrease rate" ), vInfosConnectors[ eDecreaseRate ] );

    QGroupBox* pInjuriesGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Injuries" ), pLocalFireClassDataGroup );
    QWidget* pInjuriesHolder = builder.AddFieldHolder( pInjuriesGroup );
    builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded seriousness level 1" ), vInfosConnectors[ eNbrHurtU1 ], tr( "%" ), ePercentage  );
    builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded seriousness level 2" ), vInfosConnectors[ eNbrHurtU2 ], tr( "%" ), ePercentage  );
    builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded seriousness level 3" ), vInfosConnectors[ eNbrHurtU3 ], tr( "%" ), ePercentage  );
    builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded extreme seriousness" ), vInfosConnectors[ eNbrHurtUE ], tr( "%" ), ePercentage  );
    builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Killed" ), vInfosConnectors[ eNbrDead ], tr( "%" ), ePercentage  );

    QGroupBox* pUrbanModifiersGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Urban attrition" ), pLocalFireClassDataGroup );
    new helpers::ADN_UrbanModifiersTable( pUrbanModifiersGroup, vInfosConnectors[ eUrbanAttrition ] );

    QGroupBox* pExtinguisherAgentsGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Extinguisher agents" ), pLocalFireClassDataGroup );
    ADN_ExtinguisherAgentInfos_Table* pExtinguisherAgentInfosTable = new ADN_ExtinguisherAgentInfos_Table( pExtinguisherAgentsGroup );
    vInfosConnectors[ eExtinguisherAgents ] = &pExtinguisherAgentInfosTable->GetConnector();

    QGroupBox* pWeatherFireEffectsGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Weather fire effects" ), pLocalFireClassDataGroup );
    ADN_WeatherFireEffects_Table* pWeatherFireEffectsTable = new ADN_WeatherFireEffects_Table( pWeatherFireEffectsGroup );
    vInfosConnectors[ eWeatherFireEffects ] = &pWeatherFireEffectsTable->GetConnector();

    ADN_GroupBox* surfaceGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Surface" ), pLocalFireClassDataGroup );
    vInfosConnectors[ eSurface ] = &surfaceGroup->GetConnector();
    ADN_SurfaceFireInfos_Table *pSurfaceInfosTable = new ADN_SurfaceFireInfos_Table( surfaceGroup );
    vInfosConnectors[ eSurfaceEffects ] = &pSurfaceInfosTable->GetConnector();

    pLocalFireClassListView->SetItemConnectors( vInfosConnectors );

    // Main vertical layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pLocalFireClassListView, 1 );
    pMainLayout->addWidget( pLocalFireClassDataGroup, 4 );
}
