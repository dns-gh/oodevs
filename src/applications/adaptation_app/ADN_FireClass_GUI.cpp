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
#include "ADN_MultiPercentage.h"
#include "ADN_SearchListView.h"
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
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Global fire parameters
    Q3GroupBox* pGlobalFireParametersDataGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Global fire parameters" ) );
    builder.AddField< ADN_EditLine_Int >( pGlobalFireParametersDataGroup, tr( "Cell size" ), vInfosConnectors[ eCellSize ], tr( "m" ), eGreaterZero );
    vInfosConnectors[ eCellSize ]->Connect( &data_.GetCellSize() );

    // Local fire parameters
    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vInfosConnectors[ eName ] );
    builder.AddField< ADN_EditLine_Int >( pInfoHolder, tr( "Initial heat" ), vInfosConnectors[ eInitialHeat ] );
    builder.AddField< ADN_EditLine_Int >( pInfoHolder, tr( "Max heat" ), vInfosConnectors[ eMaxHeat ] );
    builder.AddField< ADN_EditLine_Int >( pInfoHolder, tr( "Increase rate" ), vInfosConnectors[ eIncreaseRate ] );
    builder.AddField< ADN_EditLine_Int >( pInfoHolder, tr( "Decrease rate" ), vInfosConnectors[ eDecreaseRate ] );
    // Injuries
    Q3GroupBox* pInjuriesGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Injuries" ) );
    ADN_MultiPercentage_Int* pMultiPercentage = new ADN_MultiPercentage_Int( pInjuriesGroup, builder, strClassName_ + "_Distribution" );
    pMultiPercentage->AddLine( tr( "Wounded seriousness level 1" ), vInfosConnectors[ eNbrHurtU1 ] );
    pMultiPercentage->AddLine( tr( "Wounded seriousness level 2" ), vInfosConnectors[ eNbrHurtU2 ] );
    pMultiPercentage->AddLine( tr( "Wounded seriousness level 3" ), vInfosConnectors[ eNbrHurtU3 ] );
    pMultiPercentage->AddLine( tr( "Wounded extreme seriousness" ), vInfosConnectors[ eNbrHurtUE ] );
    pMultiPercentage->AddLine( tr( "Killed" ), vInfosConnectors[ eNbrDead ] );
    pMultiPercentage->AddWarning();

    // Urban
    Q3GroupBox* pUrbanModifiersGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Urban attrition" ) );
    new helpers::ADN_UrbanModifiersTable( strClassName_ + "Urban", vInfosConnectors[ eUrbanAttrition ], pUrbanModifiersGroup );
    // Extinguisher
    Q3GroupBox* pExtinguisherAgentsGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Extinguisher agents" ) );
    ADN_ExtinguisherAgentInfos_Table* pExtinguisherAgentInfosTable = new ADN_ExtinguisherAgentInfos_Table( strClassName_ + "Extinguisher", vInfosConnectors[ eExtinguisherAgents ], pExtinguisherAgentsGroup );
    pExtinguisherAgentInfosTable->SetGoToOnDoubleClick( ::eEquipement );
    // Weather fire effects
    Q3GroupBox* pWeatherFireEffectsGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Weather fire effects" ) );
    new ADN_WeatherFireEffects_Table( strClassName_ + "Weather", vInfosConnectors[ eWeatherFireEffects ], pWeatherFireEffectsGroup );
    // Surface
    ADN_GroupBox* surfaceGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Surface" ) );
    vInfosConnectors[ eSurface ] = &surfaceGroup->GetConnector();
    new ADN_SurfaceFireInfos_Table( strClassName_ + "Surface", vInfosConnectors[ eSurfaceEffects ], surfaceGroup );

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
    ADN_SearchListView< ADN_FireClass_ListView >* pSearchListView = new ADN_SearchListView< ADN_FireClass_ListView >( this, data_.GetFireClassesInfos(), vInfosConnectors );
    pSearchListView->GetListView()->setObjectName( strClassName_ + "_List" );
    pListView_ = pSearchListView->GetListView();
    pListView_->setObjectName( strClassName_ + "_List" );
    // Sub content
    QWidget* pSubContent = CreateScrollArea( *pFireClassContent, pSearchListView, false, false, true, 0, 0 );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pGlobalFireParametersDataGroup );
    pContentLayout->addWidget( pSubContent, 1 );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent );
    pMainWidget_->setObjectName( strClassName_ );
}
