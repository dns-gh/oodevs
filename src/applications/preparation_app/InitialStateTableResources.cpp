// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "InitialStateTableResources.h"
#include "moc_InitialStateTableResources.cpp"
#include "clients_gui/SpinTableItem.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/tools.h"
#include "preparation/StaticModel.h"
#include "preparation/InitialState.h"
#include "preparation/InitialStateResource.h"

// -----------------------------------------------------------------------------
// Name: InitialStateTableResources constructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
InitialStateTableResources::InitialStateTableResources( QWidget* parent, const StaticModel& staticModel, const char* name /*= 0*/ )
: InitialStateTable_ABC( 0, 4, parent, name )
, staticModel_( staticModel )
{
    horizontalHeader()->setLabel( eName, tools::translate( "InitialStateTableResources", "Resources" ) );
    horizontalHeader()->setLabel( eCategory, tools::translate( "InitialStateTableResources", "Category" ) );
    horizontalHeader()->setLabel( eNumber, tools::translate( "InitialStateTableResources", "Quantity" ) );
    horizontalHeader()->setLabel( eThreshold, tools::translate( "InitialStateTableResources", "Logistic threshold (%)" ) );
    setLeftMargin( 0 );
    setColumnStretchable( eName, true );
    setColumnReadOnly( eName, true );
    setColumnReadOnly( eCategory, true );

    connect( this, SIGNAL( contextMenuRequested( int, int, const QPoint& ) ), SLOT( OnRequestContextMenu( int, int, const QPoint& ) ) );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableResources destructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
InitialStateTableResources::~InitialStateTableResources()
{
    // NOTHING
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: Local Helpers
    // Created: ABR 2011-03-01
    // -----------------------------------------------------------------------------
    typedef std::pair< QString, int >   T_MenuItem;
    typedef std::vector< T_MenuItem >   T_MenuItemVector;
    typedef T_MenuItemVector::iterator IT_MenuItemVector;
    struct ItemSort
    {
        bool operator()( T_MenuItem& lhs, T_MenuItem& rhs )
        {
            return lhs.first.lower() < rhs.first.lower();
        }
    };
    // -----------------------------------------------------------------------------
    // Name: SortMenu
    // Created: ABR 2011-03-01
    // -----------------------------------------------------------------------------
    void SortMenu( Q3PopupMenu& menu )
    {
        T_MenuItemVector vItems;
        while( menu.count() > 0 )
        {
            T_MenuItem item;
            item.first = menu.text( menu.idAt( 0 ) );
            item.second = menu.idAt( 0 );
            menu.removeItemAt( 0 );
            vItems.push_back( item );
        }
        std::sort( vItems.begin(), vItems.end(), ItemSort() );
        for( IT_MenuItemVector it = vItems.begin(); it != vItems.end(); ++it )
            menu.insertItem( (*it).first, (*it).second );
    }
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableResources::OnRequestContextMenu
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialStateTableResources::OnRequestContextMenu( int row, int col, const QPoint& pos )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu targetMenu( &menu );
    std::map< std::string, Q3PopupMenu* > categoryMap;
    tools::Iterator< const kernel::DotationType& > dotationIterator = staticModel_.objectTypes_.kernel::Resolver2< kernel::DotationType >::CreateIterator();

    while( dotationIterator.HasMoreElements() )
    {
        const kernel::DotationType& dotation = dotationIterator.NextElement();
        if( IsDotationAlreadyPresent( dotation.GetName().c_str() ) )
            continue;
        if( categoryMap.find( dotation.GetCategory() ) == categoryMap.end() )
            categoryMap[ dotation.GetCategory() ] = new Q3PopupMenu( &targetMenu );
        categoryMap[ dotation.GetCategory() ]->insertItem( dotation.GetName().c_str(), dotation.GetId() );
    }
    for( std::map< std::string, Q3PopupMenu* >::iterator it = categoryMap.begin(); it != categoryMap.end(); ++it )
    {
        SortMenu( *it->second );
        targetMenu.insertItem( it->first.c_str(), it->second );
    }

    menu.insertItem( tools::translate( "InitialStateTableResources", "Add resource"), &targetMenu, 0 );
    if( isSelected( row, col ) )
        menu.insertItem( tools::translate( "InitialStateTableResources", "Remove resource" ), this, SLOT( OnRemoveCurrentItem() ) );
    int nMenuResult = menu.exec( pos );
    if( nMenuResult > 0 )
        OnAddItem( nMenuResult );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableResources::IsDotationAlreadyPresent
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
bool InitialStateTableResources::IsDotationAlreadyPresent( const QString& name ) const
{
    for( int nRow = 0; nRow < numRows(); ++nRow )
        if( item( nRow, eName )->text() == name )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableResources::keyPressEvent
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialStateTableResources::keyPressEvent( QKeyEvent * e )
{
    if( e->key() == Qt::Key_Delete )
        OnRemoveCurrentItem();
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableResources::OnAddItem
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialStateTableResources::OnAddItem( int id )
{
    const kernel::DotationType* dotation = staticModel_.objectTypes_.kernel::Resolver2< kernel::DotationType >::Find( id );
    assert( dotation != 0 );
    AddLine( dotation->GetName().c_str(), dotation->GetCategory().c_str() );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableResources::OnRemoveItem
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialStateTableResources::OnRemoveCurrentItem()
{
    removeRow( currentRow() );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableResources::FillResources
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialStateTableResources::AddLine( const QString& name, const QString& category, unsigned number /*= 0*/, double threshold /*= 0.*/ )
{
    int nRow = numRows();
    insertRows( nRow );
    setText( nRow, eName, name );
    setText( nRow, eCategory, category );
    setItem( nRow, eNumber, new gui::SpinTableItem< int >( this, 0, std::numeric_limits< int >::max(), 10 ) );
    item( nRow, eNumber )->setText( QString::number( number ) );
    setItem( nRow, eThreshold, new gui::SpinTableItem< double >( this, 0, 100, 5 ) );
    static_cast< gui::SpinTableItem< double >* >( item( nRow, eThreshold ) )->setText( QString::number( threshold, 'f', 2 ) );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableResources::ReadExtension
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialStateTableResources::ReadExtension( const InitialState& extension )
{
    while( numRows() )
        removeRow( 0 );
    for( InitialState::CIT_Resources it = extension.resources_.begin(); it != extension.resources_.end(); ++it )
        AddLine( it->name_, it->category_, it->number_, it->threshold_ );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableResources::WriteExtension
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialStateTableResources::WriteExtension( InitialState& extension ) const
{
    extension.resources_.clear();
    for( int nRow = 0; nRow < numRows(); ++nRow )
    {
        unsigned int number = item( nRow, eNumber )->text().toUInt();
        if( !number )
            continue;
        extension.resources_.push_back( InitialStateResource( item( nRow, eName )->text(), item( nRow, eCategory )->text(), number, item( nRow, eThreshold )->text().toDouble() ) );
    }
}
