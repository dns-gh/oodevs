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
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/DotationCapacityType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
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
    , typeId_     ( 0 )
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
    if( agregated_ || IsReadOnly() )
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
    double consumption = 0;
    kernel::AgentType* agent = staticModel_.types_.tools::Resolver< kernel::AgentType >::Find( typeId_ );
    if( agent )
    {
        tools::Iterator< const kernel::AgentComposition& > agentCompositionIterator = agent->CreateIterator();
        while( agentCompositionIterator.HasMoreElements() )
        {
            const kernel::AgentComposition& agentComposition = agentCompositionIterator.NextElement();
            const kernel::EquipmentType& equipmentType = staticModel_.objectTypes_.Resolver2< kernel::EquipmentType >::Get( agentComposition.GetType().GetId() );
            tools::Iterator< const kernel::DotationCapacityType& > dotationIterator = equipmentType.CreateResourcesIterator();
            while( dotationIterator.HasMoreElements() )
            {
                const kernel::DotationCapacityType& type = dotationIterator.NextElement();
                if( type.GetName() == dotation->GetName() )
                {
                    consumption += agentComposition.GetCount() * type.GetNormalizedConsumption();
                    break;
                }
            }
        }
    }
    AddLine( dotation->GetName().c_str(), dotation->GetCategoryDisplay().c_str(), 0, 0, 0, consumption );
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

namespace 
{
    int RecursiveQuantity( const kernel::Entity_ABC& entity, const QString& name, const QString& category )
    {
        int quantity = 0;
        if( entity.GetTypeName() == kernel::Agent_ABC::typeName_)
        {
            const InitialState& extension = entity.Get< InitialState >();
            for( InitialState::CIT_Resources it = extension.resources_.begin(); it != extension.resources_.end(); ++it )
                if( name == it->name_ && category == it->category_ )
                    quantity += it->number_;
        }
        else
        {
            const kernel::TacticalHierarchies& hierarchy = entity.Get< kernel::TacticalHierarchies >();
            tools::Iterator< const kernel::Entity_ABC& > it = hierarchy.CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::Entity_ABC& subEntity = it.NextElement();
                quantity += RecursiveQuantity( subEntity, name, category );
            }
        }
        return quantity;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::HasChanged
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
bool UnitStateTableResource::HasChanged( kernel::Entity_ABC& selected ) const
{
    rowChanged_.clear();
    bool ret = false;
    if( selected.GetTypeName() == kernel::Agent_ABC::typeName_ )
    {
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
    }
    else
    {
        for( int row = 0; row < dataModel_.rowCount(); ++row )
        {
            unsigned int quantity = RecursiveQuantity( selected, GetDisplayData( row, eName ), GetDisplayData( row, eCategory ) );
            if( quantity != GetUserData( row, eQuantity ).toUInt() )
            {
                ret = true;
                rowChanged_.push_back( row );
            }
        }
    }
    return ret;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::Load
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableResource::Load( kernel::Entity_ABC& selected )
{
    assert( selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    kernel::Agent_ABC& agent = static_cast< kernel::Agent_ABC& >( selected );
    typeId_ = agent.GetType().GetId();
    InitialState& extension = selected.Get< InitialState >();
    for( InitialState::CIT_Resources it = extension.resources_.begin(); it != extension.resources_.end(); ++it )
        MergeLine( it->name_, it->category_, it->number_, it->maximum_, it->threshold_, it->consumption_ );
}

namespace
{
    void RecursiveChangeResource( kernel::Entity_ABC& entity, const QString& name, const QString& category, double percentage, unsigned int& quantity, kernel::Entity_ABC*& last )
    {
        if( quantity == 0 )
            return;
        if( entity.GetTypeName() == kernel::Agent_ABC::typeName_)
        {
            InitialState& extension = entity.Get< InitialState >();
            for( InitialState::IT_Resources it = extension.resources_.begin(); it != extension.resources_.end(); ++it )
                if( name == it->name_ && category == it->category_ )
                {
                    last = &entity;
                    unsigned int newQuantity = std::min( quantity, static_cast< unsigned int >( it->maximum_ * percentage ) );
                    it->number_ = newQuantity;
                    quantity -= newQuantity;
                }
        }
        else
        {
            const kernel::TacticalHierarchies& hierarchy = entity.Get< kernel::TacticalHierarchies >();
            tools::Iterator< const kernel::Entity_ABC& > it = hierarchy.CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::Entity_ABC& subEntity = it.NextElement();
                RecursiveChangeResource( const_cast< kernel::Entity_ABC& >( subEntity ), name, category, percentage, quantity, last );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::Commit
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableResource::Commit( kernel::Entity_ABC& selected ) const
{
    if( selected.GetTypeName() == kernel::Agent_ABC::typeName_ )
    {
        InitialState& extension = selected.Get< InitialState >();
        extension.resources_.clear();

        for( int row = 0; row < dataModel_.rowCount(); ++row )
            extension.resources_.push_back( InitialStateResource( GetDisplayData( row, eName ),
                                                                    GetDisplayData( row, eCategory ),
                                                                    GetUserData( row, eQuantity ).toUInt(),
                                                                    GetUserData( row, eMaximum ).toUInt(),
                                                                    GetUserData( row, eThreshold ).toDouble(),
                                                                    GetUserData( row, eConsumption ).toDouble() ) );
    }
    else
    {
        int nResult = QMessageBox::information( const_cast< UnitStateTableResource* >( this ), "Sword", tr( "Your modifications will be applied to all sub-units of this entity, do you want to validate ?" ), QMessageBox::Yes, QMessageBox::No );
        if( nResult == QMessageBox::No )
            return;
        for( int i = 0; i < rowChanged_.size(); ++i )
        {
            int row = rowChanged_[ i ];
            const QString name = GetDisplayData( row, eName );
            const QString category = GetDisplayData( row, eCategory );
            double percentage = GetUserData( row, ePercentage ).toDouble() * 0.01;
            unsigned int quantity = GetUserData( row, eQuantity ).toUInt();
            kernel::Entity_ABC* last = 0;
            RecursiveChangeResource( selected, name, category, percentage, quantity, last );
            if( quantity > 0 && last )
            {
                InitialState& extension = last->Get< InitialState >();
                for( InitialState::IT_Resources it = extension.resources_.begin(); it != extension.resources_.end(); ++it )
                    if( name == it->name_ && category == it->category_ )
                        it->number_ += quantity;
            }
        }
        const_cast< UnitStateTableResource* >( this )->Purge();
        const_cast< UnitStateTableResource* >( this )->RecursiveLoad( selected );
    }
}
