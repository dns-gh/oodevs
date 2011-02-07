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
#include "moc_ADN_People_GUI.cpp"
#include "ADN_GuiBuilder.h"
#include "ADN_People_Data.h"
#include "ADN_People_ListView.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Consumptions_Table.h"
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

    QVBox* pMainBox = new QVBox( pMainWidget_ );

    // Population parameters
    QGroupBox* pGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Population" ), pMainBox );

    QGroupBox* pPropertiesGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Details" ), pGroup );

    builder.AddField< ADN_EditLine_String >( pPropertiesGroup, tr( "Name" ), vInfosConnectors[ eName ] );
    builder.AddField< ADN_ComboBox_Vector< ADN_Population_Data::PopulationInfos > >( pPropertiesGroup, tr( "Associated Crowd" ), vInfosConnectors[ eModel ] );
    pMaleEditLine_ = builder.AddField< ADN_EditLine_Int >( pPropertiesGroup, tr( "Male" ), vInfosConnectors[ eMale ], tr( "%" ), ePercentage );
    connect( pMaleEditLine_, SIGNAL( textChanged( const QString& ) ), this, SLOT( PercentageChanged() ) );
    pFemaleEditLine_ = builder.AddField< ADN_EditLine_Int >( pPropertiesGroup, tr( "Female" ), vInfosConnectors[ eFemale ], tr( "%" ), ePercentage );
    connect( pFemaleEditLine_, SIGNAL( textChanged( const QString& ) ), this, SLOT( PercentageChanged() ) );
    pChildrenEditLine_ = builder.AddField< ADN_EditLine_Int >( pPropertiesGroup, tr( "Children" ), vInfosConnectors[ eChildren ], tr( "%" ), ePercentage );
    connect( pChildrenEditLine_, SIGNAL( textChanged( const QString& ) ), this, SLOT( PercentageChanged() ) );

    QGroupBox* pSecurityGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Security satisfaction level" ), pGroup );
    builder.AddField< ADN_EditLine_Double >( pSecurityGroup, tr( "Loss on fire" ), vInfosConnectors[ eLossOnFire ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Double >( pSecurityGroup, tr( "Gain per hour" ), vInfosConnectors[ eGainPerHour ], tr( "%" ), ePercentage );

    QGroupBox* pHealthGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Health satisfaction" ), pGroup );
    builder.AddField< ADN_EditLine_Int >( pHealthGroup, tr( "Number of people per medical infrastructure" ), vInfosConnectors[ eHealthNeed ] );

    QGroupBox* pScheduleGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Moving weekly schedule" ), pGroup );
    builder.AddField< ADN_TimeField >( pScheduleGroup, tr( "Transfer time" ), vInfosConnectors[ eTransferTime ] );
    ADN_Schedule_Table* pTable = new ADN_Schedule_Table( pScheduleGroup );
    connect( pPeopleList_, SIGNAL( ItemSelected( void* ) ), pTable, SLOT( OnPeopleChanged( void* ) ) );

    QHGroupBox* pConsumptionsGroup = new QHGroupBox( tr( "Consumptions" ), pGroup );
    ADN_Consumptions_Table* pConsumptions = new ADN_Consumptions_Table( pConsumptionsGroup );
    vInfosConnectors[ eConsumptions ] = &pConsumptions->GetConnector();

    pPeopleList_->SetItemConnectors( vInfosConnectors );

    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pPeopleList_, 1 );
    pMainLayout->addWidget( pMainBox, 3 );
}

// -----------------------------------------------------------------------------
// Name: ADN_People_GUI::PercentageChanged
// Created: LGY 2010-12-28
// -----------------------------------------------------------------------------
void ADN_People_GUI::PercentageChanged()
{
    ADN_People_Data::PeopleInfos* pInfos = static_cast< ADN_People_Data::PeopleInfos* >( pPeopleList_->GetCurrentData() );
    if( pInfos == 0 )
        return;
    pMaleEditLine_->GetValidator().setTop( 100 - pInfos->children_.GetData() - pInfos->female_.GetData() );
    pFemaleEditLine_->GetValidator().setTop( 100 - pInfos->children_.GetData() - pInfos->male_.GetData() );
    pChildrenEditLine_->GetValidator().setTop( 100 - pInfos->female_.GetData() - pInfos->male_.GetData() );
}
