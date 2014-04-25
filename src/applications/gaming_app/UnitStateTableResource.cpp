// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UnitStateTableResource.h"
#include "actions/UnitMagicAction.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTiming.h"
#include "actions/ActionTasker.h"
#include "actions/ParameterList.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Dotations_ABC.h"
#include "clients_kernel/DotationCapacityType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "gaming/Dotation.h"
#include "gaming/Dotations.h"
#include "gaming/Equipments.h"
#include "gaming/Equipment.h"
#include "gaming/StaticModel.h"
#include <assert.h>

#define EPSYLON 0.01

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource constructor
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
UnitStateTableResource::UnitStateTableResource( const QString& objectName,
                                                kernel::Controllers& controllers,
                                                const StaticModel& staticModel,
                                                actions::ActionsModel& actionsModel,
                                                const kernel::Time_ABC& simulation,
                                                QWidget* parent )
    : gui::UnitStateTableResource( objectName, parent, tools::translate( "UnitStateTableResource", "Maximal capacity" ), controllers )
    , controllers_ ( controllers )
    , staticModel_ ( staticModel )
    , actionsModel_( actionsModel )
    , simulation_  ( simulation )
{
    delegate_.AddSpinBoxOnColumn( eQuantity, 0, std::numeric_limits< int >::max(), 10 );
    delegate_.SetColumnDependency( eQuantity, eMaximum, gui::CommonDelegate::eLTE );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource destructor
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
UnitStateTableResource::~UnitStateTableResource()
{
    controllers_.Unregister( *this );
}

namespace
{
    void ComputeCapacityAndConsumption( const std::string& name, unsigned int& capacity, double& consumption, tools::Iterator< const kernel::DotationCapacityType& > agentResourceIterator, unsigned int factor = 1 )
    {
        while( agentResourceIterator.HasMoreElements() )
        {
            const kernel::DotationCapacityType& dotation = agentResourceIterator.NextElement();
            if( dotation.GetName() == name )
            {
                capacity += dotation.GetCapacity() * factor;
                consumption += dotation.GetNormalizedConsumption() * factor;
                return;
            }
        }
    }

    std::pair< unsigned int, double > GetCapacityAndConsumption( const std::string& name, const kernel::AgentType& type, const kernel::Entity_ABC& entity )
    {
        auto agentResourceIterator = type.CreateResourcesIterator();
        auto equipmentsIterator = static_cast< const Equipments& >( entity.Get< kernel::Equipments_ABC >() ).CreateIterator();
        unsigned int capacity = 0;
        double consumption = 0;
        ComputeCapacityAndConsumption( name, capacity, consumption, agentResourceIterator );
        while( equipmentsIterator.HasMoreElements() )
        {
            const Equipment& equipment = equipmentsIterator.NextElement();
            ComputeCapacityAndConsumption( name, capacity, consumption, equipment.type_.CreateResourcesIterator(), equipment.available_ );
        }
        return std::make_pair( capacity, consumption );
    }

    template< class T >
    bool HasSubordinateWithExtension( const kernel::Entity_ABC& entity, const T& extension )
    {
        if( entity.Retrieve< T >() == &extension )
            return true;
        const kernel::TacticalHierarchies& hierarchy = entity.Get< kernel::TacticalHierarchies >();
        auto it = hierarchy.CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::Entity_ABC& subEntity = it.NextElement();
            if( HasSubordinateWithExtension( subEntity, extension ) )
                return true;
        }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::NotifyUpdated
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
void UnitStateTableResource::NotifyUpdated( const kernel::Dotations_ABC& dotations )
{
    if( selected_ && HasSubordinateWithExtension( *selected_, dotations ) )
    {
        auto dotationIterator = static_cast< const Dotations& >( dotations ).CreateIterator();
        if( selected_->GetTypeName() != kernel::Agent_ABC::typeName_ || static_cast< const Dotations& >( dotations ).Count() != static_cast< unsigned long >( dataModel_.rowCount() ) )
        {
            Purge();
            RecursiveLoad( *selected_.ConstCast(), true );
        }
        else
        {
            while( dotationIterator.HasMoreElements() )
            {
                const Dotation& dotation = dotationIterator.NextElement();
                const QString name = dotation.type_->GetName().c_str();
                for( int row = 0; row < dataModel_.rowCount(); ++row )
                    if( GetUserData( row, eName ).toString() == name )
                    {
                        SetData( row, eQuantity, locale().toString( dotation.quantity_ ), dotation.quantity_ );
                        SetData( row, eThreshold, locale().toString( dotation.thresholdPercentage_, 'f', 2 ), dotation.thresholdPercentage_ );
                        break;
                    }
            }
            emit RefreshFilters();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::NotifyUpdated
// Created: JSR 2014-02-24
// -----------------------------------------------------------------------------
void UnitStateTableResource::NotifyUpdated( const kernel::Equipments_ABC& equipments )
{
    if( selected_ && HasSubordinateWithExtension( *selected_, equipments ) )
    {
        Purge();
        RecursiveLoad( *selected_.ConstCast(), true );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::HasDotationChanged
// Created: ABR 2011-08-09
// -----------------------------------------------------------------------------
int UnitStateTableResource::HasDotationChanged( const Dotation& dotation ) const
{
    const QString name = dotation.type_->GetName().c_str();
    for( int row = 0; row < dataModel_.rowCount(); ++row )
        if( GetDisplayData( row, eName ) == name )
        {
            int quantity = GetUserData( row, eQuantity ).toInt();
            float threshold = GetUserData( row, eThreshold ).toFloat();
            if( quantity != dotation.quantity_ ||
                abs( threshold - dotation.thresholdPercentage_ ) > EPSYLON )
                return row;
            break;
        }
    return -1;
}

namespace 
{
    int RecursiveQuantity( const kernel::Entity_ABC& entity, const QString& name, unsigned long& dotationId )
    {
        int quantity = 0;
        if( entity.GetTypeName() == kernel::Agent_ABC::typeName_)
        {
            auto dotationIterator = static_cast< const Dotations& >( entity.Get< kernel::Dotations_ABC >() ).CreateIterator();
            while( dotationIterator.HasMoreElements() )
            {
                const Dotation& dotation = dotationIterator.NextElement();
                if( dotation.type_ && dotation.type_->GetName().c_str() == name )
                {
                    dotationId = dotation.type_->GetId();
                    quantity += dotation.quantity_;
                }
            }
        }
        else
        {
            const kernel::TacticalHierarchies& hierarchy = entity.Get< kernel::TacticalHierarchies >();
            auto it = hierarchy.CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::Entity_ABC& subEntity = it.NextElement();
                quantity += RecursiveQuantity( subEntity, name, dotationId );
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
    rowsChanged_.clear();
    if( selected.GetTypeName() == kernel::Agent_ABC::typeName_ )
    {
    auto dotationIterator = static_cast< const Dotations& >( selected.Get< kernel::Dotations_ABC >() ).CreateIterator();
    while( dotationIterator.HasMoreElements() )
    {
        const Dotation& dotation = dotationIterator.NextElement();
        int rowChanged = HasDotationChanged( dotation );
        if( rowChanged != -1 )
            rowsChanged_[ dotation.type_->GetId() ] = rowChanged;
    }
    }
    else
    {
        for( int row = 0; row < dataModel_.rowCount(); ++row )
        {
            unsigned long dotationId = 0;
            unsigned int quantity = RecursiveQuantity( selected, GetDisplayData( row, eName ), dotationId );
            if( quantity != GetUserData( row, eQuantity ).toUInt() )
                rowsChanged_[ dotationId ] = row;
        }
    }
    return !rowsChanged_.empty();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::Load
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void UnitStateTableResource::Load( kernel::Entity_ABC& selected )
{
    assert( selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    kernel::AgentType& agent = staticModel_.types_.tools::Resolver< kernel::AgentType >::Get( static_cast< kernel::Agent_ABC& >( selected ).GetType().GetId() );
    auto dotationIterator = static_cast< const Dotations& >( selected.Get< kernel::Dotations_ABC >() ).CreateIterator();
    while( dotationIterator.HasMoreElements() )
    {
        const Dotation& dotation = dotationIterator.NextElement();
        const std::string& name = dotation.type_->GetName();
        const auto capacityAndConsumption = GetCapacityAndConsumption( name, agent, selected );
        MergeLine( name.c_str(), dotation.type_->GetCategoryDisplay().c_str(), dotation.quantity_, capacityAndConsumption.first, dotation.thresholdPercentage_, capacityAndConsumption.second );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::RecursiveMagicAction
// Created: JSR 2012-03-22
// -----------------------------------------------------------------------------
void UnitStateTableResource::RecursiveMagicAction( kernel::Entity_ABC& entity, const QString& name, double percentage, unsigned int& quantity, kernel::Entity_ABC*& last ) const
{
    if( entity.GetTypeName() == kernel::Agent_ABC::typeName_)
    {
        auto dotationIterator = static_cast< const Dotations& >( entity.Get< kernel::Dotations_ABC >() ).CreateIterator();
        while( dotationIterator.HasMoreElements() )
        {
            const Dotation& dotation = dotationIterator.NextElement();
            if( dotation.type_ && dotation.type_->GetName().c_str() == name )
            {
                last = &entity;
                kernel::AgentType& agent = staticModel_.types_.tools::Resolver< kernel::AgentType >::Get( static_cast< kernel::Agent_ABC& >( entity ).GetType().GetId() );
                const auto capacityAndConsumption = GetCapacityAndConsumption( name.toStdString(), agent, entity );
                unsigned int newQuantity = std::min( quantity, static_cast< unsigned int >( capacityAndConsumption.first * percentage ) );
                quantity -= newQuantity;

                CreateMagicAction( newQuantity, dotation, &entity );
                break;
            }
        }
    }
    else
    {
        const kernel::TacticalHierarchies& hierarchy = entity.Get< kernel::TacticalHierarchies >();
        auto it = hierarchy.CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::Entity_ABC& subEntity = it.NextElement();
            RecursiveMagicAction( const_cast< kernel::Entity_ABC& >( subEntity ), name, percentage, quantity, last );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::CreateMagicAction
// Created: LDC 2014-02-14
// -----------------------------------------------------------------------------
void UnitStateTableResource::CreateMagicAction( unsigned int quantity, const Dotation& dotation, kernel::Entity_ABC* entity ) const
{
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( staticModel_.types_ ).Get( "change_dotation" );
    std::unique_ptr< actions::Action_ABC > action( new actions::UnitMagicAction( actionType, controllers_.controller_, true ) );

    auto it = actionType.CreateIterator();
    actions::parameters::ParameterList* parameterList = new actions::parameters::ParameterList( it.NextElement() );
    action->AddParameter( *parameterList );
    actions::parameters::ParameterList& list = parameterList->AddList( "Dotation" );
    list.AddIdentifier( "ID", dotation.type_->GetId() );
    list.AddQuantity( "Quantity", quantity );
    list.AddNumeric( "Threshold", dotation.thresholdPercentage_ );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( controllers_.controller_, entity, false ) );
    actionsModel_.Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::Commit
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void UnitStateTableResource::Commit( kernel::Entity_ABC& selected ) const
{
    if( rowsChanged_.empty() )
        return;
    if( selected.GetTypeName() == kernel::Agent_ABC::typeName_ )
    {
        kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( staticModel_.types_ ).Get( "change_dotation" );
        std::unique_ptr< actions::Action_ABC > action( new actions::UnitMagicAction( actionType, controllers_.controller_, false ) );

        auto it = actionType.CreateIterator();
        actions::parameters::ParameterList* parameterList = new actions::parameters::ParameterList( it.NextElement() );
        action->AddParameter( *parameterList );

        for( auto it = rowsChanged_.begin(); it != rowsChanged_.end(); ++it )
        {
            assert( it->second >=0 && it->second < dataModel_.rowCount() );
            actions::parameters::ParameterList& list = parameterList->AddList( "Dotation" );
            list.AddIdentifier( "ID", it->first );
            list.AddQuantity( "Quantity", GetUserData( it->second, eQuantity ).toInt() );
            list.AddNumeric( "Threshold", GetUserData( it->second, eThreshold ).toFloat() );
        }
        rowsChanged_.clear();
        action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new actions::ActionTasker( controllers_.controller_, selected_, false ) );
        actionsModel_.Publish( *action );
    }
    else
    {
        int nResult = QMessageBox::information( const_cast< UnitStateTableResource* >( this ), "Sword", tools::translate( "UnitStateTableResource", "Your modifications will be applied to all sub-units of this entity, do you want to validate ?" ), QMessageBox::Yes, QMessageBox::No );
        if( nResult == QMessageBox::No )
            return;
        for( auto it = rowsChanged_.begin(); it != rowsChanged_.end(); ++it )
        {
            int row = it->second;
            const QString name = GetDisplayData( row, eName );
            double percentage = GetUserData( row, ePercentage ).toDouble() * 0.01;
            unsigned int quantity = GetUserData( row, eQuantity ).toUInt();
            kernel::Entity_ABC* last = 0;
            RecursiveMagicAction( selected, name, percentage, quantity, last );
            if( quantity > 0 && last )
            {
                auto dotationIterator = static_cast< const Dotations& >( last->Get< kernel::Dotations_ABC >() ).CreateIterator();
                while( dotationIterator.HasMoreElements() )
                {
                    const Dotation& dotation = dotationIterator.NextElement();
                    if( dotation.type_ && dotation.type_->GetName().c_str() == name )
                    {
                        kernel::AgentType& agent = staticModel_.types_.tools::Resolver< kernel::AgentType >::Get( static_cast< kernel::Agent_ABC& >( *last ).GetType().GetId() );
                        const auto capacityAndConsumption = GetCapacityAndConsumption( name.toStdString(), agent, *last );
                        unsigned int newQuantity = static_cast< unsigned int >( capacityAndConsumption.first * percentage );
                        CreateMagicAction( newQuantity + quantity, dotation, last );
                        break;
                    }
                }
            }
        }
        rowsChanged_.clear();
    }
}
