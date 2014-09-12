// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "RemoveBlocksDialog.h"
#include "moc_RemoveBlocksDialog.cpp"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/VariantPointer.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichSpinBox.h"
#include "clients_gui/RichWidget.h"
#include "preparation/UrbanHierarchies.h"
#include "preparation/UrbanModel.h"

// -----------------------------------------------------------------------------
// Name: RemoveBlocksDialog constructor
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
RemoveBlocksDialog::RemoveBlocksDialog( QWidget* parent, kernel::Controllers& controllers, UrbanModel& model )
    : QDialog( parent, "RemoveBlocksDialog", false )
    , urbanModel_ ( model )
    , controllers_( controllers )
    , processing_ ( false )
{
    gui::SubObjectName subObject( "RemoveBlocksDialog" );
    setCaption( tr( "Remove urban blocks" ) );
    setMinimumSize( 400, 500 );

    // Size field
    QHBoxLayout* sizeLayout = new QHBoxLayout();
    //sizeLayout
    sizeLayout->setAlignment( Qt::AlignCenter );
    sizeLayout->setMargin( 10 );
    sizeLayout->setSpacing( 10 );
    sizeLayout->addWidget( new QLabel( tr( "Minimum size:" ) ) );
    sizeEditor_ = new gui::RichSpinBox( "sizeEditor" );
    sizeLayout->addWidget( sizeEditor_, 1 );
    sizeLayout->addWidget( new QLabel( tr( "m²" ) ) );
    connect( sizeEditor_, SIGNAL( valueChanged( int ) ), SLOT( OnValueChanged( int ) ) );

    // List view
    horizontalHeaders_ << "" << tr( "ID" ) << tr( "Name" ) << tr( "Area (m²)" );
    dataModel_ = new QStandardItemModel( this );
    tableView_ = new gui::RichWidget< QTableView >( "tableView" );
    tableView_->setSortingEnabled( true );
    tableView_->setSelectionBehavior( QAbstractItemView::SelectRows );
    tableView_->setSelectionMode( QAbstractItemView::SingleSelection );
    tableView_->setAlternatingRowColors( true );
    tableView_->verticalHeader()->setVisible( false );
    connect( tableView_, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( OnSelectionChanged( const QModelIndex& ) ) );
    connect( dataModel_, SIGNAL( itemChanged( QStandardItem* ) ), this, SLOT( OnItemChanged( QStandardItem* ) ) );

    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel();
    proxyModel->setDynamicSortFilter( true );
    proxyModel->setSourceModel( dataModel_ );
    proxyModel->setSortRole( Qt::UserRole + 2 );
    tableView_->setModel( proxyModel );
    tableView_->setEditTriggers( gui::RichWidget< QTableView >::AllEditTriggers );

    // Label layout
    QLabel* selectAllLabel = new QLabel();
    selectAllLabel->setText( "<a href='selectall'>" + Qt::escape( tr( "All" ) ) + "</a>" );
    connect( selectAllLabel, SIGNAL( linkActivated( const QString& ) ), SLOT( OnLinkActivated( const QString& ) ) );
    QLabel* selectNoneLabel = new QLabel();
    selectNoneLabel->setText( "<a href='selectnone'>" + Qt::escape( tr( "None" ) ) + "</a>" );
    connect( selectNoneLabel, SIGNAL( linkActivated( const QString& ) ), SLOT( OnLinkActivated( const QString& ) ) );

    QHBoxLayout* labelLayout = new QHBoxLayout();
    labelLayout->setMargin( 5 );
    labelLayout->setSpacing( 10 );
    labelLayout->addWidget( new QLabel( tr( "Check:" ) ), 0, Qt::AlignLeft );
    labelLayout->addWidget( selectAllLabel, 0, Qt::AlignLeft );
    labelLayout->addWidget( selectNoneLabel, 0, Qt::AlignLeft );
    labelLayout->addStretch();

    // Buttons
    deleteButton_ = new gui::RichPushButton( "delete", tr( "Delete" ) );
    connect( deleteButton_, SIGNAL( pressed() ), SLOT( accept() ) );
    gui::RichPushButton* cancelButton = new gui::RichPushButton( "cancel", tr( "Cancel" ) );
    connect( cancelButton, SIGNAL( pressed() ), SLOT( reject() ) );
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget( deleteButton_ );
    buttonLayout->addWidget( cancelButton );

    // Layout
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addLayout( sizeLayout );
    mainLayout->addWidget( new QLabel( tr( "The following urban blocks will be removed:" ) ) );
    mainLayout->addWidget( tableView_ );
    mainLayout->addLayout( labelLayout );
    mainLayout->addLayout( buttonLayout );
    hide();

    // Help
    QShortcut* s = new QShortcut( QKeySequence( Qt::Key_F1 ), this );
    connect( s, SIGNAL( activated() ), parent, SIGNAL( ShowHelp() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RemoveBlocksDialog destructor
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
RemoveBlocksDialog::~RemoveBlocksDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RemoveBlocksDialog::accept
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
void RemoveBlocksDialog::accept()
{
    assert( !urbanObjects_.empty() );
    urbanModel_.DeleteBlocks( urbanObjects_ );
    urbanObjects_.clear();
    selectables_.clear();
    QDialog::accept();
}

// -----------------------------------------------------------------------------
// Name: RemoveBlocksDialog::showEvent
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
void RemoveBlocksDialog::showEvent( QShowEvent* event )
{
    sizeEditor_->setValue( 1000 );
    QDialog::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: RemoveBlocksDialog::NotifySelectionChanged
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
void RemoveBlocksDialog::NotifySelectionChanged( const T_Elements& elements )
{
    urbanObjects_ = elements;
}

// -----------------------------------------------------------------------------
// Name: RemoveBlocksDialog::AddItem
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
template< typename T >
void RemoveBlocksDialog::AddItem( QList< QStandardItem* >& items, T sortElement, const QString& displayText, const kernel::UrbanObject_ABC& entity, bool checkable /* = false */ )
{
    QStandardItem* item = new QStandardItem( displayText );
    item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    if( checkable )
    {
        item->setFlags( item->flags() | Qt::ItemIsUserCheckable );
        item->setCheckState( Qt::Checked ); // $$$$ ABR 2012-06-13: Improvement, keep last state and set it back
    }

    // User role        = selection role
    QVariant variant;
    kernel::SafePointer< kernel::UrbanObject_ABC >* safePtr = new kernel::SafePointer< kernel::UrbanObject_ABC >( controllers_, &entity );
    variant.setValue( kernel::VariantPointer( safePtr ) );
    item->setData( variant, Qt::UserRole );

    // User role + 2    = sort role
    item->setData( sortElement, Qt::UserRole + 2 );

    items.push_back( item );
}

// -----------------------------------------------------------------------------
// Name: RemoveBlocksDialog::OnValueChanged
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
void RemoveBlocksDialog::OnValueChanged( int value )
{
    dataModel_->clear();
    selectables_.clear();
    dataModel_->setHorizontalHeaderLabels( horizontalHeaders_ );
    tableView_->horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
    tableView_->horizontalHeader()->setResizeMode( 1, QHeaderView::ResizeToContents  );
    tableView_->horizontalHeader()->setResizeMode( 2, QHeaderView::Stretch );
    tableView_->horizontalHeader()->setResizeMode( 3, QHeaderView::ResizeToContents );

    tools::Iterator< const kernel::UrbanObject_ABC& > it = urbanModel_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::UrbanObject_ABC& urbanObject = it.NextElement();
        if( const UrbanHierarchies* hierarchy = static_cast< const UrbanHierarchies* >( urbanObject.Retrieve< kernel::Hierarchies >() ) )
            if( hierarchy->GetLevel() == eUrbanLevelBlock )
                if( const kernel::UrbanPositions_ABC* position = urbanObject.Retrieve< kernel::UrbanPositions_ABC >() )
                {
                    float area = position->ComputeArea();
                    if( area < static_cast< float >( value ) )
                    {
                        QList< QStandardItem* > items;

                        // CheckBox
                        AddItem( items, true, "", urbanObject, true );
                        // ID
                        AddItem( items, static_cast< unsigned int >( urbanObject.GetId() ), locale().toString( static_cast< unsigned int >( urbanObject.GetId() ) ), urbanObject );
                        // Name
                        AddItem( items, urbanObject.GetName(), urbanObject.GetName(), urbanObject );
                        // Area
                        AddItem( items, area, locale().toString( area ), urbanObject );

                        dataModel_->appendRow( items );
                        selectables_.push_back( &urbanObject );
                    }
                }
    }
    controllers_.actions_.SetMultipleSelection( selectables_ );
    deleteButton_->setEnabled( !selectables_.empty() );
}

// -----------------------------------------------------------------------------
// Name: RemoveBlocksDialog::OnItemChanged
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
void RemoveBlocksDialog::OnItemChanged( QStandardItem* item )
{
    if( !processing_ && item && item->isCheckable() )
    {
        const kernel::SafePointer< kernel::UrbanObject_ABC >* safeUrbanObject = static_cast< const kernel::SafePointer< kernel::UrbanObject_ABC >* >( item->index().data( Qt::UserRole ).value< kernel::VariantPointer >().ptr_ );
        if( !safeUrbanObject || !*safeUrbanObject )
            return;
        const kernel::UrbanObject_ABC& urbanObject = **safeUrbanObject;
        if( item->checkState() == Qt::Checked )
        {
            assert( std::find( selectables_.begin(), selectables_.end(), &urbanObject ) == selectables_.end() );
            selectables_.push_back( &urbanObject );
        }
        else
        {
            assert( item->checkState() == Qt::Unchecked );
            auto it = std::find( selectables_.begin(), selectables_.end(), &urbanObject );
            assert( it != selectables_.end() );
            selectables_.erase( it );
        }
        controllers_.actions_.SetMultipleSelection( selectables_ );
        deleteButton_->setEnabled( !selectables_.empty() );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoveBlocksDialog::OnLinkActivated
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
void RemoveBlocksDialog::OnLinkActivated( const QString& link )
{
    selectables_.clear();
    if( link != "selectall" && link != "selectnone" )
        return;
    Qt::CheckState state = ( link == "selectall" ) ? Qt::Checked : Qt::Unchecked;
    processing_ = true;
    for( int i = 0; i < dataModel_->rowCount(); ++i )
    {
        QStandardItem* item = dataModel_->item( i, 0 );
        assert( item && item->isCheckable() );
        item->setCheckState( state );
        if( state == Qt::Checked )
        {
            const kernel::SafePointer< kernel::UrbanObject_ABC >* safeUrbanObject = static_cast< const kernel::SafePointer< kernel::UrbanObject_ABC >* >( item->index().data( Qt::UserRole ).value< kernel::VariantPointer >().ptr_ );
            const kernel::UrbanObject_ABC& urbanObject = **safeUrbanObject;
            selectables_.push_back( &urbanObject );
        }
    }
    processing_ = false;
    controllers_.actions_.SetMultipleSelection( selectables_ );
    deleteButton_->setEnabled( !selectables_.empty() );
}

// -----------------------------------------------------------------------------
// Name: RemoveBlocksDialog::OnSelectionChanged
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
void RemoveBlocksDialog::OnSelectionChanged( const QModelIndex& index )
{
    const kernel::SafePointer< kernel::UrbanObject_ABC >* entity = static_cast< const kernel::SafePointer< kernel::UrbanObject_ABC >* >( index.data( Qt::UserRole ).value< kernel::VariantPointer >().ptr_ );
    if( entity && *entity )
        ( *entity )->Activate( controllers_.actions_ );
}
