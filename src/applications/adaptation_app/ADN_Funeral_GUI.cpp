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
    : ADN_GUI_ABC( eLogistic )
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
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( "funeral-tab" );
    static const int maxSize = 500;

    // Funeral
    QWidget* pHolder = builder.AddFieldHolder( 0 );

    ADN_EditLine_Int* pTransporterSpeed = builder.AddField< ADN_EditLine_Int >( pHolder, "fake-transporter-speed", tr( "Fake transporter speed" ), data_.fakeTransporterSpeed_ );
    pTransporterSpeed->GetValidator().setBottom( 1 );
    pHolder->setMaximumWidth( maxSize );

    resourceTable_ = new ADN_FuneralPackagingResources_GUI( builder.GetChildName( "funeral-table" ), data_.funeralPackagingResources_ );
    resourceTable_->SetGoToOnDoubleClick( ::eSupplies );
    resourceTable_->setMaximumWidth( maxSize );
    resourceTable_->setFixedHeight( 200 );
    resourceTable_->setSortingEnabled( false );
    resourceTable_->setSelectionMode( QTableView::SingleSelection );

    moveUpButton_ = builder.AddWidget< QPushButton >( "up" );
    moveUpButton_->setIcon( MAKE_ICON( arrow_up ) );
    moveUpButton_->setMaximumWidth( maxSize );
    moveUpButton_->setEnabled( false );

    moveDownButton_ = builder.AddWidget< QPushButton >( "down" );
    moveDownButton_->setIcon( MAKE_ICON( arrow_down ) );
    moveDownButton_->setMaximumWidth( maxSize );
    moveDownButton_->setEnabled( false );

    connect( moveUpButton_, SIGNAL( clicked() ), this, SLOT( OnButtonUp() ) );
    connect( moveDownButton_, SIGNAL( clicked() ), this, SLOT( OnButtonDown() ) );
    connect( resourceTable_->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ),
             this, SLOT( OnSelectionChanged() ) );
    connect( resourceTable_, SIGNAL( CurrentRemoved() ), this, SLOT( OnSelectionChanged() ) );

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
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent );
    builder.PopSubName(); // "funeral-tab"
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_GUI::OnSelectionChanged
// Created: ABR 2013-11-26
// -----------------------------------------------------------------------------
void ADN_Funeral_GUI::OnSelectionChanged()
{
    QModelIndexList indexes = resourceTable_->selectionModel()->selectedIndexes();
    int row = -1;
    if( indexes.size() == 1 )
        row = indexes[ 0 ].row();
    moveUpButton_->setEnabled( row != -1 && row != 0 );
    moveDownButton_->setEnabled( row != -1 && row != resourceTable_->numRows() - 1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_GUI::SwapResource
// Created: MMC 2011-12-06
// -----------------------------------------------------------------------------
void ADN_Funeral_GUI::SwapResource( int offsetRow )
{
    int numRows = resourceTable_->numRows();
    const QModelIndexList selections = resourceTable_->selectionModel()->selection().indexes();
    int curRow = selections.size() == 1 ? selections[ 0 ].row() : -1;
    int curCol = selections.size() == 1 ? selections[ 0 ].column() : -1;
    int targetRow = curRow + offsetRow;
    if( curRow < 0 || curRow >= numRows ||
        targetRow < 0 || targetRow >= numRows )
        return;

    ADN_FuneralPackagingResource* curPackagingResource = data_.funeralPackagingResources_[ curRow ];
    ADN_FuneralPackagingResource* targetPackagingResource = data_.funeralPackagingResources_[ targetRow ];
    if( curPackagingResource && targetPackagingResource )
    {
        curPackagingResource->ExchangeData( *targetPackagingResource );
        resourceTable_->clearSelection();
        resourceTable_->GetConnector().Disconnect( &data_.funeralPackagingResources_ );
        resourceTable_->GetConnector().Connect( &data_.funeralPackagingResources_ );
        QModelIndex targetIndex = resourceTable_->model()->index( targetRow, curCol );
        resourceTable_->selectionModel()->select( targetIndex, QItemSelectionModel::Select | QItemSelectionModel::Clear );
        OnSelectionChanged();
    }
}
