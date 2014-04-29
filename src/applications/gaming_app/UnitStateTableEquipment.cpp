// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UnitStateTableEquipment.h"
#include "actions/UnitMagicAction.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTiming.h"
#include "actions/ActionTasker.h"
#include "actions/ParameterList.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/BreakdownOriginType.h"
#include "clients_kernel/BreakdownType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Types.h"
#include "gaming/Equipments.h"
#include "gaming/Equipment.h"
#include "gaming/StaticModel.h"
#include <assert.h>

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment constructor
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
UnitStateTableEquipment::UnitStateTableEquipment( kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel,
                                                  const kernel::Time_ABC& simulation, QWidget* parent, const kernel::Profile_ABC& profile,
                                                  gui::DisplayExtractor& extractor )
    : gui::UnitStateTableEquipment( parent, extractor, controllers )
    , controllers_ ( controllers )
    , staticModel_ ( staticModel )
    , actionsModel_( actionsModel )
    , simulation_  ( simulation )
    , profile_     ( profile )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment destructor
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
UnitStateTableEquipment::~UnitStateTableEquipment()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::CountLines
// Created: ABR 2011-08-09
// -----------------------------------------------------------------------------
int UnitStateTableEquipment::CountLines( const QString& name, int firstRow, E_EquipmentState state ) const
{
    int result = 0;
    for( int row = firstRow; row < dataModel_.rowCount() && name == GetDisplayData( row, eName ); ++row )
        if( GetEnumData< E_EquipmentState >( row, eState ) == state )
            ++result;
    return result;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::LineChanged
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
bool UnitStateTableEquipment::LineChanged( const QString& name, int& row, int size, E_EquipmentState state, const std::vector< unsigned int >& currentBreakdowns ) const
{
    for( int i = 0; row < dataModel_.rowCount() && i < size && GetDisplayData( row, eName ) == name; ++i, ++row )
        if( GetEnumData< E_EquipmentState >( row, eState ) != state ||
             ( state == eEquipmentState_RepairableWithEvacuation || state == eEquipmentState_InMaintenance ) && GetEnumData< unsigned int >( row, eBreakdown ) != ( currentBreakdowns.empty() ? 0 : currentBreakdowns[ i ] ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::BreakdownIDToComboIndex
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
std::vector< unsigned int > UnitStateTableEquipment::BreakdownIDToComboIndex( const QStringList* breakdowns, const std::vector< int >& breakdownIDs ) const
{
    std::vector< unsigned int > result;
    if( !breakdowns )
        return result;
    for( unsigned int i = 0; i < breakdownIDs.size(); ++i )
    {
        const kernel::BreakdownType& type = staticModel_.objectTypes_.Resolver2< kernel::BreakdownType >::Get( breakdownIDs[ i ] );
        const int index = breakdowns->findIndex( QString::fromStdString( type.GetName() ) );
        assert( index != -1 );
        result.push_back( static_cast< unsigned int >( index ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::BreakdownComboIndexToId
// Created: ABR 2011-08-09
// -----------------------------------------------------------------------------
unsigned int UnitStateTableEquipment::BreakdownComboIndexToId( const QStringList& breakdowns, unsigned int index ) const
{
    assert( index >= 0 && static_cast< int >( index ) < breakdowns.size() );
    if( index == 0 ) // $$$$ ABR 2011-08-09: random
        return 0;
    const kernel::BreakdownType& type = staticModel_.objectTypes_.Resolver2< kernel::BreakdownType >::Get( breakdowns[ index ].toStdString() );
    return type.GetId();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::NotifyUpdated
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
void UnitStateTableEquipment::NotifyUpdated( const kernel::Equipments_ABC& equipments )
{
    if( selected_ && selected_->Retrieve< kernel::Equipments_ABC >() == &equipments )
    {
        Purge();
        RecursiveLoad( *selected_.ConstCast(), true );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::HasChanged
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
bool UnitStateTableEquipment::HasChanged( kernel::Entity_ABC& selected ) const
{
    if( IsReadOnly() || selected.GetTypeName() != kernel::Agent_ABC::typeName_ )
        return false;

    assert( selected_ == &selected );

    rowsChanged_.clear();
    for( auto it = static_cast< const Equipments& >( selected.Get< kernel::Equipments_ABC >() ).CreateIterator(); it.HasMoreElements(); )
    {
        const Equipment& equipment = it.NextElement();
        const QString name = QString::fromStdString( equipment.type_.GetName() );
        int row = 0;
        for( ; row < dataModel_.rowCount(); ++row )
            if( GetDisplayData( row, eName ) == name )
                break;
        int first_row = row;
        const QStringList* breakdowns = delegate_.GetComboContent( row, eBreakdown );
        const std::vector< int > maintenanceDiagnosedBreakdowns = equipment.GetBreakdownsInTreatment( true );
        const std::vector< int > maintenanceUndiagnosedBreakdowns = equipment.GetBreakdownsInTreatment( false );
        if( LineChanged( name, row, equipment.available_, eEquipmentState_Available ) ||
            LineChanged( name, row, equipment.unavailable_, eEquipmentState_Destroyed ) ||
            LineChanged( name, row, equipment.repairable_, eEquipmentState_RepairableWithEvacuation,
                BreakdownIDToComboIndex( breakdowns, equipment.GetBreakdowns() ) ) ||
            LineChanged( name, row, equipment.onSiteFixable_, eEquipmentState_OnSiteFixable ) ||
            LineChanged( name, row, static_cast< int >( maintenanceDiagnosedBreakdowns.size() ), eEquipmentState_InMaintenance,
                BreakdownIDToComboIndex( breakdowns, maintenanceDiagnosedBreakdowns ) ) ||
            LineChanged( name, row, static_cast< int >( maintenanceDiagnosedBreakdowns.size() ), eEquipmentState_InMaintenance,
                BreakdownIDToComboIndex( breakdowns, maintenanceUndiagnosedBreakdowns ) ) ||
            LineChanged( name, row, equipment.prisonners_, eEquipmentState_Prisonner ) )
            rowsChanged_[ equipment.type_.GetId() ] = first_row;
    }
    return !rowsChanged_.empty();
}

namespace
{
    QStringList GetBreakdownTypes( const Equipment& equipment )
    {
        QStringList breakdownTypes;
        auto it = equipment.type_.CreateBreakdownsIterator();
        if( it.HasMoreElements() )
            breakdownTypes << tools::translate( "UnitStateTableEquipment", "Random" );
        std::set< std::string > alreadyAddedBreakdowns;
        while( it.HasMoreElements() )
        {
            const std::string& name = it.NextElement().GetName();
            if( alreadyAddedBreakdowns.find( name ) == alreadyAddedBreakdowns.end() )
            {
                alreadyAddedBreakdowns.insert( name );
                breakdownTypes << QString::fromStdString( name );
            }
        }
        return breakdownTypes;
    }

    QStringList GetUnknownBreakdowns( const QStringList& breakdowns, const kernel::Resolver2< kernel::BreakdownType >& types )
    {
        QStringList result;
        for( auto it = breakdowns.begin(); it != breakdowns.end(); ++it )
        {
            const kernel::BreakdownType* type = types.Find( it->toStdString() );
            result += !type ? *it : QString::fromStdString( type->GetUnknownName() );
        }
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::Load
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void UnitStateTableEquipment::Load( kernel::Entity_ABC& selected )
{
    assert( selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    const bool isUnknown = !profile_.CanDoMagic( selected ) && controllers_.GetCurrentMode() != eModes_Replay;
    for( auto it = static_cast< const Equipments& >( selected.Get< kernel::Equipments_ABC >() ).CreateIterator(); it.HasMoreElements(); )
    {
        const Equipment& equipment = it.NextElement();
        const QString name = equipment.type_.GetName().c_str();
        const QStringList breakdownTypes = GetBreakdownTypes( equipment );
        const QStringList unknownTypes = isUnknown ? GetUnknownBreakdowns( breakdownTypes, staticModel_.objectTypes_ ) : QStringList();
        const QStringList& displayTypes = isUnknown ? unknownTypes : breakdownTypes;
        const std::vector< int > maintenanceDiagnosedBreakdowns = equipment.GetBreakdownsInTreatment( true );
        const std::vector< int > maintenanceUndiagnosedBreakdowns = equipment.GetBreakdownsInTreatment( false );
        AddLines( name, selected, equipment.available_, eEquipmentState_Available, breakdownTypes );
        AddLines( name, selected, equipment.unavailable_, eEquipmentState_Destroyed, breakdownTypes );
        AddLines( name, selected, equipment.repairable_, eEquipmentState_RepairableWithEvacuation, displayTypes,
                  BreakdownIDToComboIndex( &breakdownTypes, equipment.GetBreakdowns() ) );
        AddLines( name, selected, equipment.onSiteFixable_, eEquipmentState_OnSiteFixable, breakdownTypes );
        AddLines( name, selected, static_cast< int >( maintenanceDiagnosedBreakdowns.size() ), eEquipmentState_InMaintenance, breakdownTypes,
                  BreakdownIDToComboIndex( &breakdownTypes, maintenanceDiagnosedBreakdowns ) );
        AddLines( name, selected, static_cast< int >( maintenanceUndiagnosedBreakdowns.size() ), eEquipmentState_InMaintenance, displayTypes,
                  BreakdownIDToComboIndex( &breakdownTypes, maintenanceUndiagnosedBreakdowns ) );
        AddLines( name, selected, equipment.prisonners_, eEquipmentState_Prisonner, breakdownTypes );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::Commit
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void UnitStateTableEquipment::Commit( kernel::Entity_ABC& selected ) const
{
    if( selected_ != &selected || selected.GetTypeName() != kernel::Agent_ABC::typeName_ )
        return;
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( staticModel_.types_ ).Get( "change_equipment_state" );
    std::unique_ptr< actions::Action_ABC > action( new actions::UnitMagicAction( actionType, controllers_.controller_, false ) );

    auto it = actionType.CreateIterator();
    actions::parameters::ParameterList* parameterList = new actions::parameters::ParameterList( it.NextElement() );
    action->AddParameter( *parameterList );

    for( auto it = rowsChanged_.begin(); it != rowsChanged_.end(); ++it )
    {
        actions::parameters::ParameterList& list = parameterList->AddList( "Equipment" );
        list.AddIdentifier( "ID", it->first );

        const QString& name = GetDisplayData( it->second, eName );
        list.AddQuantity( "NbAvailable",                CountLines( name, it->second, eEquipmentState_Available ) );
        list.AddQuantity( "NbDestroyed",                CountLines( name, it->second, eEquipmentState_Destroyed ) );
        list.AddQuantity( "NbRepairableWithEvacuation", CountLines( name, it->second, eEquipmentState_RepairableWithEvacuation ) );
        list.AddQuantity( "NbOnSiteFixable",            CountLines( name, it->second, eEquipmentState_OnSiteFixable ) );
        list.AddQuantity( "NbInMaintenance",            CountLines( name, it->second, eEquipmentState_InMaintenance ) );
        list.AddQuantity( "NbPrisonner",                CountLines( name, it->second, eEquipmentState_Prisonner ) );

        const QStringList* breakdowns = delegate_.GetComboContent( it->second, eBreakdown );
        actions::parameters::ParameterList& breakdownList = list.AddList( "Breakdowns" );
        int i = 0;
        for( int row = it->second; row < dataModel_.rowCount() && name == GetDisplayData( row, eName ); ++row )
            if( GetEnumData< E_EquipmentState >( row, eState ) == eEquipmentState_RepairableWithEvacuation )
            {
                assert( breakdowns );
                breakdownList.AddIdentifier( QString( "Breakdown %L1" ).arg( ++i ).toStdString(), BreakdownComboIndexToId( *breakdowns, GetEnumData< unsigned int >( row, eBreakdown ) ) );
            }
    }
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( controllers_.controller_, selected_, false ) );
    actionsModel_.Publish( *action );
}
