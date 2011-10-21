// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ModelConsistencyDialog.h"
#include "moc_ModelConsistencyDialog.cpp"
#include "clients_gui/Tools.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Entity_ABC.h"

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog constructor
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
ModelConsistencyDialog::ModelConsistencyDialog( QWidget* parent, Model& model, const StaticModel& staticModel, kernel::ActionController& actionController )
    : QDialog ( parent, "ModelConsistencyDialog" )
    , actionController_( actionController )
    , checker_         ( model, staticModel )
{
    // Initialize dialog
    setCaption( tr( "Consistency analysis" ) );
    setMinimumSize( 500, 500 );

    // Model
    horizontalHeaders_ << tr( "ID" ) << tr( "Name" ) << tr( "Description" );
    dataModel_ = new QStandardItemModel( this );
    dataModel_->setColumnCount( 3 );

    // View
    tableView_ = new QTableView( this );
    tableView_->setModel( dataModel_ );
    tableView_->setSelectionBehavior( QAbstractItemView::SelectRows );
    tableView_->setSelectionMode( QAbstractItemView::SingleSelection );
    tableView_->setAlternatingRowColors( true );
    tableView_->verticalHeader()->setVisible( false );
    connect( tableView_, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( OnSelectionChanged( const QModelIndex& ) ) );

    // Buttons
    QPushButton* refreshButton = new QPushButton( tr( "Refresh" ) );
    QPushButton* closeButton = new QPushButton( tr( "Close" ) );
    connect( refreshButton, SIGNAL( clicked() ), SLOT( OnRefresh() ) );
    connect( closeButton, SIGNAL( clicked() ), SLOT( reject() ) );

    // Layout creation
    QVBoxLayout* mainLayout = new QVBoxLayout();
    setLayout( mainLayout );
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    // Layout management
    mainLayout->addWidget( tableView_ );
    mainLayout->addLayout( buttonLayout );
    buttonLayout->addWidget( refreshButton );
    buttonLayout->addWidget( closeButton );

    // Fill errors text
    errorDescriptions_[ ModelConsistencyChecker::eLongNameUniqueness ]     = tr( "Duplicate long name for units %1." );
    errorDescriptions_[ ModelConsistencyChecker::eTeamNameUniqueness ]     = tr( "Duplicate name for parties %1." );
    errorDescriptions_[ ModelConsistencyChecker::eObjectNameUniqueness ]   = tr( "Duplicate name for objects %1." );
    errorDescriptions_[ ModelConsistencyChecker::eLimaNameUniqueness ]     = tr( "Duplicate name for limas %1." );
    errorDescriptions_[ ModelConsistencyChecker::eLimitNameUniqueness ]    = tr( "Duplicate name for limits %1." );
    errorDescriptions_[ ModelConsistencyChecker::eStockInitialization ]    = tr( "No stocks initialized." );
    errorDescriptions_[ ModelConsistencyChecker::eLogisticInitialization ] = tr( "No logistic link initialized." );
    errorDescriptions_[ ModelConsistencyChecker::eProfileUniqueness ]      = tr( "Association with multiple profiles: %1." );
    errorDescriptions_[ ModelConsistencyChecker::eProfileUnreadable ]      = tr( "Not 'readable' to any user profile. You will not be able to see it on the game." );
    errorDescriptions_[ ModelConsistencyChecker::eProfileUnwritable ]      = tr( "Not 'writable' to any user profile. You will not be able to give orders to it on the game." );
    errorDescriptions_[ ModelConsistencyChecker::eGhostExistence ]         = tr( "A phantom unit is present." );
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog destructor
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
ModelConsistencyDialog::~ModelConsistencyDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::OnRefresh
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::OnRefresh()
{
    CheckConsistency( currentFilters_ );
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::CheckConsistency
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::CheckConsistency( unsigned int filters /*= ModelConsistencyChecker::eAllChecks*/ )
{
    currentFilters_ = filters;
    checker_.CheckConsistency( currentFilters_ );
    UpdateDataModel();
    if( !checker_.GetConsistencyErrors().empty() && !isShown() )
        show();
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::UpdateErrorListView
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::UpdateDataModel()
{
    dataModel_->clear();
    dataModel_->setHorizontalHeaderLabels( horizontalHeaders_ );
    tableView_->horizontalHeader()->setResizeMode( eID, QHeaderView::ResizeToContents );
    tableView_->horizontalHeader()->setResizeMode( eName, QHeaderView::ResizeToContents );
    tableView_->horizontalHeader()->setResizeMode( eDescription, QHeaderView::Stretch );

    int currentRow = 0;
    const ModelConsistencyChecker::T_ConsistencyErrors& errors = checker_.GetConsistencyErrors();
    for( ModelConsistencyChecker::CIT_ConsistencyErrors it = errors.begin(); it != errors.end(); ++it )
    {
        const ModelConsistencyChecker::ConsistencyError& error = *it;
        QString idList;
        for( std::vector< const kernel::Entity_ABC* >::const_iterator entityIt = error.entities_.begin(); entityIt != error.entities_.end(); ++entityIt )
            idList += ( ( idList.isEmpty() ) ? "" : ( entityIt + 1 == error.entities_.end() ) ? tr( " and " ) : ", " ) + QString::number( ( *entityIt )->GetId() );

        for( std::vector< const kernel::Entity_ABC* >::const_iterator entityIt = error.entities_.begin(); entityIt != error.entities_.end(); ++entityIt, ++currentRow )
        {
            const kernel::Entity_ABC& entity = **entityIt;
            AddItem( currentRow, eID, QString::number( entity.GetId() ), entity );
            AddItem( currentRow, eName, entity.GetName(), entity );
            AddItem( currentRow, eDescription, ( error.type_ & ModelConsistencyChecker::eAllUniqueness || error.type_ & ModelConsistencyChecker::eProfileUniqueness )
                ? errorDescriptions_[ error.type_ ].arg( ( error.optional_.empty() ) ? idList : error.optional_.c_str() )
                : errorDescriptions_[ error.type_ ], entity );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::AddItem
// Created: ABR 2011-09-26
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::AddItem( int row, int col, QString text, const kernel::Entity_ABC& entity )
{
    QStandardItem* item = new QStandardItem( text );
    item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QVariant* variant = new QVariant();
    variant->setValue( VariantPointer( &entity ) );
    item->setData( *variant, Qt::UserRole + 1 );
    dataModel_->setItem( row, col, item );
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::OnSelectionChanged
// Created: ABR 2011-09-26
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::OnSelectionChanged( const QModelIndex& index )
{
    const kernel::Entity_ABC* entity = static_cast< const kernel::Entity_ABC* >( index.data( Qt::UserRole + 1 ).value< VariantPointer >().ptr_ );
    if( entity )
    {
        entity->Select( actionController_ );
        entity->Activate( actionController_ );
    }
}
