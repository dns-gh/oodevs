// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_People_GUI.h"
#include "ADN_GuiBuilder.h"
#include "ADN_People_Data.h"
#include "ADN_People_ListView.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Consumptions_Table.h"
#include "ADN_MultiPercentage.h"
#include "ADN_Schedule_Table.h"

// -----------------------------------------------------------------------------
// Name: ADN_People_GUI constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
ADN_People_GUI::ADN_People_GUI( ADN_People_Data& data )
    : ADN_GUI_ABC( "ADN_People_GUI" )
    , data_      ( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_People_GUI destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
ADN_People_GUI::~ADN_People_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_People_GUI::Build
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void ADN_People_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0, "Population main widget" );

    // Create the population listview.
    pPeopleList_ = new ADN_People_ListView( pMainWidget_ );
    pPeopleList_->GetConnector().Connect( &data_.GetPeople() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    Q3VBox* pMainBox = new Q3VBox( pMainWidget_ );

    // Population parameters
    Q3GroupBox* pGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Population" ), pMainBox );

    Q3GroupBox* pPropertiesGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Details" ), pGroup );

    builder.AddField< ADN_EditLine_String >( pPropertiesGroup, tr( "Name" ), vInfosConnectors[ eName ] );
    builder.AddField< ADN_ComboBox_Vector< ADN_Population_Data::PopulationInfos > >( pPropertiesGroup, tr( "Associated Crowd" ), vInfosConnectors[ eModel ] );
    // repartition
    ADN_MultiPercentage* pMultiPercentage = new ADN_MultiPercentage( pPropertiesGroup, builder );
    pMultiPercentage->AddLine( tr( "Males" ), vInfosConnectors[ eMale ] );
    pMultiPercentage->AddLine( tr( "Females" ), vInfosConnectors[ eFemale ] );
    pMultiPercentage->AddLine( tr( "Children" ), vInfosConnectors[ eChildren ] );
    pMultiPercentage->AddWarning();

    Q3GroupBox* pSecurityGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Security satisfaction level" ), pGroup );
    builder.AddField< ADN_EditLine_Double >( pSecurityGroup, tr( "Loss on fire" ), vInfosConnectors[ eLossOnFire ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Double >( pSecurityGroup, tr( "Gain per hour" ), vInfosConnectors[ eGainPerHour ], tr( "%" ), ePercentage );

    Q3GroupBox* pHealthGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Health satisfaction" ), pGroup );
    builder.AddField< ADN_EditLine_Int >( pHealthGroup, tr( "Number of people per medical infrastructure" ), vInfosConnectors[ eHealthNeed ], 0, eGreaterEqualZero );

    Q3GroupBox* pScheduleGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Moving weekly schedule" ), pGroup );
    builder.AddField< ADN_TimeField >( pScheduleGroup, tr( "Transfer time" ), vInfosConnectors[ eTransferTime ] );
    ADN_Schedule_Table* pTable = new ADN_Schedule_Table( pScheduleGroup );
    connect( pPeopleList_, SIGNAL( ItemSelected( void* ) ), pTable, SLOT( OnPeopleChanged( void* ) ) );

    Q3HGroupBox* pConsumptionsGroup = new Q3HGroupBox( tr( "Consumptions" ), pGroup );
    ADN_Consumptions_Table* pConsumptions = new ADN_Consumptions_Table( pConsumptionsGroup );
    vInfosConnectors[ eConsumptions ] = &pConsumptions->GetConnector();

    pPeopleList_->SetItemConnectors( vInfosConnectors );

    // Layout
    Q3HBoxLayout* pMainLayout = new Q3HBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pPeopleList_, 1 );
    pMainLayout->addWidget( pMainBox, 3 );
}
