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
#include "moc_ADN_Funeral_GUI.cpp"
#include "ADN_Funeral_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_FuneralPackagingResources_GUI.h"
#include "ADN_Tr.h"
#include "clients_gui/resources.h"

#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Funeral_GUI::ADN_Funeral_GUI( ADN_Funeral_Data& data )
    : ADN_GUI_ABC( "ADN_Funeral_GUI" )
    , data_      ( data )
    , resourceTable_( 0 )
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

    static const int maxSize = 500;

    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );
    Q3GroupBox* pGroup = new Q3GroupBox( 4, Qt::Vertical, tr( "Funeral system data" ), pMainWidget_ );    
    QWidget* pHolder = builder.AddFieldHolder( pGroup );

    ADN_EditLine_Int* pTransporterSpeed = builder.AddField<ADN_EditLine_Int>( pHolder, tr( "Fake transporter speed" ), data_.fakeTransporterSpeed_ );
    pTransporterSpeed->GetValidator().setBottom( 1 );
    pTransporterSpeed->setMaximumWidth( 300 );

    resourceTable_ = new ADN_FuneralPackagingResources_GUI( pGroup );
    resourceTable_->GetConnector().Connect( &data_.funeralPackagingResources_ );
    resourceTable_->setMaximumWidth( maxSize );
    resourceTable_->setSorting( false );
    resourceTable_->setSelectionMode( Q3Table::SingleRow );

    QPushButton* moveUpButton_ = new QPushButton( pGroup );
    moveUpButton_->setIcon( MAKE_ICON( arrow_up ) );
    moveUpButton_->setMaximumWidth( maxSize );
    
    QPushButton* moveDownButton_ = new QPushButton( pGroup );
    moveDownButton_->setIcon( MAKE_ICON( arrow_down ) );
    moveDownButton_->setMaximumWidth( maxSize );

    connect( moveUpButton_, SIGNAL( clicked() ), this, SLOT( OnButtonUp() ) );
    connect( moveDownButton_, SIGNAL( clicked() ), this, SLOT( OnButtonDown() ) );

/*
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
*/

    // Layout
    Q3HBoxLayout* pLayout = new Q3HBoxLayout( pMainWidget_, 10, 10 );
    pLayout->addWidget( pGroup );

    builder.AddStretcher( pLayout, Qt::Vertical );
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_GUI::SwapResource
// Created: MMC 2011-12-06
// -----------------------------------------------------------------------------
void ADN_Funeral_GUI::SwapResource( int offsetRow )
{
    if( !resourceTable_ )
        return;

    int numRows = resourceTable_->numRows();
    int curRow = resourceTable_->currentRow();
    int targetRow = curRow + offsetRow;

    if( curRow < 0 || curRow >= numRows )
        return;
    if ( targetRow < 0 || targetRow >= numRows )
        return;

    ADN_FuneralPackagingResource* curPackagingResource = data_.funeralPackagingResources_[ curRow ];
    ADN_FuneralPackagingResource* targetPackagingResource = data_.funeralPackagingResources_[ targetRow ];
    if ( curPackagingResource && targetPackagingResource )
    {
        curPackagingResource->ExchangeData( *targetPackagingResource );
        resourceTable_->clearSelection();
        resourceTable_->GetConnector().Disconnect( &data_.funeralPackagingResources_ );
        resourceTable_->GetConnector().Connect( &data_.funeralPackagingResources_ );
        resourceTable_->updateContents();
        resourceTable_->selectRow( targetRow );
    }
}