// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Population_GUI.h"
#include "ADN_MainWindow.h"
#include "ADN_App.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_Population_Data.h"
#include "ADN_Connector_Vector_ABC.h"
#include "ADN_MultiPercentage.h"
#include "ADN_Population_ListView.h"
#include "ADN_Population_SpeedEffect_Attitude_ListView.h"
#include "ADN_Population_SpeedEffect_Volume_ListView.h"
#include "ADN_Population_FireEffect_Attitude_ListView.h"
#include "ADN_Population_FireEffect_Protection_ListView.h"
#include "ADN_Population_UrbanEffect_Attitude_ListView.h"
#include "ADN_Population_FireEffectRoe_GUI.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_GroupBox.h"
#include "ADN_Tr.h"
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <Qt3Support/q3groupbox.h>
#include <QtGui/qdialog.h>
#include <Qt3Support/q3vbox.h>
#include <Qt3Support/q3grid.h>
//Added by qt3to4:
#include <Qt3Support/q3gridlayout.h>

// -----------------------------------------------------------------------------
// Name: ADN_Population_GUI constructor
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
ADN_Population_GUI::ADN_Population_GUI( ADN_Population_Data& data )
    : ADN_GUI_ABC( "ADN_Population_GUI" )
    , data_( data )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_GUI destructor
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
ADN_Population_GUI::~ADN_Population_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_GUI::Build
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
void ADN_Population_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0, "Crowd main widget" );

    // Create the population listview.
    pPopulationList_ = new ADN_Population_ListView( pMainWidget_ );
    pPopulationList_->GetConnector().Connect( &data_.GetPopulation() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    Q3VBox* pMainBox = new Q3VBox( pMainWidget_ );

    // Global parameters
    Q3GroupBox* pGlobalGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Global parameters" ), pMainBox );
    Q3GroupBox* pReloadingEffectGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd effects on units firing capability" ), pGlobalGroup );
    Q3GroupBox* pNBC = new Q3GroupBox( 3, Qt::Horizontal, tr( "NBC" ), pGlobalGroup );

    //Time between NBC application
    builder.AddField< ADN_TimeField >( pNBC, tr( "Time between two NBC applications" ), vInfosConnectors[ eTimeBetweenNBCApplication ] );
    vInfosConnectors[eTimeBetweenNBCApplication]->Connect( &data_.timeBetweenNbcApplication_ );

    // Decontamination delay
    builder.AddField< ADN_TimeField >( pNBC, tr( "Decontamination delay" ), vInfosConnectors[ eDecontaminationDelay ] );
    vInfosConnectors[eDecontaminationDelay]->Connect( &data_.decontaminationDelay_ );

    // Density
    builder.AddField<ADN_EditLine_Double>( pReloadingEffectGroup, tr( "Density" ), vInfosConnectors[eReloadingEffectDensity], tr( "people/m²" ), eGreaterZero );
    vInfosConnectors[eReloadingEffectDensity]->Connect( &data_.reloadingSpeedEffectInfos_.rDensity_ );

    // Modifier
    builder.AddField<ADN_EditLine_Double>( pReloadingEffectGroup, tr( "Modifier" ), vInfosConnectors[eReloadingEffectModifier], 0, eGreaterZero );
    vInfosConnectors[eReloadingEffectModifier]->Connect( &data_.reloadingSpeedEffectInfos_.rModifier_ );

    // Population parameters
    Q3GroupBox* pGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Crowd" ), pMainBox );

    Q3GroupBox* pPropertiesGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Details" ), pGroup );

    // Name
    builder.AddField<ADN_EditLine_String>( pPropertiesGroup, tr( "Name" ), vInfosConnectors[eName] );

    // Model
    builder.AddField< ADN_ComboBox_Vector<ADN_Models_Data::ModelInfos> >( pPropertiesGroup, tr( "Behavior model" ), vInfosConnectors[eModel] );

    // Concentration density
    builder.AddField<ADN_EditLine_Double>( pPropertiesGroup, tr( "Density" ), vInfosConnectors[eConcentrationDensity], tr( "people/m²" ), eGreaterZero );

    // Move nominal density
    builder.AddField<ADN_EditLine_Double>( pPropertiesGroup, tr( "Density while moving" ), vInfosConnectors[eMoveDensity], tr( "people/m²" ), eGreaterZero );

    // Move speed
    builder.AddField<ADN_EditLine_Double>( pPropertiesGroup, tr( "Average movement speed" ), vInfosConnectors[eMoveSpeed], tr( "km/h" ), eGreaterZero );

    //Armed Individuals
    builder.AddField< ADN_EditLine_Int >( pPropertiesGroup, tr( "Armed individuals" ), vInfosConnectors[ eArmedIndividuals ], tr( "%" ), ePercentage );

    //Repartition
    ADN_MultiPercentage* pMultiPercentage = new ADN_MultiPercentage( pPropertiesGroup, builder );
    pMultiPercentage->AddLine( tr( "Males" ), vInfosConnectors[ eMale ] );
    pMultiPercentage->AddLine( tr( "Females" ), vInfosConnectors[ eFemale ] );
    pMultiPercentage->AddLine( tr( "Children" ), vInfosConnectors[ eChildren ] );
    pMultiPercentage->AddWarning();

    // Speed effects
    //@{
    Q3GroupBox* pSpeedEffectGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd effects on units movement capability" ), pGroup );

    ADN_Population_SpeedEffect_Attitude_ListView* pSpeedEffectAttitudeList = new ADN_Population_SpeedEffect_Attitude_ListView( pSpeedEffectGroup );
    vInfosConnectors[eSpeedEffectAttitude] = &pSpeedEffectAttitudeList->GetConnector();

    ADN_Population_SpeedEffect_Volume_ListView* pVolumeList = new ADN_Population_SpeedEffect_Volume_ListView( pSpeedEffectGroup );
    vInfosConnectors[eSpeedEffectVolume] = &pVolumeList->GetConnector();

    Q3GroupBox* pSpeedEffectVolumeGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Effect" ), pSpeedEffectGroup );

    builder.AddField< ADN_EditLine_Double >( pSpeedEffectVolumeGroup, tr( "Density" ), vInfosConnectors[eSpeedEffectDensity], tr( "people/m²" ), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pSpeedEffectVolumeGroup, tr( "Max speed" ), vInfosConnectors[eSpeedEffectMaxSpeed], tr( "km/h" ), eGreaterEqualZero );
    //@}

    // Fire effects
    //@{
    Q3GroupBox* pFireEffectGlobalGroup = new Q3GroupBox( 0, Qt::Horizontal, tr( "Attritions" ), pGroup );

    Q3GroupBox* pFireEffectGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd -> Units" ), pFireEffectGlobalGroup );
    ADN_Population_FireEffect_Attitude_ListView* pFireEffectAttitudeList = new ADN_Population_FireEffect_Attitude_ListView( pFireEffectGroup );
    vInfosConnectors[eFireEffectAttitude] = &pFireEffectAttitudeList->GetConnector();

    Q3VBox* pFireEffectProtectionBox = new Q3VBox( pFireEffectGroup );
    pFireEffectProtectionBox->setSpacing( 5 );

    // Protection
    ADN_Population_FireEffect_Protection_ListView* pProtectionList = new ADN_Population_FireEffect_Protection_ListView( pFireEffectProtectionBox );
    vInfosConnectors[eFireEffectProtection] = &pProtectionList->GetConnector();

    // Intensity
    Q3GroupBox* pFireEffectIntensityGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Intensity" ), pFireEffectProtectionBox );
    builder.AddField< ADN_EditLine_Double >( pFireEffectIntensityGroup, tr( "Density" ), vInfosConnectors[eFireEffectIntensityDensity], tr( "people/m²" ), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pFireEffectIntensityGroup, tr( "Intensity" ), vInfosConnectors[eFireEffectIntensityFactor], 0, eGreaterEqualZero );

    // Effects
    Q3GroupBox* pFireEffectProtectionGroup = new Q3GroupBox( 2, Qt::Vertical, tr( "Effect" ), pFireEffectGroup );

    Q3GroupBox* pFireEffectUnarmedProtectionGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Unarmed" ), pFireEffectProtectionGroup );
    ADN_EditLine_Double* pDestruField       = builder.AddField< ADN_EditLine_Double >( pFireEffectUnarmedProtectionGroup, tr( "Destruction" ), vInfosConnectors[eFireEffectUnarmedDestruction], tr( "%" ) );
    ADN_EditLine_Double* pWithEvacField     = builder.AddField< ADN_EditLine_Double >( pFireEffectUnarmedProtectionGroup, tr( "Fixable with evacuation" ), vInfosConnectors[eFireEffectUnarmedFixableWithEvacuation], tr( "%" ) );
    ADN_EditLine_Double* pWithoutEvacField  = builder.AddField< ADN_EditLine_Double >( pFireEffectUnarmedProtectionGroup, tr( "Fixable without evacuation" ), vInfosConnectors[eFireEffectUnarmedFixableWithoutEvacuation], tr( "%" ) );

    pValidatorDestruUnarmed_    = new ADN_PercentageValidator( pDestruField );
    pValidatorEvacUnarmed_      = new ADN_PercentageValidator( pWithEvacField );
    pValidatorWithoutUnarmed_   = new ADN_PercentageValidator( pWithoutEvacField );

    pDestruField->setValidator( pValidatorDestruUnarmed_ );
    pWithEvacField->setValidator( pValidatorEvacUnarmed_ );
    pWithoutEvacField->setValidator( pValidatorWithoutUnarmed_ );

    Q3GroupBox* pFireEffectArmedProtectionGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Armed" ), pFireEffectProtectionGroup );
    pDestruField       = builder.AddField< ADN_EditLine_Double >( pFireEffectArmedProtectionGroup, tr( "Destruction" ), vInfosConnectors[eFireEffectArmedDestruction], tr( "%" ) );
    pWithEvacField     = builder.AddField< ADN_EditLine_Double >( pFireEffectArmedProtectionGroup, tr( "Fixable with evacuation" ), vInfosConnectors[eFireEffectArmedFixableWithEvacuation], tr( "%" ) );
    pWithoutEvacField  = builder.AddField< ADN_EditLine_Double >( pFireEffectArmedProtectionGroup, tr( "Fixable without evacuation" ), vInfosConnectors[eFireEffectArmedFixableWithoutEvacuation], tr( "%" ) );

    pValidatorDestruArmed_  = new ADN_PercentageValidator( pDestruField );
    pValidatorEvacArmed_    = new ADN_PercentageValidator( pWithEvacField );
    pValidatorWithoutArmed_ = new ADN_PercentageValidator( pWithoutEvacField );

    pDestruField->setValidator( pValidatorDestruArmed_ );
    pWithEvacField->setValidator( pValidatorEvacArmed_ );
    pWithoutEvacField->setValidator( pValidatorWithoutArmed_ );
    //@}

    // Fire Roe effects
    //@{
    Q3GroupBox* pFireEffectRoeGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Units -> Crowd" ), pFireEffectGlobalGroup );
    ADN_Population_FireEffectRoe_GUI* pFireEffectRoe = new ADN_Population_FireEffectRoe_GUI( pFireEffectRoeGroup );
    vInfosConnectors[eFireEffectRoe] = &pFireEffectRoe->GetConnector();
    //@}

    // Urban area destruction
    //@{
    Q3GroupBox* pUrbanAreaDestructionlGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Urban area destruction" ), pGroup );
    ADN_Population_UrbanEffect_Attitude_ListView* pUrbanEffectAttitudeList = new ADN_Population_UrbanEffect_Attitude_ListView( pUrbanAreaDestructionlGroup );
    vInfosConnectors[eUrbanBlocDestructionAttitude] = &pUrbanEffectAttitudeList->GetConnector();

    Q3GroupBox* pTimeDestructUrbanGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Time to destruct a urban block" ), pUrbanAreaDestructionlGroup );
    builder.AddField< ADN_EditLine_Double >( pTimeDestructUrbanGroup, tr( "Density" ), vInfosConnectors[eUrbanBlocDestructionDensity], tr( "people/m²" ), eGreaterEqualZero );
    builder.AddField< ADN_TimeField >( pTimeDestructUrbanGroup, tr( "Time" ), vInfosConnectors[ eUrbanBlocDestructionTime ] );
    //@}

    builder.AddStretcher( pGroup, Qt::Vertical );

    pPopulationList_->SetItemConnectors( vInfosConnectors );
    pSpeedEffectAttitudeList->SetItemConnectors( vInfosConnectors );
    pVolumeList->SetItemConnectors( vInfosConnectors );
    pFireEffectAttitudeList->SetItemConnectors( vInfosConnectors );
    pProtectionList->SetItemConnectors( vInfosConnectors );
    pUrbanEffectAttitudeList->SetItemConnectors( vInfosConnectors );

    // Layout
    Q3HBoxLayout* pMainLayout = new Q3HBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pPopulationList_, 1 );
    pMainLayout->addWidget( pMainBox, 3 );

    Q3GridLayout* pFireEffectLayout = new Q3GridLayout( pFireEffectGlobalGroup->layout(), 1, 4, 5 );
    pFireEffectLayout->addWidget( pFireEffectGroup   , 0, 0 );
    pFireEffectLayout->addWidget( pFireEffectRoeGroup, 0, 3 );
    pFireEffectLayout->setColStretch( 0, 3 );
    pFireEffectLayout->setColStretch( 3, 1 );
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_GUI::UpdatePopulationFireEffectValidator
// Created: MMC 2011-03-29
// -----------------------------------------------------------------------------
void ADN_Population_GUI::UpdatePopulationFireEffectValidator( ADN_Type_Double& destruUnarmed, ADN_Type_Double& evacUnarmed, ADN_Type_Double& withoutEvacUnarmed, ADN_Type_Double& destruArmed, ADN_Type_Double& evacArmed, ADN_Type_Double& withoutEvacArmed )
{
    pValidatorDestruUnarmed_->ClearLinkedValues();
    pValidatorDestruUnarmed_->AddLinkedValue( evacUnarmed );
    pValidatorDestruUnarmed_->AddLinkedValue( withoutEvacUnarmed );

    pValidatorEvacUnarmed_->ClearLinkedValues();
    pValidatorEvacUnarmed_->AddLinkedValue( destruUnarmed );
    pValidatorEvacUnarmed_->AddLinkedValue( withoutEvacUnarmed );

    pValidatorWithoutUnarmed_->ClearLinkedValues();
    pValidatorWithoutUnarmed_->AddLinkedValue( destruUnarmed );
    pValidatorWithoutUnarmed_->AddLinkedValue( evacUnarmed );

    pValidatorDestruArmed_->ClearLinkedValues();
    pValidatorDestruArmed_->AddLinkedValue( evacArmed );
    pValidatorDestruArmed_->AddLinkedValue( withoutEvacArmed );

    pValidatorEvacArmed_->ClearLinkedValues();
    pValidatorEvacArmed_->AddLinkedValue( destruArmed );
    pValidatorEvacArmed_->AddLinkedValue( withoutEvacArmed );

    pValidatorWithoutArmed_->ClearLinkedValues();
    pValidatorWithoutArmed_->AddLinkedValue( destruArmed );
    pValidatorWithoutArmed_->AddLinkedValue( evacArmed );
}