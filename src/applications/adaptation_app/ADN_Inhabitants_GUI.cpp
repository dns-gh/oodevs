// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Inhabitants_GUI.h"
#include "moc_ADN_Inhabitants_GUI.cpp"
#include "ADN_GuiBuilder.h"
#include "ADN_GoToButton.h"
#include "ADN_Inhabitants_Data.h"
#include "ADN_Inhabitants_ListView.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Consumptions_Table.h"
#include "ADN_MultiPercentage.h"
#include "ADN_Schedule_Table.h"

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_GUI constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
ADN_Inhabitants_GUI::ADN_Inhabitants_GUI( ADN_Inhabitants_Data& data )
    : ADN_GUI_ABC( eInhabitants )
    , data_      ( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_GUI destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
ADN_Inhabitants_GUI::~ADN_Inhabitants_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_GUI::Build
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void ADN_Inhabitants_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Population parameters
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddLocalizedField( data_.GetInhabitants(), pInfoHolder, "name", tr( "Name" ), vInfosConnectors[ eName ] );
    ADN_GoToButton* goToButton = new ADN_GoToButton( ::eCrowds );
    goToButton->SetLinkedCombo( builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, "associated-crowd", tr( "Associated Crowd" ), vInfosConnectors[ eModel ], 0, eNone, goToButton ) );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, "angry-crowd-misison", tr( "Angry crowd mission" ), vInfosConnectors[ eAngryCrowdMission ] );
    // repartition
    Q3GroupBox* pRepartition = new Q3GroupBox( 3, Qt::Horizontal, tr( "Distribution" ) );
    ADN_MultiPercentage_Double* pMultiPercentage = new ADN_MultiPercentage_Double( pRepartition, builder, builder.GetChildName( "distribution" ) );
    pMultiPercentage->AddLine( tr( "Males" ), vInfosConnectors[ eMale ], "males" );
    pMultiPercentage->AddLine( tr( "Females" ), vInfosConnectors[ eFemale ], "females" );
    pMultiPercentage->AddLine( tr( "Children" ), vInfosConnectors[ eChildren ], "children" );
    pMultiPercentage->AddWarning();

    // Security
    Q3GroupBox* pSecurityGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Security satisfaction level" ) );
    builder.AddField< ADN_EditLine_Double >( pSecurityGroup, "loss-on-fire", tr( "Loss on fire" ), vInfosConnectors[ eLossOnFire ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Double >( pSecurityGroup, "gain-per-hour", tr( "Gain per hour" ), vInfosConnectors[ eGainPerHour ], tr( "%" ), ePercentage );

    // Health
    Q3GroupBox* pHealthGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Health satisfaction" ) );
    builder.AddField< ADN_EditLine_Int >( pHealthGroup, "number-of-people-per-medical", tr( "Number of people per medical infrastructure" ), vInfosConnectors[ eHealthNeed ], 0, eGreaterEqualZero );

    // Schedule
    Q3GroupBox* pScheduleGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Moving weekly schedule" ) );
    builder.AddField< ADN_TimeField >( pScheduleGroup, "transfer-time", tr( "Transfer time" ), vInfosConnectors[ eTransferTime ] );

    new ADN_Schedule_Table( builder.GetChildName( "schedule-table" ), vInfosConnectors[ eSchedule ], pScheduleGroup );

    // Consumptions
    Q3HGroupBox* pConsumptionsGroup = new Q3HGroupBox( tr( "Consumptions" ) );
    new ADN_Consumptions_Table( builder.GetChildName( "consumptions-table" ), vInfosConnectors[ eConsumptions ], pConsumptionsGroup );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QGridLayout* pContentLayout = new QGridLayout( pContent, 5, 2 );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder, 0, 0, 1, 2 );
    pContentLayout->addWidget( pRepartition, 1, 0, 1, 2 );
    pContentLayout->addWidget( pSecurityGroup, 2, 0, 1, 2 );
    pContentLayout->addWidget( pHealthGroup, 4, 0, 1, 2 );
    pContentLayout->addWidget( pScheduleGroup, 5, 0 );
    pContentLayout->addWidget( pConsumptionsGroup, 5, 1 );

    // List view
    QWidget* pSearchListView = builder.AddSearchListView< ADN_Inhabitants_ListView >( this, data_.GetInhabitants(), vInfosConnectors );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent, pSearchListView );
}
