// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamDotationTypeList.h"
#include "moc_ParamDotationTypeList.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/DotationType.h"
#include "actions/DotationTypeList.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"
#include <tools/Iterator.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList constructor
// Created: ABR 2012-01-10
// -----------------------------------------------------------------------------
ParamDotationTypeList::ParamDotationTypeList( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder, parameter )
    , resolver_( builder.GetStaticModel().objectTypes_ )
    , list_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList destructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamDotationTypeList::~ParamDotationTypeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamDotationTypeList::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );

    group_->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    list_ = new QTreeView( parent );
    list_->setRootIsDecorated( true );
    list_->setEditTriggers( 0 );
    model_.setColumnCount( 1 );
    list_->setModel( &model_ );
    list_->setHeaderHidden( true );
    list_->setSelectionMode( QAbstractItemView::MultiSelection );
    list_->header()->setResizeMode( 0, QHeaderView::ResizeToContents );

    tools::Iterator< const kernel::DotationType& > it = resolver_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::DotationType& type = it.NextElement();
        AddItem( type.GetCategoryDisplay().c_str(), type.GetName().c_str(), type.GetId() );
    }
    connect( list_, SIGNAL( clicked( const QModelIndex& ) ), SLOT( Clicked( const QModelIndex& ) ) );
    connect( list_, SIGNAL( clicked( const QModelIndex& ) ), SLOT( Update() ) );
    layout->addWidget( list_ );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList::Clicked
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void ParamDotationTypeList::Clicked( const QModelIndex& index )
{
    QStandardItem* item = index.isValid()? model_.itemFromIndex( index ): 0 ;
    if( item )
    {
        QStandardItem* child = item->child( 0 );
        if( child )
        {
            bool selected = !list_->selectionModel()->isSelected( child->index() );
            if( selected )
                list_->expand( index );
            for( int row = 0 ; row < item->rowCount(); ++row )
                list_->selectionModel()->select( item->child( row )->index(), selected ? QItemSelectionModel::Select : QItemSelectionModel::Deselect );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList::AddItem
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void ParamDotationTypeList::AddItem( const QString& parent, const QString& child, unsigned int id )
{
    QList< QStandardItem* > parentItemList = model_.findItems( parent );
    QStandardItem* parentItem;
    if( parentItemList.isEmpty() )
    {
        parentItem = new QStandardItem( parent );
        parentItem->setSelectable( false );
        model_.appendRow( parentItem );
    }
    else
        parentItem = parentItemList[ 0 ];
    QStandardItem* item = new QStandardItem( child );
    item->setData( id );
    parentItem->appendRow( item );
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ParamDotationTypeList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( !list_ )
        return;
    std::unique_ptr< actions::Parameter_ABC > param( new actions::parameters::DotationTypeList( parameter_ ) );
    if( IsChecked() )
    {
        QModelIndexList selection = list_->selectionModel()->selection().indexes();
        for( auto it = selection.begin(); it != selection.end(); ++it )
        {
            QStandardItem* item = model_.itemFromIndex( *it );
            if( item->data().isValid() )
            {
                const unsigned id = item->data().toUInt();
                param->AddParameter( *new actions::parameters::DotationType( parameter_, id, resolver_ ) );
            }
        }
    }
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList::InternalCheckValidity
// Created: ABR 2011-11-22
// -----------------------------------------------------------------------------
bool ParamDotationTypeList::InternalCheckValidity() const
{
    return list_ && model_.rowCount() != 0;
}
