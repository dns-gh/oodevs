// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_FireClass_GUI.h"
#include "moc_ADN_FireClass_GUI.cpp"
#include "ADN_WeatherFireEffects_Table.h"
#include "ADN_ExtinguisherAgentInfos_Table.h"
#include "ADN_FireClass_ListView.h"
#include "ADN_FireClass_Data.h"
#include "ADN_GroupBox.h"
#include "ADN_GuiBuilder.h"
#include "ADN_SurfaceFireInfos_Table.h"
#include "ADN_UrbanModifiersTable.h"

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_GUI constructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_FireClass_GUI::ADN_FireClass_GUI( ADN_FireClass_Data& data )
    : ADN_GUI_ABC( "ADN_FireClass_GUI" )
    , data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_GUI destructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_FireClass_GUI::~ADN_FireClass_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_GUI::Build
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_FireClass_GUI::Build()
{
    assert( pMainWidget_ == 0 );

     ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );

    // Local fire class list
    ADN_FireClass_ListView* pFireClassListView = new ADN_FireClass_ListView( pMainWidget_ );
    pFireClassListView->GetConnector().Connect( &data_.GetFireClassesInfos() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    //Global fire parameters
    Q3GroupBox* pGlobalFireParametersDataGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Global fire parameters" ), pMainWidget_ );
    builder.AddField< ADN_EditLine_Int >( pGlobalFireParametersDataGroup, tr( "Cell size" ), vInfosConnectors[ eCellSize ], tr( "m" ), eGreaterZero );
    vInfosConnectors[ eCellSize ]->Connect( &data_.GetCellSize() );

    // Local fire class data
    Q3GroupBox* pFireClassDataGroup = new Q3GroupBox( 2, Qt::Horizontal, tr( "Fire" ), pMainWidget_ );
    //new QWidget( pFireClassDataGroup );
    QWidget* pHolder = builder.AddFieldHolder( pFireClassDataGroup );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Name" ), vInfosConnectors[ eName ] );
    builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Initial heat" ), vInfosConnectors[ eInitialHeat ] );
    builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Max heat" ), vInfosConnectors[ eMaxHeat ] );
    builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Increase rate" ), vInfosConnectors[ eIncreaseRate ] );
    builder.AddField< ADN_EditLine_Int >( pHolder, tr( "Decrease rate" ), vInfosConnectors[ eDecreaseRate ] );

    Q3GroupBox* pInjuriesGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Injuries" ), pFireClassDataGroup );
    QWidget* pInjuriesHolder = builder.AddFieldHolder( pInjuriesGroup );
    builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded seriousness level 1" ), vInfosConnectors[ eNbrHurtU1 ], tr( "%" ), ePercentage  );
    builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded seriousness level 2" ), vInfosConnectors[ eNbrHurtU2 ], tr( "%" ), ePercentage  );
    builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded seriousness level 3" ), vInfosConnectors[ eNbrHurtU3 ], tr( "%" ), ePercentage  );
    builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded extreme seriousness" ), vInfosConnectors[ eNbrHurtUE ], tr( "%" ), ePercentage  );
    builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Killed" ), vInfosConnectors[ eNbrDead ], tr( "%" ), ePercentage  );

    Q3GroupBox* pUrbanModifiersGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Urban attrition" ), pFireClassDataGroup );
    new helpers::ADN_UrbanModifiersTable( pUrbanModifiersGroup, vInfosConnectors[ eUrbanAttrition ] );

    Q3GroupBox* pExtinguisherAgentsGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Extinguisher agents" ), pFireClassDataGroup );
    ADN_ExtinguisherAgentInfos_Table* pExtinguisherAgentInfosTable = new ADN_ExtinguisherAgentInfos_Table( pExtinguisherAgentsGroup );
    vInfosConnectors[ eExtinguisherAgents ] = &pExtinguisherAgentInfosTable->GetConnector();

    Q3GroupBox* pWeatherFireEffectsGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Weather fire effects" ), pFireClassDataGroup );
    ADN_WeatherFireEffects_Table* pWeatherFireEffectsTable = new ADN_WeatherFireEffects_Table( pWeatherFireEffectsGroup );
    vInfosConnectors[ eWeatherFireEffects ] = &pWeatherFireEffectsTable->GetConnector();

    ADN_GroupBox* surfaceGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Surface" ), pFireClassDataGroup );
    vInfosConnectors[ eSurface ] = &surfaceGroup->GetConnector();
    ADN_SurfaceFireInfos_Table *pSurfaceInfosTable = new ADN_SurfaceFireInfos_Table( surfaceGroup );
    vInfosConnectors[ eSurfaceEffects ] = &pSurfaceInfosTable->GetConnector();

    pFireClassListView->SetItemConnectors( vInfosConnectors );

    // Main vertical layout
    Q3VBoxLayout* pMainLayout = new Q3VBoxLayout( pMainWidget_, 10, 10 );

    Q3HBoxLayout* pGlobalLayout = new Q3HBoxLayout();
    pGlobalLayout->addWidget( pGlobalFireParametersDataGroup );

    QBoxLayout* pListAndDataLayout = new QHBoxLayout( 0, 10, 10 );
    pListAndDataLayout->addWidget( pFireClassListView, 1 );
    pListAndDataLayout->addWidget( pFireClassDataGroup, 4 );

    pMainLayout->addLayout( pGlobalLayout );
    pMainLayout->addLayout( pListAndDataLayout, 2 );
}
