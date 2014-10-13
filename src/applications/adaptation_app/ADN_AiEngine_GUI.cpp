// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_AiEngine_GUI.h"
#include "moc_ADN_AiEngine_GUI.cpp"
#include "ADN_Workspace.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_AiEngine_UrbanSpeeds_Table.h"
#include "ADN_AiEngine_Perception_Table.h"
#include "ADN_CommonGfx.h"
#include "ADN_Project_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_TimeField.h"

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_GUI constructor
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
ADN_AiEngine_GUI::ADN_AiEngine_GUI( ADN_AiEngine_Data& data )
    : ADN_GUI_ABC( eAiEngine )
    , data_      ( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_GUI destructor
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
ADN_AiEngine_GUI::~ADN_AiEngine_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_GUI::Build
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
void ADN_AiEngine_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );

    // Danger
    Q3GroupBox* pDangerBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Threat management" ) );
    builder.AddField< ADN_EditLine_Double >( pDangerBox, "operational-state", tr( "Operational state maximum effect on threat" ), data_.rOperationalStateMaxDecrease_, tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Double >( pDangerBox, "information-relevance", tr( "Information relevance maximum effect on threat" ), data_.rPertinenceMaxDecrease_, tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Double >( pDangerBox, "neutralization-state", tr( "Neutralization state maximum effect on threat" ), data_.rNeutralizedStateMaxDecrease_, tr( "%" ), ePercentage );

    // Operational state
    Q3GroupBox* pOpStateBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Operational state computation" ) );
    builder.AddField< ADN_EditLine_Double >( pOpStateBox, "non-major-equipments-state-weight", tr( "Non major equipments state weight" ), data_.rMinorEquipmentWeight_, 0, eZeroOne );
    builder.AddField< ADN_EditLine_Double >( pOpStateBox, "major-equipments-state-weight", tr( "Major equipments state weight" ), data_.rMajorEquipmentWeight_, 0, eZeroOne );
    builder.AddField< ADN_EditLine_Double >( pOpStateBox, "crew-state-weight", tr( "Crew state weight" ), data_.rHumanWeight_, 0, eZeroOne );
    builder.AddField< ADN_EditLine_Double >( pOpStateBox, "decisional-threshold", tr( "Decisional threshold" ), data_.rDecisionalThreshold_, 0, eZeroOne );

    // Urban area effect on fire
    Q3GroupBox* pUrbanStateBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Urban combat computation" ) );
    builder.AddField< ADN_EditLine_Double >( pUrbanStateBox, "urban-perception-view-factor", tr( "Urban perception view factor" ), data_.rUrbanViewFactor_, tr( "%" ), ePercentage );
    
    // Critical Intelligence
    Q3GroupBox* pCriticalIntelligenceBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Time to acquire critical intelligence" ) );
    builder.AddField< ADN_TimeField >( pCriticalIntelligenceBox, "minimum-affinity", tr( "Minimum affinity" ), data_.rMinimumAffinity_ );
    builder.AddField< ADN_TimeField >( pCriticalIntelligenceBox, "neutral-affinity", tr( "Neutral affinity" ), data_.rNeutralAffinity_ );
    builder.AddField< ADN_TimeField >( pCriticalIntelligenceBox, "maximum-affinity", tr( "Maximum affinity" ), data_.rMaximumAffinity_ );

    // Wound effects modificators
    Q3GroupBox* pWoundEffectsBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Equipment state on power indicators" ) );
    builder.AddField< ADN_EditLine_Double >( pWoundEffectsBox, "available", tr( "Available" ), data_.rAvailableModificator_, tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Double >( pWoundEffectsBox, "unavailable", tr( "Unavailable" ), data_.rUnavailableModificator_, tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Double >( pWoundEffectsBox, "repairable", tr( "Repairable" ), data_.rRepairableModificator_, tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Double >( pWoundEffectsBox, "repairing", tr( "Repairing" ), data_.rRepairingModificator_, tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Double >( pWoundEffectsBox, "captured", tr( "Captured" ), data_.rCapturedModificator_, tr( "%" ), ePercentage );
    
    // Force ratio
    Q3GroupBox* pForceRatioBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Force ratio feedback time" ) );
    builder.AddField< ADN_TimeField >( pForceRatioBox, "default-force-ratio-feedback-time", tr( "Default force ratio feedback time" ), data_.rDefaultFeedbackTime_ );

    // Search/Recon speeds
    QGroupBox* pSpeedsBox = new gui::RichGroupBox( "speeds-box", tr( "Recon and search speeds" ) );
    QVBoxLayout* pSpeedsLayout = new QVBoxLayout( pSpeedsBox );

    Q3GroupBox* pOpenSpeeds = new Q3GroupBox( 3, Qt::Horizontal );
    pOpenSpeeds->setFrameStyle( Q3GroupBox::NoFrame );
    builder.AddField< ADN_EditLine_Double >( pOpenSpeeds, "open-terrain-recon-speed", tr( "Open terrain recon speed" ), data_.reconSpeed_, tr( "m/min" ), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pOpenSpeeds, "open-terrain-search-speed", tr( "Open terrain search speed" ), data_.searchSpeed_, tr( "m/min" ), eGreaterEqualZero );

    ADN_AiEngine_UrbanSpeeds_Table* urbanTable = new ADN_AiEngine_UrbanSpeeds_Table( builder.GetChildName( "urban-speeds-table" ), data_.urbanSearchSpeeds_ );

    pSpeedsLayout->addWidget( pOpenSpeeds );
    pSpeedsLayout->addWidget( urbanTable );

    // Perception
    QGroupBox* pPerceptionBox = new gui::RichGroupBox( "perception-box", tr( "Perception" ) );
    QVBoxLayout* pPerceptionLayout = new QVBoxLayout( pPerceptionBox );
    {
        Q3GroupBox* pPerceptionSubBox = new Q3GroupBox( 3, Qt::Horizontal );
        pPerceptionSubBox->setFrameStyle( Q3GroupBox::NoFrame );
        builder.AddEnumField( pPerceptionSubBox, "perception-max-level", tr( "Maximum perception level of hostile units" ), data_.nMaxPerceptionLevel_ );
        ADN_CheckBox* destroyedUnits = new ADN_CheckBox( tr( "Destroyed units detection" ), pPerceptionSubBox, "destroyed-units-detection" );
        destroyedUnits->GetConnector().Connect( &data_.bDetectDestroyedUnits_ );
        pPerceptionLayout->addWidget( pPerceptionSubBox );
    }
    ADN_AiEngine_Perception_Table* perceptionTable = new ADN_AiEngine_Perception_Table( builder.GetChildName( "urban-speeds-table" ), data_.perceptionInfos_ );
    pPerceptionLayout->addWidget( perceptionTable );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QHBoxLayout* pContentLayout = new QHBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    QVBoxLayout* pLeftLayout = new QVBoxLayout( pContentLayout );
    QVBoxLayout* pRightLayout = new QVBoxLayout( pContentLayout );
    pLeftLayout->addWidget( pDangerBox );
    pLeftLayout->addWidget( pOpStateBox );
    pLeftLayout->addWidget( pSpeedsBox );
    pLeftLayout->addWidget( pCriticalIntelligenceBox );
    pLeftLayout->addStretch( 1 );
    pRightLayout->addWidget( pUrbanStateBox );
    pRightLayout->addWidget( pWoundEffectsBox );
    pRightLayout->addWidget( pForceRatioBox );
    pRightLayout->addWidget( pPerceptionBox );
    pRightLayout->addStretch( 1 );
    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent );
}
