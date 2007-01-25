// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UnitListView.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatComposition.h"
#include "clients_kernel/AgentNature.h"
#include "clients_kernel/AutomatType.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitListView constructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
UnitListView::UnitListView( QWidget* parent, Controllers& controllers, const AgentTypes& list, ItemFactory_ABC& factory )
    : ListView< UnitListView >( parent, *this, factory )
    , controllers_( controllers )
    , types_( list )
{
    setMinimumSize( 1, 1 );
    addColumn( tr( "Unit type" ) );
    addColumn( tr( "Count" ) );
    setRootIsDecorated( true );
    header()->hide();
    setSorting( -1 );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitListView destructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
UnitListView::~UnitListView()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::SetOpen
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
void UnitListView::SetOpen( bool open )
{
    for( QListViewItem* item = firstChild(); item; item = item->nextSibling() )
        item->setOpen( open );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::SetSorting
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void UnitListView::SetSorting( const std::string& nature )
{
    sorting_ = nature;
    clear();
    DisplayList();
}

// -----------------------------------------------------------------------------
// Name: UnitListView::NotifyUpdated
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void UnitListView::NotifyUpdated( const ModelLoaded& )
{
    DisplayList();
}

// -----------------------------------------------------------------------------
// Name: UnitListView::NotifyUpdated
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void UnitListView::NotifyUpdated( const ModelUnLoaded& )
{
    clear();
}

// -----------------------------------------------------------------------------
// Name: UnitListView::DisplayList
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
void UnitListView::DisplayList()
{
    if( sorting_ == "level" )
        DisplayBy( &AgentNature::GetLevel );
    else if( sorting_ == "atlas" )
        DisplayBy( &AgentNature::GetAtlas );
    else if( sorting_ == "nature" )
        DisplayByNature();
    else
    {
        Iterator< const AutomatType& > it( types_.Resolver< AutomatType >::CreateIterator() );
        DeleteTail( ListView< UnitListView >::Display( it, this ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitListView::DisplayBy
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
void UnitListView::DisplayBy( const std::string& (kernel::AgentNature::*function)() const )
{
    Iterator< const AgentType& > it( types_.Resolver< AgentType >::CreateIterator() );
    while( it.HasMoreElements() )
    {
        const AgentType& type = it.NextElement();
        const std::string& strText = (type.GetNature().*function)();
        const QString text = strText.c_str();
        QListViewItem* parentItem = findItem( text, 0 );
        if( !parentItem )
        {
            parentItem = new QListViewItem( this );
            parentItem->setText( 0, text );
        }
        ValuedListItem* item = new ValuedListItem( parentItem );
        item->SetNamed( type );
        item->setDragEnabled( true );
    }
}

namespace
{
    QListViewItem* _FindSibling( const QString& text, QListViewItem* bro )
    {
        while( bro && bro->text( 0 ) != text )
            bro = bro->nextSibling();
        return bro;
    }
    template< typename P >
    QListViewItem* _FindItem( const QString& text, P* parent )
    {
        return parent ? _FindSibling( text, parent->firstChild() ) : 0;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitListView::CreateNaturePath
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
QListViewItem* UnitListView::CreateNaturePath( const std::string& path )
{
    const std::string::size_type pos = path.find_last_of( '/' );
    QListViewItem* parent = 0;
    QString text( path.c_str() );
    if( pos != std::string::npos )
    {
        const std::string head = path.substr( 0, pos );
        const std::string tail = path.substr( pos + 1, path.length() - pos - 1 );
        parent = CreateNaturePath( head );
        text = tail.c_str();
    }
    QListViewItem* result = 0;
    if( parent )
    {
        result = _FindItem( text, parent );
        if( ! result )
            result = new QListViewItem( parent );
    } else
    {
        result = _FindItem( text, this );
        if( ! result )
            result = new QListViewItem( this );
    }
    result->setText( 0, text );
    return result;
}

// -----------------------------------------------------------------------------
// Name: UnitListView::DisplayByNature
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
void UnitListView::DisplayByNature()
{
    Iterator< const AgentType& > it( types_.Resolver< AgentType >::CreateIterator() );
    while( it.HasMoreElements() )
    {
        const AgentType& type = it.NextElement();
        const std::string& nature = type.GetNature().GetNature();
        QListViewItem* parentItem = CreateNaturePath( nature );
        ValuedListItem* item = new ValuedListItem( parentItem );
        item->SetNamed( type );
        item->setDragEnabled( true );
    }
    Sort( firstChild() );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::Sort
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
void UnitListView::Sort( QListViewItem* item )
{
    QListViewItem* after = 0;
    while( item )
    {
        Sort( item->firstChild() );
        if( item->childCount() )
        {
            if( ! after )
            {
                QListViewItem* parent = item->parent();
                if( parent )
                {
                    parent->removeItem( item );
                    parent->insertItem( item );
                }
            }
            else
                item->moveItem( after );
            after = item;
        }
        item = item->nextSibling();
    }
}

// -----------------------------------------------------------------------------
// Name: UnitListView::Display
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void UnitListView::Display( const AutomatComposition& type, ValuedListItem* item )
{
    QString cnt;
    if( type.GetMax() != type.GetMin() )
        cnt = QString( "%1..%2" ).arg( type.GetMin() ).arg( type.GetMax() == std::numeric_limits< unsigned int >::max() ? "*" : QString::number( type.GetMax() ) );
    else
        cnt = QString::number( type.GetMin() );

    item->Set( &type.GetType(), type.GetType().GetName(), cnt );
    item->setDragEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::Display
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void UnitListView::Display( const AutomatType& type, ValuedListItem* item )
{
    item->SetNamed( type );
    item->setDragEnabled( true );
    Iterator< const AutomatComposition& > it( type.CreateIterator() );
    DeleteTail( ListView< UnitListView >::Display( it, item ) );

    const AgentType& pc = *type.GetTypePC();
    ValuedListItem* pcItem = new ValuedListItem( item );
    pcItem->Set( &pc, pc.GetName(), tr( "CP" ) );
    pcItem->setDragEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::dragObject
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
QDragObject* UnitListView::dragObject()
{
    ValuedListItem* pItem = static_cast< ValuedListItem* >( selectedItem() );
    if( !pItem )
        return 0;

    QByteArray* pBytes = new QByteArray();
    std::string mimeType;
    if( pItem->IsA< const AgentType* >() )
    {
        pBytes->setRawData( (const char*)&( pItem->GetValue< const AgentType* >() ), sizeof( AgentType* ) );
        mimeType = "csword/AgentType";
    }
    else if( pItem->IsA< const AutomatType* >() )
    {
        pBytes->setRawData( (const char*)&( pItem->GetValue< const AutomatType* >() ), sizeof( AutomatType* ) );
        mimeType = "csword/AutomatType";
    }
    if( mimeType.empty() )
        return 0;
    QStoredDrag* data = new QStoredDrag( mimeType.c_str(), this );
    data->setEncodedData( *pBytes );
    return data;
}
