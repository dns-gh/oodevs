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
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;
    static const int maxSize = 500;

    // Funeral
    QWidget* pHolder = builder.AddFieldHolder( 0 );

    ADN_EditLine_Int* pTransporterSpeed = builder.AddField<ADN_EditLine_Int>( pHolder, tr( "Fake transporter speed" ), data_.fakeTransporterSpeed_ );
    pTransporterSpeed->GetValidator().setBottom( 1 );
    pHolder->setMaximumWidth( maxSize );

    resourceTable_ = new ADN_FuneralPackagingResources_GUI();
    resourceTable_->GetConnector().Connect( &data_.funeralPackagingResources_ );
    resourceTable_->setMaximumWidth( maxSize );
    resourceTable_->setFixedHeight( 200 );
    resourceTable_->setSorting( false );
    resourceTable_->setSelectionMode( Q3Table::SingleRow );

    QPushButton* moveUpButton_ = new QPushButton();
    moveUpButton_->setIcon( MAKE_ICON( arrow_up ) );
    moveUpButton_->setMaximumWidth( maxSize );
    
    QPushButton* moveDownButton_ = new QPushButton();
    moveDownButton_->setIcon( MAKE_ICON( arrow_down ) );
    moveDownButton_->setMaximumWidth( maxSize );

    connect( moveUpButton_, SIGNAL( clicked() ), this, SLOT( OnButtonUp() ) );
    connect( moveDownButton_, SIGNAL( clicked() ), this, SLOT( OnButtonDown() ) );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pHolder );
    pContentLayout->addWidget( resourceTable_ );
    pContentLayout->addWidget( moveUpButton_ );
    pContentLayout->addWidget( moveDownButton_ );
    pContentLayout->addStretch( 1 );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent );
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