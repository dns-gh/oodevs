// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UnitStateTableResource.h"
#include "moc_UnitStateTableResource.cpp"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "preparation/InitialState.h"
#include "preparation/InitialStateResource.h"
#include "preparation/StaticModel.h"

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
// Name: UnitStateTableResource constructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTableResource::UnitStateTableResource( const StaticModel& staticModel, QWidget* parent, const char* name /*= 0*/ )
    : gui::UnitStateTableResource( parent, true, name )
    , staticModel_( staticModel )
{
    connect( this, SIGNAL( contextMenuRequested( int, int, const QPoint& ) ), SLOT( OnRequestContextMenu( int, int, const QPoint& ) ) );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource destructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTableResource::~UnitStateTableResource()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::keyPressEvent
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTableResource::keyPressEvent( QKeyEvent * e )
{
    if( !isReadOnly() && e->key() == Qt::Key_Delete )
        OnRemoveCurrentItem();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::AddItem
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTableResource::AddItem( int id )
{
    const kernel::DotationType* dotation = staticModel_.objectTypes_.kernel::Resolver2< kernel::DotationType >::Find( id );
    assert( dotation != 0 );
    AddLine( dotation->GetName().c_str(), dotation->GetCategory().c_str() );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::IsDotationAlreadyPresent
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
bool UnitStateTableResource::IsDotationAlreadyPresent( const QString& name ) const
{
    for( int nRow = 0; nRow < numRows(); ++nRow )
        if( item( nRow, eName )->text() == name )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::OnRemoveCurrentItem
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTableResource::OnRemoveCurrentItem()
{
    removeRow( currentRow() );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::OnRequestContextMenu
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTableResource::OnRequestContextMenu( int row, int col, const QPoint& pos )
{
    if( isReadOnly() )
        return;
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
    menu.insertItem( tr( "Add resource"), &targetMenu, 0 );
    if( isSelected( row, col ) )
        menu.insertItem( tr( "Remove resource" ), this, SLOT( OnRemoveCurrentItem() ) );
    int nMenuResult = menu.exec( pos );
    if( nMenuResult > 0 )
        AddItem( nMenuResult );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::HasChanged
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
bool UnitStateTableResource::HasChanged( kernel::Entity_ABC& selected ) const
{
    if( selected.GetTypeName() != kernel::Agent_ABC::typeName_ )
        return false;

    InitialState& extension = selected.Get< InitialState >();
    if( extension.resources_.size() != static_cast< unsigned int >( numRows() ) )
        return true;
    int nRow = 0;
    for( InitialState::CIT_Resources it = extension.resources_.begin(); it != extension.resources_.end() && nRow < numRows(); ++it, ++nRow )
        if( it->name_ != item( nRow, eName )->text() ||
            it->category_ != item( nRow, eCategory )->text() ||
            it->number_ != GetNumericValue< unsigned int >( nRow, eQuantity ) ||
            it->maximum_ != GetNumericValue< unsigned int >( nRow, eMaximum ) ||
            it->threshold_ != GetNumericValue< double >( nRow, eThreshold ) )
        return true;

    return false;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::Load
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableResource::Load( kernel::Entity_ABC& selected )
{
    assert( selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    InitialState& extension = selected.Get< InitialState >();
    for( InitialState::CIT_Resources it = extension.resources_.begin(); it != extension.resources_.end(); ++it )
        MergeLine( it->name_, it->category_, it->number_, it->maximum_, it->threshold_ );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::Commit
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableResource::Commit( kernel::Entity_ABC& selected ) const
{
    assert( selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    InitialState& extension = selected.Get< InitialState >();
    extension.resources_.clear();
    for( int nRow = 0; nRow < numRows(); ++nRow )
    {
        unsigned int number = GetNumericValue< unsigned int >( nRow, eQuantity );
        if( !number )
            continue;
        extension.resources_.push_back( InitialStateResource( item( nRow, eName )->text(),
                                                              item( nRow, eCategory )->text(),
                                                              number,
                                                              GetNumericValue< unsigned int >( nRow, eMaximum ),
                                                              GetNumericValue< double >( nRow, eThreshold ) ) );
    }
}
