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
#include "clients_kernel/ContextMenu.h"
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
    void SortMenu( kernel::ContextMenu& menu )
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
UnitStateTableResource::UnitStateTableResource( QWidget* parent, const StaticModel& staticModel )
    : gui::UnitStateTableResource( parent, tools::translate( "UnitStateTableResource", "Default capacity" ) )
    , staticModel_( staticModel )
{
    delegate_.AddSpinBoxOnColumn( eQuantity, 0, std::numeric_limits< int >::max(), 10 );
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
// Name: UnitStateTableResource::contextMenuEvent
// Created: ABR 2011-10-04
// -----------------------------------------------------------------------------
void UnitStateTableResource::contextMenuEvent( QContextMenuEvent* e )
{
    if( IsReadOnly() )
        return;
    kernel::ContextMenu menu( this );
    kernel::ContextMenu targetMenu( &menu );
    std::map< std::string, kernel::ContextMenu* > categoryMap;
    tools::Iterator< const kernel::DotationType& > dotationIterator = staticModel_.objectTypes_.kernel::Resolver2< kernel::DotationType >::CreateIterator();

    while( dotationIterator.HasMoreElements() )
    {
        const kernel::DotationType& dotation = dotationIterator.NextElement();
        if( IsDotationAlreadyPresent( dotation.GetName().c_str() ) )
            continue;
        if( categoryMap.find( dotation.GetCategoryDisplay() ) == categoryMap.end() )
            categoryMap[ dotation.GetCategoryDisplay() ] = new kernel::ContextMenu( &targetMenu );
        categoryMap[ dotation.GetCategoryDisplay() ]->insertItem( dotation.GetName().c_str(), dotation.GetId() );
    }
    for( std::map< std::string, kernel::ContextMenu* >::iterator it = categoryMap.begin(); it != categoryMap.end(); ++it )
    {
        SortMenu( *it->second );
        targetMenu.insertItem( it->first.c_str(), it->second );
    }
    menu.insertItem( tools::translate( "UnitStateTableResource", "Add resource"), &targetMenu, 0 );
    QModelIndex index = indexAt( e->pos() );
    if( index.isValid() )
    {
        setCurrentIndex( index );
        menu.insertItem( tools::translate( "UnitStateTableResource", "Remove resource" ), this, SLOT( OnRemoveCurrentItem() ) );
    }
    int nMenuResult = menu.exec( e->globalPos() );
    if( nMenuResult > 0 )
        AddItem( nMenuResult );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::keyPressEvent
// Created: ABR 2011-10-04
// -----------------------------------------------------------------------------
void UnitStateTableResource::keyPressEvent( QKeyEvent * e )
{
    if( !IsReadOnly() && e->key() == Qt::Key_Delete && currentIndex().isValid() )
        OnRemoveCurrentItem();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::OnRemoveCurrentItem
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTableResource::OnRemoveCurrentItem()
{
    QModelIndex index = proxyModel_.mapToSource( currentIndex() );
    if( index.isValid() )
        dataModel_.removeRow( index.row() );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::AddItem
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTableResource::AddItem( int id )
{
    const kernel::DotationType* dotation = staticModel_.objectTypes_.kernel::Resolver2< kernel::DotationType >::Find( id );
    assert( dotation != 0 );
    AddLine( dotation->GetName().c_str(), dotation->GetCategoryDisplay().c_str() );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::IsDotationAlreadyPresent
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
bool UnitStateTableResource::IsDotationAlreadyPresent( const QString& name ) const
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
        if( GetDisplayData( row, eName ) == name )
            return true;
    return false;
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
    if( extension.resources_.size() != static_cast< unsigned int >( dataModel_.rowCount() ) )
        return true;
    for( InitialState::CIT_Resources it = extension.resources_.begin(); it != extension.resources_.end(); ++it )
    {
        for( int row = 0; row < dataModel_.rowCount(); ++row )
            if( GetDisplayData( row, eName ) == it->name_ &&
                GetDisplayData( row, eCategory ) == it->category_ )
            {
                if( it->number_ != GetUserData( row, eQuantity ).toUInt() ||
                    it->maximum_ != GetUserData( row, eMaximum ).toUInt() ||
                    it->threshold_ != GetUserData( row, eThreshold ).toDouble() )
                    return true;
                break;
            }
    }
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
        MergeLine( it->name_, it->category_, it->number_, it->maximum_, it->threshold_, it->consumption_ );
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

    for( int row = 0; row < dataModel_.rowCount(); ++row )
        if( GetUserData( row, eQuantity ).toUInt() != 0 )
            extension.resources_.push_back( InitialStateResource( GetDisplayData( row, eName ),
                                                                  GetDisplayData( row, eCategory ),
                                                                  GetUserData( row, eQuantity ).toUInt(),
                                                                  GetUserData( row, eMaximum ).toUInt(),
                                                                  GetUserData( row, eThreshold ).toDouble(),
                                                                  GetUserData( row, eConsumption ).toDouble() ) );
}
