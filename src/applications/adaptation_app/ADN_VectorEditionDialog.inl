// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_kernel/VariantPointer.h"

// -----------------------------------------------------------------------------
// Name: ADN_VectorEditionDialog constructor
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
template< typename SourceType, typename TargetType >
ADN_VectorEditionDialog< SourceType, TargetType >::ADN_VectorEditionDialog( const QString& objectName,
                                                                            const QString& title,
                                                                            QWidget* parent /* = 0 */ )
    : QDialog( parent )
{
    // Initialization
    setModal( true );
    setObjectName( objectName );
    setCaption( title );
    setMinimumSize( 400, 600 );

    // List
    treeView_ = new QTreeView( this );
    treeView_->setObjectName( objectName + "_treeview" );
    treeView_->header()->setVisible( false );

    dataModel_.reset( new QStandardItemModel( treeView_ ) );
    proxyModel_.reset( new QSortFilterProxyModel( treeView_ ) );
    proxyModel_->setSortLocaleAware( true );
    proxyModel_->setSourceModel( dataModel_.get() );
    treeView_->setModel( proxyModel_.get() );

    // Buttons
    okButton_ = new QPushButton();
    okButton_->setObjectName( objectName + "_ok" );
    cancelButton_ = new QPushButton();
    cancelButton_->setObjectName( objectName + "_cancel" );
    InitializeTranslation();
    connect( okButton_, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( cancelButton_, SIGNAL( clicked() ), this, SLOT( reject() ) );

    // Layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin( 0 );
    buttonLayout->addWidget( okButton_ );
    buttonLayout->addWidget( cancelButton_ );
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->addWidget( treeView_ );
    layout->addLayout( buttonLayout );
}

// -----------------------------------------------------------------------------
// Name: ADN_VectorEditionDialog destructor
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
template< typename SourceType, typename TargetType >
ADN_VectorEditionDialog< SourceType, TargetType >::~ADN_VectorEditionDialog()
{
    editionInfos_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_VectorEditionDialog::setVisible
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
template< typename SourceType, typename TargetType >
void ADN_VectorEditionDialog< SourceType, TargetType >::setVisible( bool visible )
{
    if( visible )
    {
        treeView_->expandAll();
        proxyModel_->sort( 0, Qt::AscendingOrder );
    }
    QDialog::setVisible( visible );
}

// -----------------------------------------------------------------------------
// Name: ADN_VectorEditionDialog::accept
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
template< typename SourceType, typename TargetType >
void ADN_VectorEditionDialog< SourceType, TargetType >::accept()
{
    for( int row = 0; row < dataModel_->rowCount(); ++row )
    {
        QStandardItem* rootItem = dataModel_->item( row );
        assert( rootItem );
        T_EditionInfo* vectorInfo = 0;
        for( auto itVector = editionInfos_.begin(); !vectorInfo && itVector != editionInfos_.end(); ++itVector )
            if( ( *itVector )->name_ == rootItem->text() )
                vectorInfo = &**itVector;
        if( !vectorInfo )
            continue;

        int addedCount = 0;
        for( int child = 0; child < rootItem->rowCount(); ++child )
        {
            QStandardItem* childItem = rootItem->child( child );
            assert( childItem );
            SourceType* source = const_cast< SourceType* >( static_cast< const SourceType* >( childItem->data( gui::Roles::DataRole ).value< kernel::VariantPointer >().ptr_ ) );
            if( !source )
                continue;
            const void* target = childItem->data( gui::Roles::DataRole + 1 ).value< kernel::VariantPointer >().ptr_;

            if( childItem->checkState() == Qt::Checked && !target && ++addedCount )
                vectorInfo->targetConnector_.AddItem( new TargetType( vectorInfo->source_, source ) );
            else if( childItem->checkState() == Qt::Unchecked && target )
                vectorInfo->targetConnector_.RemItem( const_cast< void* >( target ) );
        }
        if( addedCount > 0 )
            vectorInfo->targetConnector_.AddItem( 0 );
    }
    QDialog::accept();
}

namespace
{
    void AddVariantOnItem( QStandardItem* item, int role, const void* ptr )
    {
        QVariant variant;
        variant.setValue( kernel::VariantPointer( ptr ) );
        item->setData( variant, role );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_VectorEditionDialog::AddVector
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
template< typename SourceType, typename TargetType >
void ADN_VectorEditionDialog< SourceType, TargetType >::AddVector( const QString& vectorName,
                                                                   const T_SourceVector& sourceVector,
                                                                   const QStandardItemModel& targetModel,
                                                                   ADN_Connector_Vector_ABC& targetConnector,
                                                                   const T_FilterFunctor& filterFunctor )
{
    editionInfos_.push_back( std::auto_ptr< T_EditionInfo >( new T_EditionInfo( vectorName, sourceVector, targetConnector ) ) );

    QStandardItem* rootItem = new QStandardItem( vectorName );
    rootItem->setFlags( rootItem->flags() | Qt::ItemIsTristate );

    for( auto it = sourceVector.begin(); it != sourceVector.end(); ++it )
    {
        if( *it == 0 || !filterFunctor( **it ) )
            continue;
        QString name = ( *it )->strName_.GetData().c_str();
        QStandardItem* childItem = new QStandardItem( name );
        childItem->setFlags( childItem->flags() | Qt::ItemIsUserCheckable );

        AddVariantOnItem( childItem, gui::Roles::DataRole, *it );

        QList< QStandardItem* > items = targetModel.findItems( name );
        if( items.size() == 1 )
        {
            childItem->setCheckState( Qt::Checked );
            AddVariantOnItem( childItem, gui::Roles::DataRole + 1, static_cast< ADN_StandardItem* >( items[ 0 ] )->GetData() );
        }
        else
        {
            childItem->setCheckState( Qt::Unchecked );
            AddVariantOnItem( childItem, gui::Roles::DataRole + 1, 0 );
        }
        rootItem->appendRow( childItem );
    }
    dataModel_->appendRow( rootItem );
}
