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
#include "FilterProxyModel.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog constructor
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
ModelConsistencyDialog::ModelConsistencyDialog( QWidget* parent, Model& model, const StaticModel& staticModel, kernel::Controllers& controllers )
    : QDialog ( parent, "ModelConsistencyDialog" )
    , actionController_( controllers.actions_ )
    , checker_         ( model, staticModel, controllers )
    , pMapper_         ( new QSignalMapper( this ) )
{
    // Initialize dialog
    setCaption( tr( "Consistency analysis" ) );
    setMinimumSize( 500, 500 );

    // Model
    horizontalHeaders_ << tr( "ID" ) << tr( "Name" ) << tr( "Description" );
    dataModel_ = new QStandardItemModel( this );
    dataModel_->setColumnCount( 3 );
    proxyModel_ = new FilterProxyModel( this );
    proxyModel_->setDynamicSortFilter( true );
    proxyModel_->setSourceModel( dataModel_ );
    proxyModel_->setSortRole( Qt::UserRole + 2 );

    // View
    tableView_ = new QTableView( this );
    tableView_->setModel( proxyModel_ );
    tableView_->setSortingEnabled( true );
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

    // CheckBox
    QHBoxLayout* checkBoxLayout = new QHBoxLayout();
    CreateCheckbox( *checkBoxLayout, boost::assign::map_list_of( ModelConsistencyChecker::eAllProfile, tr( "Profile" ) )
                                                               ( ModelConsistencyChecker::eAllGhost, tr( "Ghost" ) )
                                                               ( ModelConsistencyChecker::eAllInitialization, tr( "Logistic" ) )
                                                               ( ModelConsistencyChecker::eAllUniqueness, tr( "Unicity" ) ) 
                                                               ( ModelConsistencyChecker::eAllDQP, tr( "DQP" ) ) );

    // Layout creation
    QVBoxLayout* mainLayout = new QVBoxLayout();
    setLayout( mainLayout );
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    // Layout management
    mainLayout->addLayout( checkBoxLayout );
    mainLayout->addWidget( tableView_ );
    mainLayout->addLayout( buttonLayout );
    buttonLayout->addWidget( refreshButton );
    buttonLayout->addWidget( closeButton );

    // Fill errors text
    errorDescriptions_[ ModelConsistencyChecker::eLongNameUniqueness ]              = tr( "Duplicate long name for units %1." );
    errorDescriptions_[ ModelConsistencyChecker::eTeamNameUniqueness ]              = tr( "Duplicate name for parties %1." );
    errorDescriptions_[ ModelConsistencyChecker::eObjectNameUniqueness ]            = tr( "Duplicate name for objects %1." );
    errorDescriptions_[ ModelConsistencyChecker::eLimaNameUniqueness ]              = tr( "Duplicate name for limas %1." );
    errorDescriptions_[ ModelConsistencyChecker::eLimitNameUniqueness ]             = tr( "Duplicate name for limits %1." );
    errorDescriptions_[ ModelConsistencyChecker::eStockInitialization ]             = tr( "No stocks initialized." );
    errorDescriptions_[ ModelConsistencyChecker::eLogisticInitialization ]          = tr( "No logistic link initialized." );
    errorDescriptions_[ ModelConsistencyChecker::eProfileUniqueness ]               = tr( "Association with multiple profiles: %1." );
    errorDescriptions_[ ModelConsistencyChecker::eProfileUnreadable ]               = tr( "Not 'readable' to any user profile. You will not be able to see it on the game." );
    errorDescriptions_[ ModelConsistencyChecker::eProfileUnwritable ]               = tr( "Not 'writable' to any user profile. You will not be able to give orders to it on the game." );
    errorDescriptions_[ ModelConsistencyChecker::eGhostExistence ]                  = tr( "A phantom unit is present." );
    errorDescriptions_[ ModelConsistencyChecker::eGhostConverted ]                  = tr( "Unknown type '%1', a phantom unit has been created instead." );
    errorDescriptions_[ ModelConsistencyChecker::eLongNameSize ]                    = tr( "Long name size limit exceeded : %1." );
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
// Name: ModelConsistencyDialog::CreateCheckbox
// Created: LGY 2011-10-26
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::CreateCheckbox( QHBoxLayout& layout, const T_Types& names )
{
    connect( pMapper_, SIGNAL( mapped( int ) ), this, SLOT( OnFilterChanged( int ) ) );
    BOOST_FOREACH( const T_Types::value_type& name, names )
    {
        QCheckBox* pCheckBox = new QCheckBox( name.second );
        connect( pCheckBox, SIGNAL( stateChanged( int ) ), pMapper_, SLOT( map() ) );
        pMapper_->setMapping( pCheckBox, name.first );
        pCheckBox->setCheckState( Qt::Checked );
        layout.addWidget( pCheckBox );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::OnRefresh
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::OnRefresh()
{
    CheckConsistency();
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::CheckConsistency
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::CheckConsistency()
{
    checker_.CheckConsistency();
    UpdateDataModel();
    if( !checker_.GetConsistencyErrors().empty() && !isShown() && dataModel_->rowCount() > 0 )
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
        for( ModelConsistencyChecker::CIT_SafeEntities entityIt = error.entities_.begin(); entityIt != error.entities_.end(); ++entityIt )
        {
            if( *entityIt && **entityIt )
                idList += ( ( idList.isEmpty() ) ? "" : ( entityIt + 1 == error.entities_.end() ) ? tr( " and " ) : ", " ) + QString::number( ( **entityIt )->GetId() );
        }

        for( ModelConsistencyChecker::CIT_SafeEntities entityIt = error.entities_.begin(); entityIt != error.entities_.end(); ++entityIt, ++currentRow )
        {
            const kernel::SafePointer< kernel::Entity_ABC >& entity = **entityIt;
            if( entity )
            {
                QList< QStandardItem* > items;
                AddItem( static_cast< unsigned int >( entity->GetId() ), QString::number( entity->GetId() ), entity, error.type_, items );
                AddItem( entity->GetName(), entity->GetName(), entity, error.type_, items );
                QString text = ( error.type_ & ModelConsistencyChecker::eAllUniqueness ||
                                 error.type_ & ModelConsistencyChecker::eProfileUniqueness ||
                                 error.type_ & ModelConsistencyChecker::eGhostConverted ||
                                 error.type_ & ModelConsistencyChecker::eLongNameSize )
                               ? errorDescriptions_[ error.type_ ].arg( ( error.optional_.empty() ) ? idList : error.optional_.c_str() )
                               : errorDescriptions_[ error.type_ ];

                AddItem( text, text, entity, error.type_, items  );
                dataModel_->appendRow( items );
            }
        }
    }
}

namespace
{
    int Convert( ModelConsistencyChecker::E_ConsistencyCheck type )
    {
        if( type <= ModelConsistencyChecker::eAllUniqueness )
            return ModelConsistencyChecker::eAllUniqueness;
        if( type <= ModelConsistencyChecker::eAllInitialization )
            return ModelConsistencyChecker::eAllInitialization;
        if( type <= ModelConsistencyChecker::eAllProfile )
            return ModelConsistencyChecker::eAllProfile;
        if ( type <= ModelConsistencyChecker::eAllGhost )
            return ModelConsistencyChecker::eAllGhost;
        return ModelConsistencyChecker::eAllDQP;
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::AddItem
// Created: ABR 2011-09-26
// -----------------------------------------------------------------------------
template< typename T >
void ModelConsistencyDialog::AddItem( T data, QString text, const kernel::SafePointer< kernel::Entity_ABC >& entity,
                                      ModelConsistencyChecker::E_ConsistencyCheck type, QList< QStandardItem* >& items )
{
    QStandardItem* item = new QStandardItem( text );
    item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QVariant* variant = new QVariant();
    variant->setValue( VariantPointer( &entity ) );
    item->setData( *variant, Qt::UserRole );
    QVariant* errorType = new QVariant( Convert( type ) );
    item->setData( *errorType, Qt::UserRole + 1 );
    item->setData( data, Qt::UserRole + 2 );
    items.push_back( item );
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::OnSelectionChanged
// Created: ABR 2011-09-26
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::OnSelectionChanged( const QModelIndex& index )
{
    const kernel::SafePointer< kernel::Entity_ABC >* entity = static_cast< const kernel::SafePointer< kernel::Entity_ABC >* >( index.data( Qt::UserRole ).value< VariantPointer >().ptr_ );
    if( entity && *entity )
    {
        ( *entity )->Select( actionController_ );
        ( *entity )->Activate( actionController_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::OnFilterChanged
// Created: LGY 2011-10-26
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::OnFilterChanged( int type )
{
    proxyModel_->ToggleFilter( static_cast< ModelConsistencyChecker::E_ConsistencyCheck >( type ) );
    proxyModel_->setSourceModel( dataModel_ );
}
