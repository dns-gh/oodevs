// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
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

#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_CommonGfx.h"
#include "ADN_FileChooser.h"
#include "ADN_Project_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_TimeField.h"

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_GUI constructor
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
ADN_AiEngine_GUI::ADN_AiEngine_GUI( ADN_AiEngine_Data& data )
    : ADN_GUI_ABC( "ADN_AiEngine_GUI" )
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
    ADN_GuiBuilder builder;

    // Danger
    Q3GroupBox* pDangerBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Threat management" ) );
    builder.AddField<ADN_EditLine_Double>( pDangerBox, tr( "Operational state maximum effect on threat" ), data_.rOperationalStateMaxDecrease_, tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pDangerBox, tr( "Information relevance maximum effect on threat" ), data_.rPertinenceMaxDecrease_, tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pDangerBox, tr( "Neutralization state maximum effect on threat" ), data_.rNeutralizedStateMaxDecrease_, tr( "%" ), ePercentage );

    // Operational state
    Q3GroupBox* pOpStateBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Operational state computation" ) );
    builder.AddField<ADN_EditLine_Double>( pOpStateBox, tr( "Non major equipments state weight" ), data_.rMinorEquipmentWeight_, 0, eZeroOne );
    builder.AddField<ADN_EditLine_Double>( pOpStateBox, tr( "Major equipments state weight" ), data_.rMajorEquipmentWeight_, 0, eZeroOne );
    builder.AddField<ADN_EditLine_Double>( pOpStateBox, tr( "Crew state weight" ), data_.rHumanWeight_, 0, eZeroOne );
    builder.AddField<ADN_EditLine_Double>( pOpStateBox, tr( "Decisional threshold" ), data_.rDecisionalThreshold_, 0, eZeroOne );

    // Urban area effect on fire
    Q3GroupBox* pUrbanStateBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Urban combat computation" ) );
    builder.AddField<ADN_EditLine_Double>( pUrbanStateBox, tr( "Urban combat hit ratio" ), data_.rUrbanCombatWeight_, tr( "%" ), ePercentage );

    // Critical Intelligence
    Q3GroupBox* pCriticalIntelligenceBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Time to acquire critical intelligence" ) );
    builder.AddField<ADN_TimeField>( pCriticalIntelligenceBox, tr( "Minimum affinity" ), data_.rMinimumAffinity_ );
    builder.AddField<ADN_TimeField>( pCriticalIntelligenceBox, tr( "Neutral affinity" ), data_.rNeutralAffinity_ );
    builder.AddField<ADN_TimeField>( pCriticalIntelligenceBox, tr( "Maximum affinity" ), data_.rMaximumAffinity_ );

    // Wound effects modificators
    Q3GroupBox* pWoundEffectsBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Equipment state on power indicators" ) );
    builder.AddField<ADN_EditLine_Double>( pWoundEffectsBox, tr( "Available" ), data_.rAvailableModificator_, tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pWoundEffectsBox, tr( "Unavailable" ), data_.rUnavailableModificator_, tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pWoundEffectsBox, tr( "Repairable" ), data_.rRepairableModificator_, tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pWoundEffectsBox, tr( "Repairing" ), data_.rRepairingModificator_, tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pWoundEffectsBox, tr( "Captured" ), data_.rCapturedModificator_, tr( "%" ), ePercentage );

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

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent );
}
