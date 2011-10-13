// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Funeral_GUI.h"

#include "ADN_Funeral_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Tr.h"

#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Funeral_GUI::ADN_Funeral_GUI( ADN_Funeral_Data& data )
    : ADN_GUI_ABC( "ADN_Funeral_GUI" )
    , data_      ( data )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ADN_Funeral_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Funeral_GUI::~ADN_Funeral_GUI()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ADN_Funeral_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Funeral_GUI::Build()
{
    if( pMainWidget_ != 0 )
        return;

    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );
    Q3GroupBox* pGroup = new Q3GroupBox( 5, Qt::Horizontal, tr( "Funeral system data" ), pMainWidget_ );

    /*QWidget* pHolder = builder.AddFieldHolder( pGroup );

    builder.AddField< ADN_FuneralUnitSelector >( pHolder, tr( "Unit type" ), data_.infos_.ptrUnit_ );
    builder.AddField< ADN_ComboBox_Vector< ADN_Missions_Data::Mission > >( pHolder, tr( "Mission" ), missionConnector_ );
    ConnectMission( true );

    Q3HGroupBox* pTrucksGroup = new Q3HGroupBox( tr( "Convoy setup duration" ), pGroup );
    ADN_Funeral_TrucksTimeTable* pTrucksTimeTable = new ADN_Funeral_TrucksTimeTable( pTrucksGroup );
    pTrucksTimeTable->GetConnector().Connect( & data_.infos_.vConvoySetupInfos_ );

    pTrucksGroup = new Q3HGroupBox( tr( "Convoy loading duration" ), pGroup );
    pTrucksTimeTable = new ADN_Funeral_TrucksTimeTable( pTrucksGroup );
    pTrucksTimeTable->GetConnector().Connect( & data_.infos_.vConvoyLoadingInfos_ );

    pTrucksGroup = new Q3HGroupBox( tr( "Convoy unloading duration" ), pGroup );
    pTrucksTimeTable = new ADN_Funeral_TrucksTimeTable( pTrucksGroup );
    pTrucksTimeTable->GetConnector().Connect( & data_.infos_.vConvoyUnloadingInfos_ );

    pTrucksGroup = new Q3HGroupBox( tr( "Convoy speed modifiers" ), pGroup );
    ADN_Funeral_TrucksDoubleTable* pTrucksDoubleTable = new ADN_Funeral_TrucksDoubleTable( pTrucksGroup );
    pTrucksDoubleTable->GetConnector().Connect( & data_.infos_.vConvoySpeedModificatorInfos_ );

    Q3HGroupBox* pVectorGroup = new Q3HGroupBox( tr( "Conveyors availability warnings" ), pGroup );
    ADN_AvailabilityWarningTable* pWarningTable = new ADN_AvailabilityWarningTable( pVectorGroup );
    pWarningTable->GetConnector().Connect( & data_.infos_.vVectorWarnings_ );

    // Layout
    Q3VBoxLayout* pLayout = new Q3VBoxLayout( pMainWidget_, 10, 10 );
    pLayout->addWidget( pGroup );
    builder.AddStretcher( pLayout, Qt::Vertical );*/
}

