// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Crowds_GUI.h"
#include "ADN_MainWindow.h"
#include "ADN_App.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_Crowds_Data.h"
#include "ADN_Connector_Vector_ABC.h"
#include "ADN_GoToButton.h"
#include "ADN_MultiPercentage.h"
#include "ADN_Crowds_ListView.h"
#include "ADN_Crowds_SpeedEffect_Attitude_ListView.h"
#include "ADN_Crowds_SpeedEffect_Volume_ListView.h"
#include "ADN_Crowds_FireEffect_Attitude_ListView.h"
#include "ADN_Crowds_FireEffect_Protection_ListView.h"
#include "ADN_Crowds_UrbanEffect_Attitude_ListView.h"
#include "ADN_Crowds_FireEffectRoe_GUI.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_GroupBox.h"
#include "ADN_SearchListView.h"
#include "ADN_Tr.h"
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <Qt3Support/q3groupbox.h>
#include <QtGui/qdialog.h>
#include <Qt3Support/q3vbox.h>
#include <Qt3Support/q3grid.h>

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_GUI constructor
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
ADN_Crowds_GUI::ADN_Crowds_GUI( ADN_Crowds_Data& data )
    : ADN_GUI_ABC( "ADN_Crowds_GUI" )
    , data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_GUI destructor
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
ADN_Crowds_GUI::~ADN_Crowds_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_GUI::Build
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
void ADN_Crowds_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Global parameters
    Q3GroupBox* pGlobalGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Global parameters" ) );
    Q3GroupBox* pReloadingEffectGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd effects on units firing capability" ), pGlobalGroup );
    // Density
    builder.AddField< ADN_EditLine_Double >( pReloadingEffectGroup, tr( "Density" ), vInfosConnectors[ eReloadingEffectDensity ], tr( "people/m²" ), eGreaterZero );
    vInfosConnectors[ eReloadingEffectDensity ]->Connect( &data_.reloadingSpeedEffectInfos_.rDensity_ );
    // Modifier
    builder.AddField< ADN_EditLine_Double >( pReloadingEffectGroup, tr( "Modifier" ), vInfosConnectors[ eReloadingEffectModifier ], 0, eGreaterZero );
    vInfosConnectors[ eReloadingEffectModifier ]->Connect( &data_.reloadingSpeedEffectInfos_.rModifier_ );
    //Time between NBC application
    builder.AddField< ADN_TimeField >( pGlobalGroup, tr( "Time between two NBC applications" ), vInfosConnectors[ eTimeBetweenNBCApplication ] );
    vInfosConnectors[ eTimeBetweenNBCApplication ]->Connect( &data_.timeBetweenNbcApplication_ );

    // Population parameters
    Q3GroupBox* pPropertiesGroup = new Q3GroupBox( 5, Qt::Vertical, tr( "Details" ) );
    //{
        // Information
        QWidget* pInfoHolder = builder.AddFieldHolder( pPropertiesGroup );
        ADN_EditLine_ABC* nameField = builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vInfosConnectors[ eName ] );
        nameField->ConnectWithRefValidity( data_.GetCrowds() );
        ADN_GoToButton* goToButton = new ADN_GoToButton( eModels, eEntityType_Population );
        goToButton->SetLinkedCombo( builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, tr( "Behavior model" ), vInfosConnectors[ eModel ], 0, eNone, goToButton ) );
        // Density
        Q3GroupBox* pDensity = new Q3GroupBox( 3, Qt::Horizontal, "", pPropertiesGroup );
        builder.AddField< ADN_EditLine_Double >( pDensity, tr( "Density" ), vInfosConnectors[ eConcentrationDensity ], tr( "people/m²" ), eGreaterZero );
        builder.AddField< ADN_EditLine_Double >( pDensity, tr( "Density while moving" ), vInfosConnectors[ eMoveDensity ], tr( "people/m²" ), eGreaterZero );
        builder.AddField< ADN_EditLine_Double >( pDensity, tr( "Average movement speed" ), vInfosConnectors[ eMoveSpeed ], tr( "km/h" ), eGreaterZero );

        // Armed
        Q3GroupBox* pArmed = new Q3GroupBox( 3, Qt::Horizontal, "", pPropertiesGroup );
        builder.AddField< ADN_EditLine_Int >( pArmed, tr( "Armed individuals" ), vInfosConnectors[ eArmedIndividuals ], tr( "%" ), ePercentage );

        // Human
        Q3GroupBox* pHuman = new Q3GroupBox( 3, Qt::Horizontal, "", pPropertiesGroup );
        ADN_MultiPercentage_Double* pMultiPercentage = new ADN_MultiPercentage_Double( pHuman, builder, strClassName_ + "_HumanRepartition" );
        pMultiPercentage->AddLine( tr( "Males" ), vInfosConnectors[ eMale ] );
        pMultiPercentage->AddLine( tr( "Females" ), vInfosConnectors[ eFemale ] );
        pMultiPercentage->AddLine( tr( "Children" ), vInfosConnectors[ eChildren ] );
        pMultiPercentage->AddWarning();

        // NBC
        Q3GroupBox* pNBC = new Q3GroupBox( 3, Qt::Horizontal, "", pPropertiesGroup );
        builder.AddField< ADN_TimeField >( pNBC, tr( "Decontamination delay" ), vInfosConnectors[ eDecontaminationDelay ] );
    //}

    // Speed effects
    Q3GroupBox* pSpeedEffectGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd effects on units movement capability" ) );
    pSpeedEffectGroup->setFixedHeight( 200 );
    ADN_Crowds_SpeedEffect_Attitude_ListView* pSpeedEffectAttitudeList = new ADN_Crowds_SpeedEffect_Attitude_ListView( pSpeedEffectGroup );
    pSpeedEffectAttitudeList->setObjectName( strClassName_ + "_AttitudeList" );
    vInfosConnectors[ eSpeedEffectAttitude ] = &pSpeedEffectAttitudeList->GetConnector();
    ADN_Crowds_SpeedEffect_Volume_ListView* pVolumeList = new ADN_Crowds_SpeedEffect_Volume_ListView( pSpeedEffectGroup );
    pVolumeList->setObjectName( strClassName_ + "_VolumeList" );
    vInfosConnectors[ eSpeedEffectVolume ] = &pVolumeList->GetConnector();
    Q3GroupBox* pSpeedEffectVolumeGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Effect" ), pSpeedEffectGroup );
    builder.AddField< ADN_EditLine_Double >( pSpeedEffectVolumeGroup, tr( "Density" ), vInfosConnectors[eSpeedEffectDensity], tr( "people/m²" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pSpeedEffectVolumeGroup, tr( "Max speed" ), vInfosConnectors[eSpeedEffectMaxSpeed], tr( "km/h" ), eGreaterEqualZero );

    // Fire effects
    Q3GroupBox* pFireEffectGlobalGroup = new Q3GroupBox( 0, Qt::Horizontal, tr( "Attritions" ) );
    //{
        Q3GroupBox* pFireEffectGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd -> Units" ), pFireEffectGlobalGroup );
        ADN_Crowds_FireEffect_Attitude_ListView* pFireEffectAttitudeList = new ADN_Crowds_FireEffect_Attitude_ListView( pFireEffectGroup );
        pFireEffectAttitudeList->setObjectName( strClassName_ + "_FireAttitudeList" );
        vInfosConnectors[ eFireEffectAttitude ] = &pFireEffectAttitudeList->GetConnector();
        Q3VBox* pFireEffectProtectionBox = new Q3VBox( pFireEffectGroup );
        pFireEffectProtectionBox->setSpacing( 5 );
        // Protection
        ADN_Crowds_FireEffect_Protection_ListView* pProtectionList = new ADN_Crowds_FireEffect_Protection_ListView( pFireEffectProtectionBox );
        pProtectionList->setObjectName( strClassName_ + "_ProtectionList" );
        vInfosConnectors[ eFireEffectProtection ] = &pProtectionList->GetConnector();
        // Intensity
        Q3GroupBox* pFireEffectIntensityGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Intensity" ), pFireEffectProtectionBox );
        builder.AddField< ADN_EditLine_Double >( pFireEffectIntensityGroup, tr( "Density" ), vInfosConnectors[ eFireEffectIntensityDensity ], tr( "people/m²" ), eGreaterEqualZero );
        builder.AddField< ADN_EditLine_Double >( pFireEffectIntensityGroup, tr( "Intensity" ), vInfosConnectors[ eFireEffectIntensityFactor ], 0, eGreaterEqualZero );

        // Effects
        Q3GroupBox* pFireEffectProtectionGroup = new Q3GroupBox( 2, Qt::Vertical, tr( "Effect" ), pFireEffectGroup );

        Q3GroupBox* pFireEffectUnarmedProtectionGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Unarmed" ), pFireEffectProtectionGroup );
        ADN_MultiPercentage_Double* pMultiPercentageUnarmed = new ADN_MultiPercentage_Double( pFireEffectUnarmedProtectionGroup, builder, strClassName_ + "_Unarmed_Effects" );
        pMultiPercentageUnarmed->AddLine( tr( "Destruction" ), vInfosConnectors[ eFireEffectUnarmedDestruction ] );
        pMultiPercentageUnarmed->AddLine( tr( "Fixable with evacuation" ), vInfosConnectors[ eFireEffectUnarmedFixableWithEvacuation ] );
        pMultiPercentageUnarmed->AddLine( tr( "Fixable without evacuation" ), vInfosConnectors[ eFireEffectUnarmedFixableWithoutEvacuation ] );

        Q3GroupBox* pFireEffectArmedProtectionGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Armed" ), pFireEffectProtectionGroup );
        ADN_MultiPercentage_Double* pMultiPercentageArmed = new ADN_MultiPercentage_Double( pFireEffectArmedProtectionGroup, builder, strClassName_ + "_Armed_Effects" );
        pMultiPercentageArmed->AddLine( tr( "Destruction" ), vInfosConnectors[ eFireEffectArmedDestruction ] );
        pMultiPercentageArmed->AddLine( tr( "Fixable with evacuation" ), vInfosConnectors[ eFireEffectArmedFixableWithEvacuation ] );
        pMultiPercentageArmed->AddLine( tr( "Fixable without evacuation" ), vInfosConnectors[ eFireEffectArmedFixableWithoutEvacuation ] );

        // Fire Roe effects
        Q3GroupBox* pFireEffectRoeGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Units -> Crowd" ), pFireEffectGlobalGroup );
        new ADN_Crowds_FireEffectRoe_GUI( strClassName_ + "_FireEffectRoe", vInfosConnectors[ eFireEffectRoe ], pFireEffectRoeGroup );
    //}

    // Urban area destruction
    Q3GroupBox* pUrbanAreaDestructionlGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Urban area destruction" ) );
    pUrbanAreaDestructionlGroup->setFixedHeight( 200 );
    ADN_Crowds_UrbanEffect_Attitude_ListView* pUrbanEffectAttitudeList = new ADN_Crowds_UrbanEffect_Attitude_ListView( pUrbanAreaDestructionlGroup );
    pUrbanEffectAttitudeList->setObjectName( strClassName_+ "_UrbanDestructionList" );
    vInfosConnectors[ eUrbanBlocDestructionAttitude ] = &pUrbanEffectAttitudeList->GetConnector();
    Q3GroupBox* pTimeDestructUrbanGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Time to destruct a urban block" ), pUrbanAreaDestructionlGroup );
    builder.AddField< ADN_EditLine_Double >( pTimeDestructUrbanGroup, tr( "Density" ), vInfosConnectors[ eUrbanBlocDestructionDensity ], tr( "people/m²" ), eGreaterEqualZero );
    builder.AddField< ADN_TimeField >( pTimeDestructUrbanGroup, tr( "Time" ), vInfosConnectors[ eUrbanBlocDestructionTime ] );

    // Connectors
    pSpeedEffectAttitudeList->SetItemConnectors( vInfosConnectors );
    pVolumeList->SetItemConnectors( vInfosConnectors );
    pFireEffectAttitudeList->SetItemConnectors( vInfosConnectors );
    pProtectionList->SetItemConnectors( vInfosConnectors );
    pUrbanEffectAttitudeList->SetItemConnectors( vInfosConnectors );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Fire effect layouts
    Q3GridLayout* pFireEffectLayout = new Q3GridLayout( pFireEffectGlobalGroup->layout(), 1, 4, 5 );
    pFireEffectLayout->setColStretch( 0, 3 );
    pFireEffectLayout->setColStretch( 3, 1 );
    pFireEffectLayout->addWidget( pFireEffectGroup   , 0, 0 );
    pFireEffectLayout->addWidget( pFireEffectRoeGroup, 0, 3 );

    // Specific layout
    QWidget* pSpecificContent = new QWidget();
    QVBoxLayout* pSpecificLayout = new QVBoxLayout( pSpecificContent );
    pSpecificLayout->setMargin( 10 );
    pSpecificLayout->setSpacing( 10 );
    pSpecificLayout->setAlignment( Qt::AlignTop );
    pSpecificLayout->addWidget( pPropertiesGroup );
    pSpecificLayout->addWidget( pSpeedEffectGroup );
    pSpecificLayout->addWidget( pFireEffectGlobalGroup );
    pSpecificLayout->addWidget( pUrbanAreaDestructionlGroup );

    // List view
    ADN_SearchListView< ADN_Crowds_ListView >* pSearchListView = new ADN_SearchListView< ADN_Crowds_ListView >( this, data_.GetCrowds(), vInfosConnectors );
    pListView_ = pSearchListView->GetListView();
    pListView_->setObjectName( strClassName_ + "_List" );

    // Sub content
    QWidget* pSubContent = CreateScrollArea( *pSpecificContent, pSearchListView );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pGlobalGroup );
    pContentLayout->addWidget( pSubContent, 1 );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent );
}
