// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Supply_GUI.h"

#include "ADN_Supply_Data.h"
#include "ADN_Supply_TrucksDoubleTable.h"
#include "ADN_Supply_TrucksTimeTable.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Tr.h"
#include "ADN_SupplyUnitSelector.h"
#include "ADN_AvailabilityWarningTable.h"

#include "ENT/ENT_Tr.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qvbox.h>


// -----------------------------------------------------------------------------
// Name: ADN_Supply_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Supply_GUI::ADN_Supply_GUI( ADN_Supply_Data& data )
: ADN_GUI_ABC( "ADN_Supply_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Supply_GUI::~ADN_Supply_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Supply_GUI::Build()
{
    if( pMainWidget_ != 0 )
        return;

    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );
    QGroupBox* pGroup = new QGroupBox( 5, Qt::Horizontal, tr( "Supply system data" ), pMainWidget_ );

    QWidget* pHolder = builder.AddFieldHolder( pGroup );

    builder.AddField< ADN_SupplyUnitSelector >( pHolder, tr( "Unit type" ), data_.infos_.ptrUnit_ );
    builder.AddField< ADN_ComboBox_Vector< ADN_Missions_Data::Mission > >( pHolder, tr( "Mission" ), missionConnector_ );
    ConnectMission( true );

    QHGroupBox* pTrucksGroup = new QHGroupBox( tr( "Convoy setup duration" ), pGroup );
    ADN_Supply_TrucksTimeTable* pTrucksTimeTable = new ADN_Supply_TrucksTimeTable( pTrucksGroup );
    pTrucksTimeTable->GetConnector().Connect( & data_.infos_.vConvoySetupInfos_ );

    pTrucksGroup = new QHGroupBox( tr( "Convoy loading duration" ), pGroup );
    pTrucksTimeTable = new ADN_Supply_TrucksTimeTable( pTrucksGroup );
    pTrucksTimeTable->GetConnector().Connect( & data_.infos_.vConvoyLoadingInfos_ );

    pTrucksGroup = new QHGroupBox( tr( "Convoy unloading duration" ), pGroup );
    pTrucksTimeTable = new ADN_Supply_TrucksTimeTable( pTrucksGroup );
    pTrucksTimeTable->GetConnector().Connect( & data_.infos_.vConvoyUnloadingInfos_ );

    pTrucksGroup = new QHGroupBox( tr( "Convoy speed modifiers" ), pGroup );
    ADN_Supply_TrucksDoubleTable* pTrucksDoubleTable = new ADN_Supply_TrucksDoubleTable( pTrucksGroup );
    pTrucksDoubleTable->GetConnector().Connect( & data_.infos_.vConvoySpeedModificatorInfos_ );

    QHGroupBox* pVectorGroup = new QHGroupBox( tr( "Conveyors availability warnings" ), pGroup );
    ADN_AvailabilityWarningTable* pWarningTable = new ADN_AvailabilityWarningTable( pVectorGroup );
    pWarningTable->GetConnector().Connect( & data_.infos_.vVectorWarnings_ );

    // Layout
    QVBoxLayout* pLayout = new QVBoxLayout( pMainWidget_, 10, 10 );
    pLayout->addWidget( pGroup );
    builder.AddStretcher( pLayout, Qt::Vertical );
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_GUI::ConnectMission
// Created: JSR 2010-05-07
// -----------------------------------------------------------------------------
void ADN_Supply_GUI::ConnectMission( bool connect)
{
    missionConnector_->Connect( &data_.infos_.ptrSupplyMission_, connect );
}
