// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamDotationTypeList.h"
#include "moc_ParamDotationTypeList.cpp"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Iterator.h"
#include "gaming/ActionParameterDotationTypeList.h"
#include "gaming/ActionParameterDotationType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList constructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamDotationTypeList::ParamDotationTypeList( QObject* parent, const kernel::OrderParameter& parameter, const Resolver_ABC< DotationType >& resolver )
    : QObject( parent )
    , Param_ABC( parameter.GetName() )
    , resolver_( resolver )
    , parameter_( parameter )
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
void ParamDotationTypeList::BuildInterface( QWidget* parent )
{
    list_ = new QListView( parent );
    list_->setRootIsDecorated( true );
    list_->addColumn( "name" ); list_->addColumn( "id", 0 );
    list_->setSorting( -1, true );
    list_->header()->hide();
    list_->header()->setResizeEnabled( false, 1 );
    list_->hideColumn( 1 );
    
    list_->setSelectionMode( QListView::Multi );

    Iterator< const DotationType& > it = resolver_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const DotationType& type = it.NextElement();
        AddItem( type.GetName().c_str(), type.GetCategory().c_str(), type.GetId() );
    }

    connect( list_, SIGNAL( clicked( QListViewItem* ) ), SLOT( Clicked( QListViewItem* ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList::Clicked
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void ParamDotationTypeList::Clicked( QListViewItem* item )
{
    QListViewItem* child = item ? item->firstChild() : 0;
    bool selected = child ? !child->isSelected() : false;
    list_->ensureItemVisible( child );
    while( child )
    {
        list_->setSelected( child, selected );
        child = child->nextSibling();
    }
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList::AddItem
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void ParamDotationTypeList::AddItem( const QString& parent, const QString& child, unsigned id )
{
    QListViewItem* parentItem = list_->findItem( parent, 0 );
    if( ! parentItem )
    {
        parentItem = new QListViewItem( list_, parent );
        parentItem->setSelectable( false );
    }
    new QListViewItem( parentItem, child, QString::number( id ) );
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ParamDotationTypeList::CommitTo( ActionParameterContainer_ABC& action ) const
{
    if( ! list_ )
        return;

    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterDotationTypeList( parameter_ ) );
    QListViewItemIterator it( list_ );
    while( it.current() )
    {
        if( it.current()->isSelected() )
        {
            const unsigned id = it.current()->text( 1 ).toUInt();
            param->AddParameter( *new ActionParameterDotationType( parameter_, id, resolver_ ) );
        }
        ++it;
    }
    action.AddParameter( *param.release() );
}
