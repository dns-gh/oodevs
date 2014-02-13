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
#include "moc_ADN_Crowds_GUI.cpp"
#include "ADN_GuiBuilder.h"
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
    : ADN_GUI_ABC( eCrowds )
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
    builder.PushSubName( "global" );
    Q3GroupBox* pReloadingEffectGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd effects on units firing capability" ), pGlobalGroup );
    // Density
    builder.AddField< ADN_EditLine_Double >( pReloadingEffectGroup, "density", tr( "Density" ), vInfosConnectors[ eReloadingEffectDensity ], tr( "people/m²" ), eGreaterZero );
    vInfosConnectors[ eReloadingEffectDensity ]->Connect( &data_.reloadingSpeedEffectInfos_.rDensity_ );
    // Modifier
    builder.AddField< ADN_EditLine_Double >( pReloadingEffectGroup, "modifier", tr( "Modifier" ), vInfosConnectors[ eReloadingEffectModifier ], 0, eGreaterZero );
    vInfosConnectors[ eReloadingEffectModifier ]->Connect( &data_.reloadingSpeedEffectInfos_.rModifier_ );
    //Time between NBC application
    builder.AddField< ADN_TimeField >( pGlobalGroup, "time-between-two-nbc", tr( "Time between two NBC applications" ), vInfosConnectors[ eTimeBetweenNBCApplication ] );
    vInfosConnectors[ eTimeBetweenNBCApplication ]->Connect( &data_.timeBetweenNbcApplication_ );

    builder.PopSubName(); //! global

    // Population parameters
    Q3GroupBox* pPropertiesGroup = new Q3GroupBox( 5, Qt::Vertical, tr( "Details" ) );
    //{
        // Information
        QWidget* pInfoHolder = builder.AddFieldHolder( pPropertiesGroup );
        builder.AddLocalizedField( data_.GetCrowds(), pInfoHolder, "name", tr( "Name" ), vInfosConnectors[ eName ] );
        ADN_GoToButton* goToButton = new ADN_GoToButton( eModels, eEntityType_Population );
        goToButton->SetLinkedCombo( builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, "behavior-model", tr( "Behavior model" ), vInfosConnectors[ eModel ], 0, eNone, goToButton ) );
        builder.AddField< ADN_CheckBox >( pInfoHolder, "crowds-collide", tr( "Collides with other crowds" ), vInfosConnectors[ eCrowdCollision ] );

        // Density
        Q3GroupBox* pDensity = new Q3GroupBox( 3, Qt::Horizontal, pPropertiesGroup );
        builder.AddField< ADN_EditLine_Double >( pDensity, "density", tr( "Density" ), vInfosConnectors[ eConcentrationDensity ], tr( "people/m²" ), eGreaterZero );
        builder.AddField< ADN_EditLine_Double >( pDensity, "density-while-moving", tr( "Density while moving" ), vInfosConnectors[ eMoveDensity ], tr( "people/m²" ), eGreaterZero );
        builder.AddField< ADN_EditLine_Double >( pDensity, "average-movement-speed", tr( "Average movement speed" ), vInfosConnectors[ eMoveSpeed ], tr( "km/h" ), eGreaterZero );

        // Armed
        Q3GroupBox* pArmed = new Q3GroupBox( 3, Qt::Horizontal, "", pPropertiesGroup );
        builder.AddField< ADN_EditLine_Int >( pArmed, "armed-individuals", tr( "Armed individuals" ), vInfosConnectors[ eArmedIndividuals ], tr( "%" ), ePercentage );

        // Human
        Q3GroupBox* pHuman = new Q3GroupBox( 3, Qt::Horizontal, "", pPropertiesGroup );
        ADN_MultiPercentage_Double* pMultiPercentage = new ADN_MultiPercentage_Double( pHuman, builder, builder.GetChildName( "human-repartition" ) );
        pMultiPercentage->AddLine( tr( "Males" ), vInfosConnectors[ eMale ], "males" );
        pMultiPercentage->AddLine( tr( "Females" ), vInfosConnectors[ eFemale ], "females" );
        pMultiPercentage->AddLine( tr( "Children" ), vInfosConnectors[ eChildren ], "children" );
        pMultiPercentage->AddWarning();

        // NBC
        Q3GroupBox* pNBC = new Q3GroupBox( 3, Qt::Horizontal, "", pPropertiesGroup );
        builder.AddField< ADN_TimeField >( pNBC, "decontamination-delay", tr( "Decontamination delay" ), vInfosConnectors[ eDecontaminationDelay ] );
    //}

    // Speed effects
    Q3GroupBox* pSpeedEffectGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd effects on units movement capability" ) );
    pSpeedEffectGroup->setFixedHeight( 200 );
    ADN_Crowds_SpeedEffect_Attitude_ListView* pSpeedEffectAttitudeList = builder.AddWidget< ADN_Crowds_SpeedEffect_Attitude_ListView >( "attitude-list", pSpeedEffectGroup );
    vInfosConnectors[ eSpeedEffectAttitude ] = &pSpeedEffectAttitudeList->GetConnector();
    ADN_Crowds_SpeedEffect_Volume_ListView* pVolumeList = builder.AddWidget< ADN_Crowds_SpeedEffect_Volume_ListView >( "volume-list", pSpeedEffectGroup );
    vInfosConnectors[ eSpeedEffectVolume ] = &pVolumeList->GetConnector();
    Q3GroupBox* pSpeedEffectVolumeGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Effect" ), pSpeedEffectGroup );
    builder.AddField< ADN_EditLine_Double >( pSpeedEffectVolumeGroup, "density", tr( "Density" ), vInfosConnectors[eSpeedEffectDensity], tr( "people/m²" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pSpeedEffectVolumeGroup, "max-speed", tr( "Max speed" ), vInfosConnectors[eSpeedEffectMaxSpeed], tr( "km/h" ), eGreaterEqualZero );

    // Fire effects
    Q3GroupBox* pFireEffectGlobalGroup = new Q3GroupBox( 0, Qt::Horizontal, tr( "Attritions" ) );
    //{
        builder.PushSubName( "attritions" );
        Q3GroupBox* pFireEffectGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd -> Units" ), pFireEffectGlobalGroup );
        ADN_Crowds_FireEffect_Attitude_ListView* pFireEffectAttitudeList = builder.AddWidget< ADN_Crowds_FireEffect_Attitude_ListView >( "fire-attitude-list", pFireEffectGroup );
        vInfosConnectors[ eFireEffectAttitude ] = &pFireEffectAttitudeList->GetConnector();
        Q3VBox* pFireEffectProtectionBox = new Q3VBox( pFireEffectGroup );
        pFireEffectProtectionBox->setSpacing( 5 );
        // Protection
        ADN_Crowds_FireEffect_Protection_ListView* pProtectionList = builder.AddWidget< ADN_Crowds_FireEffect_Protection_ListView >( "protection-list", pFireEffectProtectionBox );
        vInfosConnectors[ eFireEffectProtection ] = &pProtectionList->GetConnector();
        // Intensity
        Q3GroupBox* pFireEffectIntensityGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Intensity" ), pFireEffectProtectionBox );
        builder.AddField< ADN_EditLine_Double >( pFireEffectIntensityGroup, "density", tr( "Density" ), vInfosConnectors[ eFireEffectIntensityDensity ], tr( "people/m²" ), eGreaterEqualZero );
        builder.AddField< ADN_EditLine_Double >( pFireEffectIntensityGroup, "intensity", tr( "Intensity" ), vInfosConnectors[ eFireEffectIntensityFactor ], 0, eGreaterEqualZero );

        // Effects
        Q3GroupBox* pFireEffectProtectionGroup = new Q3GroupBox( 2, Qt::Vertical, tr( "Effect" ), pFireEffectGroup );

        Q3GroupBox* pFireEffectUnarmedProtectionGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Unarmed" ), pFireEffectProtectionGroup );
        ADN_MultiPercentage_Double* pMultiPercentageUnarmed = new ADN_MultiPercentage_Double( pFireEffectUnarmedProtectionGroup, builder, builder.GetChildName( "unarmed-effects" ) );
        pMultiPercentageUnarmed->AddLine( tr( "Destruction" ), vInfosConnectors[ eFireEffectUnarmedDestruction ], "destruction" );
        pMultiPercentageUnarmed->AddLine( tr( "Fixable with evacuation" ), vInfosConnectors[ eFireEffectUnarmedFixableWithEvacuation ], "fixable-with-evacuation" );
        pMultiPercentageUnarmed->AddLine( tr( "Fixable without evacuation" ), vInfosConnectors[ eFireEffectUnarmedFixableWithoutEvacuation ], "fixable-without-evacuation" );

        Q3GroupBox* pFireEffectArmedProtectionGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Armed" ), pFireEffectProtectionGroup );
        ADN_MultiPercentage_Double* pMultiPercentageArmed = new ADN_MultiPercentage_Double( pFireEffectArmedProtectionGroup, builder, builder.GetChildName( "armed-effects" ) );
        pMultiPercentageArmed->AddLine( tr( "Destruction" ), vInfosConnectors[ eFireEffectArmedDestruction ], "destruction" );
        pMultiPercentageArmed->AddLine( tr( "Fixable with evacuation" ), vInfosConnectors[ eFireEffectArmedFixableWithEvacuation ], "fixable-without-evacuation" );
        pMultiPercentageArmed->AddLine( tr( "Fixable without evacuation" ), vInfosConnectors[ eFireEffectArmedFixableWithoutEvacuation ], "fixable-without-evacuation" );

        // Fire Roe effects
        Q3GroupBox* pFireEffectRoeGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Units -> Crowd" ), pFireEffectGlobalGroup );
        new ADN_Crowds_FireEffectRoe_GUI( builder.GetChildName( "fire-effects-roe-table" ), vInfosConnectors[ eFireEffectRoe ], pFireEffectRoeGroup );

        builder.PopSubName(); //! attritions
    //}

    // Urban area destruction
    builder.PushSubName( "urban-area-destruction" );
    Q3GroupBox* pUrbanAreaDestructionlGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Urban area destruction" ) );
    pUrbanAreaDestructionlGroup->setFixedHeight( 200 );
    ADN_Crowds_UrbanEffect_Attitude_ListView* pUrbanEffectAttitudeList = builder.AddWidget< ADN_Crowds_UrbanEffect_Attitude_ListView >( "urban-destruction-list", pUrbanAreaDestructionlGroup );
    vInfosConnectors[ eUrbanBlocDestructionAttitude ] = &pUrbanEffectAttitudeList->GetConnector();
    Q3GroupBox* pTimeDestructUrbanGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Time to destruct a urban block" ), pUrbanAreaDestructionlGroup );
    builder.AddField< ADN_EditLine_Double >( pTimeDestructUrbanGroup, "density", tr( "Density" ), vInfosConnectors[ eUrbanBlocDestructionDensity ], tr( "people/m²" ), eGreaterEqualZero );
    builder.AddField< ADN_TimeField >( pTimeDestructUrbanGroup, "time", tr( "Time" ), vInfosConnectors[ eUrbanBlocDestructionTime ] );
    builder.PopSubName(); //! urban-area-destruction

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
    QWidget* pSearchListView = builder.AddSearchListView< ADN_Crowds_ListView >( this, data_.GetCrowds(), vInfosConnectors );

    // Sub content
    QWidget* pSubContent = CreateScrollArea( builder.GetChildName( "content" ), *pSpecificContent, pSearchListView );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pGlobalGroup );
    pContentLayout->addWidget( pSubContent, 1 );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent );
}
