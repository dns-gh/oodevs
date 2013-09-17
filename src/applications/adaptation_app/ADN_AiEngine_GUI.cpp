// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-06-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_AiEngine_GUI.cpp $
// $Author: Nld $
// $Modtime: 30/06/05 13:04 $
// $Revision: 13 $
// $Workfile: ADN_AiEngine_GUI.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_AiEngine_GUI.h"
#include "moc_ADN_AiEngine_GUI.cpp"
#include "ADN_Workspace.h"
#include "ADN_AiEngine_Data.h"
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
    builder.AddField< ADN_EditLine_Double >( pUrbanStateBox, "urban-combat-hit-ratio", tr( "Urban combat hit ratio" ), data_.rUrbanCombatWeight_, tr( "%" ), ePercentage );
    
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

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pDangerBox );
    pContentLayout->addWidget( pOpStateBox );
    pContentLayout->addWidget( pUrbanStateBox );
    pContentLayout->addWidget( pCriticalIntelligenceBox );
    pContentLayout->addWidget( pWoundEffectsBox );
    pContentLayout->addWidget( pForceRatioBox );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent );
}
