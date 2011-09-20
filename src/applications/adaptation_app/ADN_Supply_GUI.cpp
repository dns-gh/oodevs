// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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

// -----------------------------------------------------------------------------
// Name: ADN_Supply_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Supply_GUI::ADN_Supply_GUI( ADN_Supply_Data& data )
    : ADN_GUI_ABC( "ADN_Supply_GUI" )
    , data_      ( data )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Supply_GUI::~ADN_Supply_GUI()
{
    // NOTHING
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
    Q3GroupBox* pGroup = new Q3GroupBox( 5, Qt::Horizontal, tr( "Supply system data" ), pMainWidget_ );

    QWidget* pHolder = builder.AddFieldHolder( pGroup );

    builder.AddField< ADN_SupplyUnitSelector >( pHolder, tr( "Unit type" ), data_.infos_.ptrUnit_ );
    builder.AddField< ADN_ComboBox_Vector< ADN_Missions_Data::Mission > >( pHolder, tr( "Mission" ), missionConnector_ );
    ConnectMission( true );

    Q3GroupBox* pTypeGroup = new Q3VGroupBox( tr( "Convoy type" ), pGroup );
    builder.AddEnumField<E_SupplyConvoyType>( pTypeGroup, tr( "Between logistic bases" ), data_.infos_.dotationSupplyConvoyType_, ADN_Tr::ConvertFromSupplyConvoyType );
    builder.AddEnumField<E_SupplyConvoyType>( pTypeGroup, tr( "For final unit supply " ), data_.infos_.stockSupplyConvoyType_, ADN_Tr::ConvertFromSupplyConvoyType );

    Q3HGroupBox* pTrucksGroup = new Q3HGroupBox( tr( "Convoy setup duration" ), pGroup );
    ADN_Supply_TrucksTimeTable* pTrucksTimeTable = new ADN_Supply_TrucksTimeTable( pTrucksGroup );
    pTrucksTimeTable->GetConnector().Connect( & data_.infos_.vConvoySetupInfos_ );

    pTrucksGroup = new Q3HGroupBox( tr( "Convoy loading duration" ), pGroup );
    pTrucksTimeTable = new ADN_Supply_TrucksTimeTable( pTrucksGroup );
    pTrucksTimeTable->GetConnector().Connect( & data_.infos_.vConvoyLoadingInfos_ );

    pTrucksGroup = new Q3HGroupBox( tr( "Convoy unloading duration" ), pGroup );
    pTrucksTimeTable = new ADN_Supply_TrucksTimeTable( pTrucksGroup );
    pTrucksTimeTable->GetConnector().Connect( & data_.infos_.vConvoyUnloadingInfos_ );

    pTrucksGroup = new Q3HGroupBox( tr( "Convoy speed modifiers" ), pGroup );
    ADN_Supply_TrucksDoubleTable* pTrucksDoubleTable = new ADN_Supply_TrucksDoubleTable( pTrucksGroup );
    pTrucksDoubleTable->GetConnector().Connect( & data_.infos_.vConvoySpeedModificatorInfos_ );

    Q3HGroupBox* pVectorGroup = new Q3HGroupBox( tr( "Conveyors availability warnings" ), pGroup );
    ADN_AvailabilityWarningTable* pWarningTable = new ADN_AvailabilityWarningTable( pVectorGroup );
    pWarningTable->GetConnector().Connect( & data_.infos_.vVectorWarnings_ );

    // Layout
    Q3VBoxLayout* pLayout = new Q3VBoxLayout( pMainWidget_, 10, 10 );
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
