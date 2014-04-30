// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Fires_GUI.h"
#include "moc_ADN_Fires_GUI.cpp"
#include "ADN_WeatherFireEffects_Table.h"
#include "ADN_ExtinguisherAgentInfos_Table.h"
#include "ADN_Fires_ListView.h"
#include "ADN_Fires_Data.h"
#include "ADN_GroupBox.h"
#include "ADN_GuiBuilder.h"
#include "ADN_MultiPercentage.h"
#include "ADN_SurfaceFireInfos_Table.h"
#include "ADN_UrbanModifiersTable.h"

// -----------------------------------------------------------------------------
// Name: ADN_Fires_GUI constructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_Fires_GUI::ADN_Fires_GUI( ADN_Fires_Data& data )
    : ADN_GUI_ABC( eFires )
    , data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Fires_GUI destructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_Fires_GUI::~ADN_Fires_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Fires_GUI::Build
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_Fires_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Global fire parameters
    Q3GroupBox* pGlobalFireParametersDataGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Global fire parameters" ) );
    builder.AddField< ADN_EditLine_Int >( pGlobalFireParametersDataGroup, "cell-size", tr( "Cell size" ), vInfosConnectors[ eCellSize ], tr( "m" ), eGreaterZero );
    vInfosConnectors[ eCellSize ]->Connect( &data_.GetCellSize() );

    // Local fire parameters
    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );

    builder.AddLocalizedField( data_.GetFireClassesInfos(), pInfoHolder, "name", tr( "Name" ), vInfosConnectors[ eName ] );

    builder.AddField< ADN_EditLine_Int >( pInfoHolder, "initial-heat", tr( "Initial heat" ), vInfosConnectors[ eInitialHeat ] );
    builder.AddField< ADN_EditLine_Int >( pInfoHolder, "max-heat", tr( "Max heat" ), vInfosConnectors[ eMaxHeat ] );
    builder.AddField< ADN_EditLine_Int >( pInfoHolder, "increase-rate", tr( "Increase rate" ), vInfosConnectors[ eIncreaseRate ] );
    builder.AddField< ADN_EditLine_Int >( pInfoHolder, "decrease-rate", tr( "Decrease rate" ), vInfosConnectors[ eDecreaseRate ] );
    // Injuries
    Q3GroupBox* pInjuriesGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Injuries" ) );
    ADN_MultiPercentage_Int* pMultiPercentage = new ADN_MultiPercentage_Int( pInjuriesGroup, builder, builder.GetChildName( "distribution" ) );
    pMultiPercentage->AddLine( tr( "Wounded seriousness level 1" ), vInfosConnectors[ eNbrHurtU1 ], "u1" );
    pMultiPercentage->AddLine( tr( "Wounded seriousness level 2" ), vInfosConnectors[ eNbrHurtU2 ], "u2" );
    pMultiPercentage->AddLine( tr( "Wounded seriousness level 3" ), vInfosConnectors[ eNbrHurtU3 ], "u3" );
    pMultiPercentage->AddLine( tr( "Wounded extreme seriousness" ), vInfosConnectors[ eNbrHurtUE ], "ue" );
    pMultiPercentage->AddLine( tr( "Killed" ), vInfosConnectors[ eNbrDead ], "killed" );
    pMultiPercentage->AddWarning();

    // Urban
    Q3GroupBox* pUrbanModifiersGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Urban attrition" ) );
    new helpers::ADN_UrbanModifiersTable( builder.GetChildName( "urban-modifiers" ), vInfosConnectors[ eUrbanAttrition ], pUrbanModifiersGroup );
    // Extinguisher
    Q3GroupBox* pExtinguisherAgentsGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Extinguisher agents" ) );
    ADN_ExtinguisherAgentInfos_Table* pExtinguisherAgentInfosTable = new ADN_ExtinguisherAgentInfos_Table( builder.GetChildName( "extinguisher" ), vInfosConnectors[ eExtinguisherAgents ], pExtinguisherAgentsGroup );
    pExtinguisherAgentInfosTable->SetGoToOnDoubleClick( ::eSupplies );
    // Weather fire effects
    Q3GroupBox* pWeatherFireEffectsGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Weather fire effects" ) );
    new ADN_WeatherFireEffects_Table( builder.GetChildName( "weather" ), vInfosConnectors[ eWeatherFireEffects ], pWeatherFireEffectsGroup );
    // Surface
    ADN_GroupBox* surfaceGroup = builder.AddGroupBox( 0, "surface", tr( "Surface" ), vInfosConnectors[ eSurface ], 1 );
    new ADN_SurfaceFireInfos_Table( builder.GetChildName( "surface-effect" ), vInfosConnectors[ eSurfaceEffects ], surfaceGroup );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Fireclass layout
    QWidget* pFireClassContent = new QWidget();
    QGridLayout* pFireClassLayout = new QGridLayout( pFireClassContent, 3, 2 );
    pFireClassLayout->setMargin( 10 );
    pFireClassLayout->setSpacing( 10 );
    pFireClassLayout->setAlignment( Qt::AlignTop );
    pFireClassLayout->addWidget( pInfoHolder );
    pFireClassLayout->addWidget( pInjuriesGroup );
    pFireClassLayout->addWidget( pUrbanModifiersGroup );
    pFireClassLayout->addWidget( pExtinguisherAgentsGroup );
    pFireClassLayout->addWidget( pWeatherFireEffectsGroup );
    pFireClassLayout->addWidget( surfaceGroup );

    // List view
    QWidget* pSearchListView = builder.AddSearchListView< ADN_Fires_ListView >( this, data_.GetFireClassesInfos(), vInfosConnectors );
    // Sub content
    QWidget* pSubContent = CreateScrollArea( builder.GetChildName( "content" ), *pFireClassContent, pSearchListView, false, false, true, 0, 0 );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pGlobalFireParametersDataGroup );
    pContentLayout->addWidget( pSubContent, 1 );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent );
}
