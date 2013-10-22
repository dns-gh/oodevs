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
ADN_VectorEditionDialog< SourceType, TargetType >::ADN_VectorEditionDialog( const QString& objectName, const QString& title, QWidget* parent /* = 0 */ )
    : QDialog( parent )
{
    // Initialization
    setModal( true );
    setObjectName( objectName );
    setCaption( title );
    setMinimumSize( 400, 600 );

    // List
    treeView_ = new QTreeWidget( this );
    treeView_->setObjectName( objectName + "_treeview" );
    treeView_->header()->setVisible( false );

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
        treeView_->sortItems( 0, Qt::AscendingOrder );
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
    for( QTreeWidgetItemIterator itRoot = QTreeWidgetItemIterator( treeView_ ); *itRoot; ++itRoot )
    {
        QTreeWidgetItem* rootItem = *itRoot;
        assert( rootItem );
        T_EditionInfo* vectorInfo = 0;
        for( auto itVector = editionInfos_.begin(); !vectorInfo && itVector != editionInfos_.end(); ++itVector )
            if( ( *itVector )->name_ == rootItem->text( 0 ) )
                vectorInfo = &**itVector;
        if( !vectorInfo )
            continue;

        int addedCount = 0;
        for( QTreeWidgetItemIterator itChild = QTreeWidgetItemIterator( itRoot ); *itChild; ++itChild )
        {
            QTreeWidgetItem* childItem = *itChild;
            assert( childItem );
            SourceType* source = const_cast< SourceType* >( static_cast< const SourceType* >( childItem->data( 0, gui::Roles::DataRole ).value< kernel::VariantPointer >().ptr_ ) );
            if( !source )
                continue;
            const void* target = childItem->data( 0, gui::Roles::DataRole + 1 ).value< kernel::VariantPointer >().ptr_;

            if( childItem->checkState( 0 ) == Qt::Checked && !target && ++addedCount )
                vectorInfo->targetConnector_.AddItem( new TargetType( vectorInfo->source_, source ) );
            else if( childItem->checkState( 0 ) == Qt::Unchecked && target )
                vectorInfo->targetConnector_.RemItem( const_cast< void* >( target ) );
        }
        if( addedCount > 0 )
            vectorInfo->targetConnector_.AddItem( 0 );
    }
    QDialog::accept();
}

namespace
{
    void AddVariantOnItem( QTreeWidgetItem* item, int role, const void* ptr )
    {
        QVariant variant;
        variant.setValue( kernel::VariantPointer( ptr ) );
        item->setData( 0, role, variant );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_VectorEditionDialog::AddVector
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
template< typename SourceType, typename TargetType >
void ADN_VectorEditionDialog< SourceType, TargetType >::AddVector( const QString& vectorName, const T_SourceVector& sourceVector, const QStandardItemModel& targetModel, ADN_Connector_Vector_ABC& targetConnector )
{
    editionInfos_.push_back( std::auto_ptr< T_EditionInfo >( new T_EditionInfo( vectorName, sourceVector, targetConnector ) ) );

    QTreeWidgetItem* rootItem = new QTreeWidgetItem( treeView_ );
    rootItem->setText( 0, vectorName );
    rootItem->setFlags( rootItem->flags() | Qt::ItemIsTristate );

    for( auto it = sourceVector.begin(); it != sourceVector.end(); ++it )
    {
        QString name = ( *it )->strName_.GetData().c_str();
        QTreeWidgetItem* childItem = new QTreeWidgetItem( rootItem );
        childItem->setText( 0, name );
        childItem->setFlags( childItem->flags() | Qt::ItemIsUserCheckable );

        AddVariantOnItem( childItem, gui::Roles::DataRole, *it );

        QList< QStandardItem* > items = targetModel.findItems( name );
        if( items.size() == 1 )
        {
            childItem->setCheckState( 0, Qt::Checked );
            AddVariantOnItem( childItem, gui::Roles::DataRole + 1, static_cast< ADN_StandardItem* >( items[ 0 ] )->GetData() );
        }
        else
        {
            childItem->setCheckState( 0, Qt::Unchecked );
            AddVariantOnItem( childItem, gui::Roles::DataRole + 1, 0 );
        }
    }
}
