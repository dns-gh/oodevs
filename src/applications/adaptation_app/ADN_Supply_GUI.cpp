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
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;

    // Supply
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField< ADN_SupplyUnitSelector >( pInfoHolder, tr( "Unit type" ), data_.infos_.ptrUnit_ );
    builder.AddField< ADN_ComboBox_Vector< ADN_Missions_Data::Mission > >( pInfoHolder, tr( "Mission" ), missionConnector_ );
    ConnectMission( true );

    Q3GroupBox* pTypeGroup = new Q3GroupBox( 2, Qt::Vertical, tr( "Convoy type" ) );
    {
        QWidget* pHolder = builder.AddFieldHolder( pTypeGroup );
        builder.AddEnumField< E_SupplyConvoyType >( pHolder, tr( "Between logistic bases" ), data_.infos_.stockSupplyConvoyType_, ADN_Tr::ConvertFromSupplyConvoyType );
        builder.AddEnumField< E_SupplyConvoyType >( pHolder, tr( "For final unit supply " ), data_.infos_.dotationSupplyConvoyType_, ADN_Tr::ConvertFromSupplyConvoyType );
    }

    Q3HGroupBox* pVectorGroup = new Q3HGroupBox( tr( "Conveyors availability warnings" ) );
    ADN_AvailabilityWarningTable* pWarningTable = new ADN_AvailabilityWarningTable( pVectorGroup );
    pWarningTable->GetConnector().Connect( & data_.infos_.vVectorWarnings_ );

    Q3HGroupBox* pSetupGroup = new Q3HGroupBox( tr( "Convoy setup duration" ) );
    ADN_Supply_TrucksTimeTable* pTrucksTimeTable = new ADN_Supply_TrucksTimeTable( pSetupGroup );
    pTrucksTimeTable->GetConnector().Connect( & data_.infos_.vConvoySetupInfos_ );

    Q3HGroupBox* pLoadingGroup = new Q3HGroupBox( tr( "Convoy loading duration" ) );
    pTrucksTimeTable = new ADN_Supply_TrucksTimeTable( pLoadingGroup );
    pTrucksTimeTable->GetConnector().Connect( & data_.infos_.vConvoyLoadingInfos_ );

    Q3HGroupBox* pUnloadingGroup = new Q3HGroupBox( tr( "Convoy unloading duration" ) );
    pTrucksTimeTable = new ADN_Supply_TrucksTimeTable( pUnloadingGroup );
    pTrucksTimeTable->GetConnector().Connect( & data_.infos_.vConvoyUnloadingInfos_ );

    Q3HGroupBox* pSpeedGroup = new Q3HGroupBox( tr( "Convoy speed modifiers" ) );
    ADN_Supply_TrucksDoubleTable* pTrucksDoubleTable = new ADN_Supply_TrucksDoubleTable( pSpeedGroup );
    pTrucksDoubleTable->GetConnector().Connect( & data_.infos_.vConvoySpeedModificatorInfos_ );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Left layout
    QVBoxLayout* pLeftLayout = new QVBoxLayout();
    pLeftLayout->setMargin( 0 );
    pLeftLayout->setSpacing( 10 );
    pLeftLayout->setAlignment( Qt::AlignTop );
    pLeftLayout->addWidget( pInfoHolder );
    pLeftLayout->addWidget( pTypeGroup );
    pLeftLayout->addWidget( pVectorGroup );
    pLeftLayout->addStretch( 1 );

    // Grid layout
    QGridLayout* pGridLayout = new QGridLayout( 0, 2, 2 );
    pGridLayout->setMargin( 0 );
    pGridLayout->setSpacing( 10 );
    pGridLayout->addWidget( pSetupGroup );
    pGridLayout->addWidget( pSpeedGroup );
    pGridLayout->addWidget( pLoadingGroup );
    pGridLayout->addWidget( pUnloadingGroup );

    // Internal content layout
    QHBoxLayout* pInternalContentLayout = new QHBoxLayout();
    pInternalContentLayout->setMargin( 0 );
    pInternalContentLayout->setSpacing( 10 );
    pInternalContentLayout->addLayout( pLeftLayout );
    pInternalContentLayout->addLayout( pGridLayout );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->addLayout( pInternalContentLayout );
    pContentLayout->addStretch( 1 );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent );
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_GUI::ConnectMission
// Created: JSR 2010-05-07
// -----------------------------------------------------------------------------
void ADN_Supply_GUI::ConnectMission( bool connect)
{
    missionConnector_->Connect( &data_.infos_.ptrSupplyMission_, connect );
}
