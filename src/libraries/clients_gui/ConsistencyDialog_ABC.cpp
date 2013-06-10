// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ConsistencyDialog_ABC.h"
#include "moc_ConsistencyDialog_ABC.cpp"
#include "clients_kernel/ConsistencyChecker_ABC.h"
#include "RichCheckBox.h"
#include "RichGroupBox.h"
#include "RichPushButton.h"
#include "RichWidget.h"
#include "SubObjectName.h"
#include <boost/foreach.hpp>

using namespace gui;

// =============================================================================
// ConsistencyDialog_ABC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog_ABC constructor
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
ConsistencyDialog_ABC::ConsistencyDialog_ABC( const QString& objectName, QWidget* parent, kernel::ConsistencyChecker_ABC& checker )
    : QDialog( parent )
    , checker_( checker )
    , pMapper_( new QSignalMapper( this ) )
{
    SubObjectName subObject( objectName );
    setCaption( tr( "Consistency analysis" ) );
    connect( pMapper_, SIGNAL( mapped( int ) ), this, SLOT( OnFilterChanged( int ) ) );

    // Model
    dataModel_ = new QStandardItemModel( this );
    // Proxy instantiated in ConsistencyDialog constructor

    // View
    tableView_ = new RichWidget< QTableView >( "tableView", this );
    tableView_->setSortingEnabled( true );
    tableView_->setSelectionBehavior( QAbstractItemView::SelectRows );
    tableView_->setSelectionMode( QAbstractItemView::SingleSelection );
    tableView_->setAlternatingRowColors( true );
    tableView_->verticalHeader()->setVisible( false );
    connect( tableView_, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( OnSelectionChanged( const QModelIndex& ) ) );

    // Buttons
    RichPushButton* refreshButton = new RichPushButton( "refreshButton", tr( "Refresh" ) );
    RichPushButton* closeButton = new RichPushButton( "close", tr( "Close" ) );
    connect( refreshButton, SIGNAL( clicked() ), SLOT( OnRefresh() ) );
    connect( closeButton, SIGNAL( clicked() ), SLOT( OnClose() ) );
    closeButton->setDefault( true );

    // CheckBox type
    typeGroup_ = new RichGroupBox( "typeGroup", tr( "Type" ) );
    checkBoxLayout_ = new QGridLayout( typeGroup_ );
    // CheckBox Level
    levelGroup_ = new RichGroupBox( "levelGroup", tr( "Level" ) );
    QHBoxLayout* levelLayout = new QHBoxLayout( levelGroup_ );
    warningCheckBox_ = new RichCheckBox( "warningCheckBox", tr( "Warning" ) );
    errorCheckBox_ = new RichCheckBox( "errorCheckBox", tr( "Error" ) );
    warningCheckBox_->setCheckState( Qt::Checked );
    errorCheckBox_->setCheckState( Qt::Checked );
    connect( warningCheckBox_, SIGNAL( stateChanged( int ) ), this, SLOT( OnLevelChanged() ) );
    connect( errorCheckBox_  , SIGNAL( stateChanged( int ) ), this, SLOT( OnLevelChanged() ) );
    levelLayout->addWidget( warningCheckBox_ );
    levelLayout->addWidget( errorCheckBox_ );

    // Layout creation
    QVBoxLayout* mainLayout = new QVBoxLayout();
    setLayout( mainLayout );
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    // Layout management
    buttonLayout->addWidget( refreshButton );
    buttonLayout->addWidget( closeButton );

    mainLayout->addWidget( typeGroup_, 1 );
    mainLayout->addWidget( levelGroup_, 1 );
    mainLayout->addWidget( tableView_, 100 );
    mainLayout->addLayout( buttonLayout );
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog_ABC destructor
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
ConsistencyDialog_ABC::~ConsistencyDialog_ABC()
{
    delete &checker_;
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog_ABC::ShowTypeFilter
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
void ConsistencyDialog_ABC::ShowTypeFilter( bool visible )
{
    typeGroup_->setVisible( visible );
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog_ABC::ShowLevelFilter
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
void ConsistencyDialog_ABC::ShowLevelFilter( bool visible )
{
    levelGroup_->setVisible( visible );
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog_ABC::CheckConsistency
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
void ConsistencyDialog_ABC::CheckConsistency()
{
    bool hasErrors = checker_.CheckConsistency();
    UpdateDataModel();
    if( hasErrors && dataModel_->rowCount() > 0 )
        show();
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog_ABC::Display
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
void ConsistencyDialog_ABC::Display()
{
    checker_.CheckConsistency();
    UpdateDataModel();
    show();
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog_ABC::OnRefresh
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
void ConsistencyDialog_ABC::OnRefresh()
{
    emit( ClearLoadingErrors() );
    CheckConsistency();
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog_ABC::OnClose
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
void ConsistencyDialog_ABC::OnClose()
{
    emit( ClearLoadingErrors() );
    reject();
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog_ABC::CreateCheckbox
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
void ConsistencyDialog_ABC::CreateCheckbox( const T_Types& names )
{
    int index = 0;
    BOOST_FOREACH( const T_Types::value_type& name, names )
    {
        RichCheckBox* pCheckBox = new RichCheckBox( name.second, name.second );
        connect( pCheckBox, SIGNAL( stateChanged( int ) ), pMapper_, SLOT( map() ) );
        pMapper_->setMapping( pCheckBox, name.first );
        pCheckBox->setCheckState( Qt::Checked );
        checkBoxLayout_->addWidget( pCheckBox, index / 3, index % 3 );
        ++index;
    }
}

// -----------------------------------------------------------------------------
// Name: ConsistencyDialog_ABC::UpdateDataModel
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
void ConsistencyDialog_ABC::UpdateDataModel()
{
    dataModel_->clear();
    dataModel_->setHorizontalHeaderLabels( horizontalHeaders_ );
    for( int i = 0; i < dataModel_->columnCount(); ++i )
        tableView_->horizontalHeader()->setResizeMode( i, ( i != dataModel_->columnCount() - 1 ) ? QHeaderView::ResizeToContents : QHeaderView::Stretch );
}
