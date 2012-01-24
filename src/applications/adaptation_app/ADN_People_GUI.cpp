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
#include "ADN_SearchListView.h"

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
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Population parameters
    Q3GroupBox* pPropertiesGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Details" ) );
    builder.AddField< ADN_EditLine_String >( pPropertiesGroup, tr( "Name" ), vInfosConnectors[ eName ] );
    builder.AddField< ADN_ComboBox_Vector< ADN_Population_Data::PopulationInfos > >( pPropertiesGroup, tr( "Associated Crowd" ), vInfosConnectors[ eModel ] );
    builder.AddField< ADN_EditLine_String >( pPropertiesGroup, tr( "Angry crowd mission" ), vInfosConnectors[ eAngryCrowdMission ] );
    // repartition
    ADN_MultiPercentage* pMultiPercentage = new ADN_MultiPercentage( pPropertiesGroup, builder );
    pMultiPercentage->AddLine( tr( "Males" ), vInfosConnectors[ eMale ] );
    pMultiPercentage->AddLine( tr( "Females" ), vInfosConnectors[ eFemale ] );
    pMultiPercentage->AddLine( tr( "Children" ), vInfosConnectors[ eChildren ] );
    pMultiPercentage->AddWarning();

    // Security
    Q3GroupBox* pSecurityGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Security satisfaction level" ) );
    builder.AddField< ADN_EditLine_Double >( pSecurityGroup, tr( "Loss on fire" ), vInfosConnectors[ eLossOnFire ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Double >( pSecurityGroup, tr( "Gain per hour" ), vInfosConnectors[ eGainPerHour ], tr( "%" ), ePercentage );

    // Health
    Q3GroupBox* pHealthGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Health satisfaction" ) );
    builder.AddField< ADN_EditLine_Int >( pHealthGroup, tr( "Number of people per medical infrastructure" ), vInfosConnectors[ eHealthNeed ], 0, eGreaterEqualZero );

    // Schedule
    Q3GroupBox* pScheduleGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Moving weekly schedule" ) );
    builder.AddField< ADN_TimeField >( pScheduleGroup, tr( "Transfer time" ), vInfosConnectors[ eTransferTime ] );
    ADN_Schedule_Table* pTable = new ADN_Schedule_Table( pScheduleGroup );

    // Consumptions
    Q3HGroupBox* pConsumptionsGroup = new Q3HGroupBox( tr( "Consumptions" ) );
    ADN_Consumptions_Table* pConsumptions = new ADN_Consumptions_Table( pConsumptionsGroup );
    vInfosConnectors[ eConsumptions ] = &pConsumptions->GetConnector();

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    //QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    QGridLayout* pContentLayout = new QGridLayout( pContent, 4, 2 );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pPropertiesGroup, 0, 0, 1, 2 );
    pContentLayout->addWidget( pSecurityGroup, 1, 0, 1, 2 );
    pContentLayout->addWidget( pHealthGroup, 3, 0, 1, 2 );
    pContentLayout->addWidget( pScheduleGroup, 4, 0 );
    pContentLayout->addWidget( pConsumptionsGroup, 4, 1 );

    // List view
    ADN_SearchListView< ADN_People_ListView >* pSearchListView = new ADN_SearchListView< ADN_People_ListView >( data_.GetPeople(), vInfosConnectors );
    connect( pSearchListView->GetListView(), SIGNAL( ItemSelected( void* ) ), pTable, SLOT( OnPeopleChanged( void* ) ) );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent, pSearchListView );
}
