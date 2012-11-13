// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::UnitListView */

#include "clients_gui_pch.h"
#include "UnitListView.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentNature.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatComposition.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "ValuedDragObject.h"
#include "resources.h"
#include "Tools.h"

#pragma warning( disable : 4355 )

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitListView constructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
UnitListView::UnitListView( QWidget* parent, Controllers& controllers, const AgentTypes& list, ItemFactory_ABC& factory, bool followSelection /* = false */ )
    : ListView< UnitListView >( parent, *this, factory )
    , controllers_    ( controllers )
    , types_          ( list )
    , selectedAgent_  ( controllers )
    , selectedAutomat_( controllers )
    , followSelection_( followSelection )
{
    setMinimumSize( 1, 1 );
    addColumn( tools::translate( "gui::UnitListView", "Unit type" ) );
    addColumn( tools::translate( "gui::UnitListView", "Count" ) );
    setRootIsDecorated( true );
    header()->hide();
    setSorting( 0 );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitListView destructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
UnitListView::~UnitListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::viewportResizeEvent
// Created: SBO 2007-11-09
// -----------------------------------------------------------------------------
void UnitListView::viewportResizeEvent( QResizeEvent* e )
{
    Q3ScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::setColumnWidth
// Created: SBO 2007-11-09
// -----------------------------------------------------------------------------
void UnitListView::setColumnWidth( int column, int w )
{
    Q3ListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) : w );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::SetOpen
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
void UnitListView::SetOpen( bool open )
{
    for( Q3ListViewItem* item = firstChild(); item; item = item->nextSibling() )
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
        tools::Iterator< const AutomatType& > it( types_.Resolver< AutomatType >::CreateIterator() );
        DeleteTail( ListView< UnitListView >::Display( it, this ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitListView::DisplayBy
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
void UnitListView::DisplayBy( const std::string& (kernel::AgentNature::*function)() const )
{
    tools::Iterator< const AgentType& > it( types_.Resolver< AgentType >::CreateIterator() );
    while( it.HasMoreElements() )
    {
        const AgentType& type = it.NextElement();
        const std::string& strText = (type.GetNature().*function)();
        const QString text = strText.c_str();
        Q3ListViewItem* parentItem = findItem( text, 0 );
        if( !parentItem )
        {
            parentItem = new Q3ListViewItem( this );
            parentItem->setText( 0, text );
        }
        ValuedListItem* item = new ValuedListItem( parentItem );
        item->SetNamed( type );
        item->setDragEnabled( true );
        tools::Iterator< const AgentComposition& > it( type.CreateIterator() );
        DeleteTail( ListView< UnitListView >::Display( it, item ) );
    }
}

namespace
{
    Q3ListViewItem* _FindSibling( const QString& text, Q3ListViewItem* bro )
    {
        while( bro && bro->text( 0 ) != text )
            bro = bro->nextSibling();
        return bro;
    }
    template< typename P >
    Q3ListViewItem* _FindItem( const QString& text, P* parent )
    {
        return parent ? _FindSibling( text, parent->firstChild() ) : 0;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitListView::CreateNaturePath
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
Q3ListViewItem* UnitListView::CreateNaturePath( const std::string& path )
{
    const std::string::size_type pos = path.find_last_of( '/' );
    Q3ListViewItem* parent = 0;
    QString text( path.c_str() );
    if( pos != std::string::npos )
    {
        const std::string head = path.substr( 0, pos );
        const std::string tail = path.substr( pos + 1, path.length() - pos - 1 );
        parent = CreateNaturePath( head );
        text = tail.c_str();
    }
    Q3ListViewItem* result = 0;
    if( parent )
    {
        result = _FindItem( text, parent );
        if( ! result )
            result = new Q3ListViewItem( parent );
    } else
    {
        result = _FindItem( text, this );
        if( ! result )
            result = new Q3ListViewItem( this );
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
    tools::Iterator< const AgentType& > it( types_.Resolver< AgentType >::CreateIterator() );
    while( it.HasMoreElements() )
    {
        const AgentType& type = it.NextElement();
        const std::string& nature = type.GetNature().GetNature();
        Q3ListViewItem* parentItem = CreateNaturePath( nature );
        ValuedListItem* item = new ValuedListItem( parentItem );
        item->SetNamed( type );
        item->setDragEnabled( true );
        tools::Iterator< const AgentComposition& > it( type.CreateIterator() );
        DeleteTail( ListView< UnitListView >::Display( it, item ) );
    }
    Sort( firstChild() );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::Sort
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
void UnitListView::Sort( Q3ListViewItem* item )
{
    Q3ListViewItem* after = 0;
    while( item )
    {
        Sort( item->firstChild() );
        if( item->childCount() )
        {
            if( ! after )
            {
                Q3ListViewItem* parent = item->parent();
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
// Created: SBO 2007-11-09
// -----------------------------------------------------------------------------
void UnitListView::Display( const AgentComposition& type, ValuedListItem* item )
{
    item->SetFontColor( QColor( 100, 100, 100 ) );
    item->Set( &type, type.GetType().GetName().c_str(), locale().toString( type.GetCount() ) );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::Display
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void UnitListView::Display( const AutomatComposition& type, ValuedListItem* item )
{
    QString cnt;
    if( type.GetMax() != type.GetMin() )
        cnt = QString( "%L1..%L2" ).arg( type.GetMin() ).arg( type.GetMax() == std::numeric_limits< unsigned int >::max() ? "*" : locale().toString( type.GetMax() ) );
    else
        cnt = locale().toString( type.GetMin() );

    tools::Iterator< const AgentComposition& > it( type.GetType().CreateIterator() );
    DeleteTail( ListView< UnitListView >::Display( it, item ) );

    item->Set( &type.GetType(), type.GetType().GetName().c_str(), cnt );
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
    tools::Iterator< const AutomatComposition& > it( type.CreateIterator() );
    DeleteTail( ListView< UnitListView >::Display( it, item ) );

    if( ValuedListItem* pcItem = FindChild( type.GetTypePC(), item ) )
        pcItem->setPixmap( 0, MAKE_PIXMAP( commandpost ) );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::dragObject
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
Q3DragObject* UnitListView::dragObject()
{
    ValuedListItem* pItem = dynamic_cast< ValuedListItem* >( selectedItem() );
    if( !pItem )
        return 0;

    if( pItem->IsA< const AgentType >() )
        return new ValuedDragObject( pItem->GetValueNoCheck< const AgentType >(), this );
    else if( pItem->IsA< const AutomatType >() )
        return new ValuedDragObject( pItem->GetValueNoCheck< const AutomatType >(), this );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UnitListView::BeforeSelection
// Created: ABR 2012-04-03
// -----------------------------------------------------------------------------
void UnitListView::BeforeSelection()
{
    if( !followSelection_ )
        return;
    selectedAgent_ = 0;
    selectedAutomat_ = 0;
}

// -----------------------------------------------------------------------------
// Name: UnitListView::Select
// Created: ABR 2012-04-03
// -----------------------------------------------------------------------------
void UnitListView::Select( const kernel::Agent_ABC& agent )
{
    if( !followSelection_ )
        return;
    selectedAgent_ = &agent;
}

// -----------------------------------------------------------------------------
// Name: UnitListView::Select
// Created: ABR 2012-04-03
// -----------------------------------------------------------------------------
void UnitListView::Select( const kernel::Automat_ABC& automat )
{
    if( !followSelection_ )
        return;
    selectedAutomat_ = &automat;
}

// -----------------------------------------------------------------------------
// Name: UnitListView::AfterSelection
// Created: ABR 2012-04-03
// -----------------------------------------------------------------------------
void UnitListView::AfterSelection()
{
    if( !followSelection_ )
        return;
    if( selectedAgent_ && !sorting_.empty() )
        FindAndSelect( selectedAgent_->GetType().GetName().c_str() );
    else if( selectedAutomat_ && sorting_.empty() )
        FindAndSelect( selectedAutomat_->GetType().GetName().c_str() );
    else if( selectedAgent_ && sorting_.empty() )
    {
        const kernel::TacticalHierarchies* hierarchy = selectedAgent_->Retrieve< kernel::TacticalHierarchies >();
        const kernel::Automat_ABC* automat = static_cast< const kernel::Automat_ABC* >( hierarchy->GetSuperior() );
        if( automat )
            FindAndSelect( automat->GetType().GetName().c_str() );
    }
}
