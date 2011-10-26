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
}


// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_GUI destructor
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
ADN_AiEngine_GUI::~ADN_AiEngine_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_GUI::Build
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
void ADN_AiEngine_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0, "Ai main widget" );

    // Danger
    Q3GroupBox* pDangerBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Threat management" ), pMainWidget_ );

    builder.AddField<ADN_EditLine_Double>( pDangerBox, tr( "Operational state maximum effect on threat" ), data_.rOperationalStateMaxDecrease_, tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pDangerBox, tr( "Information relevance maximum effect on threat" ), data_.rPertinenceMaxDecrease_, tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pDangerBox, tr( "Neutralization state maximum effect on threat" ), data_.rNeutralizedStateMaxDecrease_, tr( "%" ), ePercentage );

    // Operational state
    Q3GroupBox* pOpStateBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Operational state computation" ), pMainWidget_ );

    builder.AddField<ADN_EditLine_Double>( pOpStateBox, tr( "Non major equipments state weight" ), data_.rMinorEquipmentWeight_, 0, eZeroOne );
    builder.AddField<ADN_EditLine_Double>( pOpStateBox, tr( "Major equipments state weight" ), data_.rMajorEquipmentWeight_, 0, eZeroOne );
    builder.AddField<ADN_EditLine_Double>( pOpStateBox, tr( "Crew state weight" ), data_.rHumanWeight_, 0, eZeroOne );
    builder.AddField<ADN_EditLine_Double>( pOpStateBox, tr( "Decisional threshold" ), data_.rDecisionalThreshold_, 0, eZeroOne );

    // Urban area effect on fire
    Q3GroupBox* pUrbanStateBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Urban combat computation" ), pMainWidget_ );
    builder.AddField<ADN_EditLine_Double>( pUrbanStateBox, tr( "Urban combat hit ratio" ), data_.rUrbanCombatWeight_, tr( "%" ), ePercentage );

    // Critical Intelligence
    Q3GroupBox* pCriticalIntelligenceBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Time to acquire critical intelligence" ), pMainWidget_ );
    builder.AddField<ADN_TimeField>( pCriticalIntelligenceBox, tr( "Minimum affinity" ), data_.rMinimumAffinity_ );
    builder.AddField<ADN_TimeField>( pCriticalIntelligenceBox, tr( "Neutral affinity" ), data_.rNeutralAffinity_ );
    builder.AddField<ADN_TimeField>( pCriticalIntelligenceBox, tr( "Maximum affinity" ), data_.rMaximumAffinity_ );

    // Layout
    Q3VBoxLayout* pMainLayout = new Q3VBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pDangerBox );
    pMainLayout->addWidget( pOpStateBox );
    pMainLayout->addWidget( pUrbanStateBox );
    pMainLayout->addWidget( pCriticalIntelligenceBox );

    builder.AddStretcher( pMainWidget_, Qt::Vertical );
}
