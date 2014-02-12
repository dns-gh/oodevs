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
#include "gaming/Dotation.h"
#include "gaming/Dotations.h"
#include "gaming/StaticModel.h"
#include <assert.h>

#define EPSYLON 0.01

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource constructor
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
UnitStateTableResource::UnitStateTableResource( kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel,
                                                const kernel::Time_ABC& simulation, QWidget* parent )
    : gui::UnitStateTableResource( parent, tools::translate( "UnitStateTableResource", "Maximal capacity" ) )
    , controllers_ ( controllers )
    , staticModel_ ( staticModel )
    , actionsModel_( actionsModel )
    , simulation_  ( simulation )
    , selected_    ( controllers )
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

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::ComputeCapacityAndConsumption
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
void UnitStateTableResource::ComputeCapacityAndConsumption( const std::string& name, unsigned int& capacity, double& consumption, tools::Iterator< const kernel::DotationCapacityType& > agentResourceIterator, unsigned int factor /* = 1*/ ) const
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

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::GetCapacityAndConsumption
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
std::pair< unsigned int, double > UnitStateTableResource::GetCapacityAndConsumption( const std::string& name, tools::Iterator< const kernel::DotationCapacityType& > agentResourceIterator, tools::Iterator< const kernel::AgentComposition& > agentCompositionIterator ) const
{
    unsigned int capacity = 0;
    double consumption = 0;

    ComputeCapacityAndConsumption( name, capacity, consumption, agentResourceIterator );
    while( agentCompositionIterator.HasMoreElements() )
    {
        const kernel::AgentComposition& agentComposition = agentCompositionIterator.NextElement();
        const std::string& agentName = agentComposition.GetType().GetName();
        tools::Iterator< const kernel::EquipmentType& > equipmentTypeIterator = staticModel_.objectTypes_.Resolver2< kernel::EquipmentType >::CreateIterator();
        while( equipmentTypeIterator.HasMoreElements() )
        {
            const kernel::EquipmentType& equipmentType = equipmentTypeIterator.NextElement();
            if( equipmentType.GetName() == agentName )
            {
                ComputeCapacityAndConsumption( name, capacity, consumption, equipmentType.CreateResourcesIterator(), agentComposition.GetCount() );
                break;
            }
        }
    }
    return std::make_pair< unsigned int, double >( capacity, consumption );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::NotifyUpdated
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
void UnitStateTableResource::NotifyUpdated( const kernel::Dotations_ABC& dotations )
{
    if( selected_ && selected_->Retrieve< kernel::Dotations_ABC >() == &dotations )
    {
        tools::Iterator< const Dotation& > dotationIterator = static_cast< const Dotations& >( dotations ).CreateIterator();
        if( static_cast< const Dotations& >( dotations ).Count() != static_cast< unsigned long >( dataModel_.rowCount() ) )
        {
            Purge();
            Load( *selected_.ConstCast() );
        }
        else
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

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::HasChanged
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
bool UnitStateTableResource::HasChanged( kernel::Entity_ABC& selected ) const
{
    assert( selected_ == &selected && selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    rowsChanged_.clear();
    tools::Iterator< const Dotation& > dotationIterator = static_cast< const Dotations& >( selected.Get< kernel::Dotations_ABC >() ).CreateIterator();
    while( dotationIterator.HasMoreElements() )
    {
        const Dotation& dotation = dotationIterator.NextElement();
        int rowChanged = HasDotationChanged( dotation );
        if( rowChanged != -1 )
            rowsChanged_[ dotation.type_->GetId() ] = rowChanged;
    }
    return !rowsChanged_.empty();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::Load
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void UnitStateTableResource::Load( kernel::Entity_ABC& selected )
{
    selected_ = &selected;
    assert( selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    kernel::AgentType& agent = staticModel_.types_.tools::Resolver< kernel::AgentType >::Get( static_cast< kernel::Agent_ABC& >( selected ).GetType().GetId() );
    tools::Iterator< const Dotation& > dotationIterator = static_cast< const Dotations& >( selected.Get< kernel::Dotations_ABC >() ).CreateIterator();
    while( dotationIterator.HasMoreElements() )
    {
        const Dotation& dotation = dotationIterator.NextElement();
        const std::string& name = dotation.type_->GetName();
        std::pair< unsigned int, double > capacityAndConsumption = GetCapacityAndConsumption( name, agent.CreateResourcesIterator(), agent.CreateIterator() );
        MergeLine( name.c_str(), dotation.type_->GetCategoryDisplay().c_str(), dotation.quantity_, capacityAndConsumption.first, dotation.thresholdPercentage_, capacityAndConsumption.second );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::Commit
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void UnitStateTableResource::Commit( kernel::Entity_ABC& selected ) const
{
    if( selected_ != &selected || selected.GetTypeName() != kernel::Agent_ABC::typeName_ || rowsChanged_.empty() )
        return;
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( staticModel_.types_ ).Get( "change_dotation" );
    std::unique_ptr< actions::Action_ABC > action( new actions::UnitMagicAction( actionType, controllers_.controller_, false ) );

    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
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
