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
#include "clients_kernel/VariantPointer.h"
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

    connect( this, SIGNAL( ClearLoadingErrors() ), parent, SLOT( ClearLoadingErrors() ) );

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
    connect( closeButton, SIGNAL( clicked() ), SLOT( OnClose() ) );
    closeButton->setDefault( true );

    // CheckBox
    QHBoxLayout* checkBoxLayout = new QHBoxLayout();
    CreateCheckbox( *checkBoxLayout, boost::assign::map_list_of( eUniquenessMask,  tr( "Unicity" ) )
                                                               ( eLogisticMask,    tr( "Logistic" ) )
                                                               ( eProfileMask,     tr( "Profile" ) )
                                                               ( eGhostMask,       tr( "Ghost" ) )
                                                               ( eCommandPostMask, tr( "Command Post" ) )
                                                               ( eOthersMask,      tr( "Others" ) ) );

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
    // Uniqueness
    errorDescriptions_[ eLongNameUniqueness ]              = tr( "Duplicate long name for units %1." );
    errorDescriptions_[ eTeamNameUniqueness ]              = tr( "Duplicate name for parties %1." );
    errorDescriptions_[ eObjectNameUniqueness ]            = tr( "Duplicate name for objects %1." );
    errorDescriptions_[ eLimaNameUniqueness ]              = tr( "Duplicate name for limas %1." );
    errorDescriptions_[ eLimitNameUniqueness ]             = tr( "Duplicate name for limits %1." );

    // Logistic
    errorDescriptions_[ eStockInitialization ]             = tr( "No stocks initialized." );
    errorDescriptions_[ eStockMaxExceeded ]                = tr( "Allocated stocks of nature %1 exceed max capacity." );
    errorDescriptions_[ eLogisticInitialization ]          = tr( "No logistic link initialized." );
    errorDescriptions_[ eNoLogisticBase ]                  = tr( "No logistic base defined." );

    // Profile
    errorDescriptions_[ eProfileUniqueness ]               = tr( "Association with multiple profiles: %1." );
    errorDescriptions_[ eProfileUnreadable ]               = tr( "Not 'readable' to any user profile. You will not be able to see it on the game." );
    errorDescriptions_[ eProfileUnwritable ]               = tr( "Not 'writable' to any user profile. You will not be able to give orders to it on the game." );

    // Ghost
    errorDescriptions_[ eGhostExistence ]                  = tr( "A phantom unit is present." );
    errorDescriptions_[ eGhostConverted ]                  = tr( "Unknown type '%1', a phantom unit has been created instead." );

    // Command Post
    errorDescriptions_[ eNoCommandPost ]                  = tr( "Automat has no command post." );
    errorDescriptions_[ eSeveralCommandPost ]             = tr( "Automat has more than one command post." );

    // Others
    errorDescriptions_[ eLongNameSize ]                    = tr( "Long name size limit exceeded : %1." );
    errorDescriptions_[ eUnknownObjectTypes ]              = tr( "Unknown object type \"%1\", some objects could not be loaded." );
    errorDescriptions_[ eUnknownCrowdTypes ]               = tr( "Unknown crowd type \"%1\", some crowds could not be loaded." );
    errorDescriptions_[ eUnknownPopulationTypes ]          = tr( "Unknown population type \"%1\", some populations could not be loaded." );
    errorDescriptions_[ eNoKnowledgeGroup ]                = tr( "Automat has no knowledge group." );
    errorDescriptions_[ eScoreError ]                      = tr( "Score definitions contain errors: %1" );
    errorDescriptions_[ eSuccessFactorError ]              = tr( "Success factor definitions contain errors: %1" );
    errorDescriptions_[ eOthers ]                          = "%1";
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
    emit( ClearLoadingErrors() );
    CheckConsistency();
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::OnClose
// Created: JSR 2012-01-05
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::OnClose()
{
    emit( ClearLoadingErrors() );
    reject();
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::CheckConsistency
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::CheckConsistency()
{
    checker_.CheckConsistency();
    UpdateDataModel();
    if( !checker_.GetConsistencyErrors().empty() && dataModel_->rowCount() > 0 )
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
            QList< QStandardItem* > items;
            const kernel::SafePointer< kernel::Entity_ABC >& entity = **entityIt;
            if( entity )
            {
                AddItem( static_cast< unsigned int >( entity->GetId() ), QString::number( entity->GetId() ), entity, error.type_, items );
                AddItem( entity->GetName(), entity->GetName(), entity, error.type_, items );
            }
            else
            {
                AddItem( 0, "---", entity, error.type_, items );
                AddItem( "---", "---", entity, error.type_, items );
            }

            QString text = errorDescriptions_[ error.type_ ];
            if( text.contains( "%1" ) )
                text = text.arg( ( error.optional_.empty() ) ? idList : error.optional_.c_str() );
            AddItem( text, text, entity, error.type_, items  );
            dataModel_->appendRow( items );
        }
    }
}

namespace
{
    #define CONVERT_TO_MASK( mask ) { if( type & mask ) return mask; }

    int Convert( E_ConsistencyCheck type )
    {
        CONVERT_TO_MASK( eUniquenessMask )
        CONVERT_TO_MASK( eLogisticMask )
        CONVERT_TO_MASK( eProfileMask )
        CONVERT_TO_MASK( eGhostMask )
        CONVERT_TO_MASK( eCommandPostMask )
        CONVERT_TO_MASK( eOthersMask )
        throw std::runtime_error( "Consistency type Unknown" );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::AddItem
// Created: ABR 2011-09-26
// -----------------------------------------------------------------------------
template< typename T >
void ModelConsistencyDialog::AddItem( T data, QString text, const kernel::SafePointer< kernel::Entity_ABC >& entity,
                                      E_ConsistencyCheck type, QList< QStandardItem* >& items )
{
    QStandardItem* item = new QStandardItem( text );
    item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QVariant* variant = new QVariant();
    variant->setValue( kernel::VariantPointer( &entity ) );
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
    const kernel::SafePointer< kernel::Entity_ABC >* entity = static_cast< const kernel::SafePointer< kernel::Entity_ABC >* >( index.data( Qt::UserRole ).value< kernel::VariantPointer >().ptr_ );
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
    proxyModel_->ToggleFilter( static_cast< E_ConsistencyCheck >( type ) );
    proxyModel_->setSourceModel( dataModel_ );
}
