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

#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qgrid.h>


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

    // Files
    QGroupBox* pFilesGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Files" ), pMainWidget_ );
    ADN_FileChooser* pChooser = 0;

    pChooser = builder.AddFileField( pFilesGroup, tr( "Binary folder" ), data_.strBinariesFolder_ );
    pChooser->SetMode( ADN_FileChooser::eDirectory );

    pChooser = builder.AddFileField( pFilesGroup, tr( "Sources folder" ), data_.strSourcesFolder_ );
    pChooser->SetMode( ADN_FileChooser::eDirectory );

    pChooser = builder.AddFileField( pFilesGroup, tr( "DIA Types file" ), data_.strDIATypesFile_ );
    pChooser = builder.AddFileField( pFilesGroup, tr( "DIA Workspace file" ), data_.strDIAWorkspaceFile_ );
    pChooser = builder.AddFileField( pFilesGroup, tr( "Models file" ), data_.strModelsFile_ );

    builder.AddField<ADN_EditLine_Int>( pFilesGroup, tr( "Debugger port" ), data_.nDebuggerPort_ );

    // Danger
    QGroupBox* pDangerBox = new QGroupBox( 3, Qt::Horizontal, tr( "Danger management" ), pMainWidget_ );

    builder.AddField<ADN_EditLine_Double>( pDangerBox, tr( "Maximum decrease due to the operational state" ), data_.rOperationalStateMaxDecrease_, tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pDangerBox, tr( "Maximum decrease due to the pertinence" ), data_.rPertinenceMaxDecrease_, tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pDangerBox, tr( "Maximum decrease due to the neutralized" ), data_.rNeutralizedStateMaxDecrease_, tr( "%" ), ePercentage );

    // Operational state
    QGroupBox* pOpStateBox = new QGroupBox( 3, Qt::Horizontal, tr( "Operationnal state" ), pMainWidget_ );
    
    builder.AddField<ADN_EditLine_Double>( pOpStateBox, tr( "Non major composantes weight" ), data_.rMinorEquipmentWeight_, 0, eZeroOne );
    builder.AddField<ADN_EditLine_Double>( pOpStateBox, tr( "Major composantes weight" ), data_.rMajorEquipmentWeight_, 0, eZeroOne );
    builder.AddField<ADN_EditLine_Double>( pOpStateBox, tr( "Humans weight" ), data_.rHumanWeight_, 0, eZeroOne );

    // Layout
    QVBoxLayout* pMainLayout = new QVBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pFilesGroup );
    pMainLayout->addWidget( pDangerBox );
    pMainLayout->addWidget( pOpStateBox );

    builder.AddStretcher( pMainWidget_, Qt::Vertical );
}
