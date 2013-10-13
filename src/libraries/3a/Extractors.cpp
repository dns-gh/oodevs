// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Extractors.h"
#include "PowerIndicator.h"
#include "StaticModel.h"
#include "protocol/Simulation.h"

namespace extractors
{

bool OperationalState::HasFlag( const sword::UnitAttributes& attributes ) const
{
    return attributes.has_raw_operational_state();
}

NumericValue OperationalState::Extract( const sword::UnitAttributes& attributes ) const
{
    return NumericValue( attributes.raw_operational_state() * 0.01f );
}

bool Positions::HasFlag( const sword::UnitAttributes& attributes ) const
{
    return attributes.has_position();
}
::Position Positions::Extract( const sword::UnitAttributes& attributes ) const
{
    return ::Position( attributes.position() );
}

bool Mounted::HasFlag( const sword::UnitAttributes& attributes ) const
{
    return attributes.has_embarked();
}
NumericValue Mounted::Extract( const sword::UnitAttributes& attributes ) const
{
    return NumericValue( attributes.embarked() ? 0.f : 1.f );
}

bool Contaminated::HasFlag( const sword::UnitAttributes& attributes ) const
{
    return attributes.has_contamination_state() && attributes.contamination_state().has_contaminated();
}
NumericValue Contaminated::Extract( const sword::UnitAttributes& attributes ) const
{
    return NumericValue( attributes.contamination_state().contaminated() ? 1.f : 0.f );
}

bool NbcDose::HasFlag( const sword::UnitAttributes& attributes ) const
{
    return attributes.has_contamination_state() && attributes.contamination_state().has_dose();
}
NumericValue NbcDose::Extract( const sword::UnitAttributes& attributes ) const
{
    return NumericValue( attributes.contamination_state().dose() );
}

// Existences
bool MaintenanceHandlingUnitId::IsCreation( const sword::SimToClient& wrapper ) const
{
    return wrapper.message().has_log_maintenance_handling_creation();
}
NumericValue MaintenanceHandlingUnitId::Extract( const sword::SimToClient& wrapper ) const
{
    return NumericValue( wrapper.message().log_maintenance_handling_creation().unit().id() );
}
bool MaintenanceHandlingUnitId::IsDestruction( const sword::SimToClient& wrapper ) const
{
    return wrapper.message().has_log_maintenance_handling_destruction();
}

bool DirectFireTargetsId::HasValue( const sword::SimToClient& wrapper ) const
{
    return wrapper.message().has_start_unit_fire() &&
        wrapper.message().start_unit_fire().type() == sword::StartUnitFire::direct &&
        wrapper.message().start_unit_fire().target().has_unit();
}
NumericValue DirectFireTargetsId::Extract( const sword::SimToClient& wrapper ) const
{
    return wrapper.message().start_unit_fire().target().unit().id();
}

bool IndirectFireTargetsPositions::HasValue( const sword::SimToClient& wrapper ) const
{
    return wrapper.message().has_start_unit_fire() &&
           wrapper.message().start_unit_fire().type() == sword::StartUnitFire::indirect &&
           wrapper.message().start_unit_fire().target().has_position();
}
::Position IndirectFireTargetsPositions::Extract( const sword::SimToClient& wrapper ) const
{
    return ::Position( wrapper.message().start_unit_fire().target().position() );
}

bool DamageIndirectFire::HasValue( const sword::SimToClient& wrapper ) const
{
    return ( wrapper.message().has_unit_damaged_by_unit_fire()
         && !wrapper.message().unit_damaged_by_unit_fire().direct_fire() );
}
NumericValue DamageIndirectFire::Extract( const sword::SimToClient& wrapper ) const
{
    return wrapper.message().unit_damaged_by_unit_fire().firer().id();
}

bool DirectFireUnitsId::HasValue( const sword::SimToClient& wrapper ) const
{
    return wrapper.message().has_start_unit_fire() &&
        wrapper.message().start_unit_fire().type() == sword::StartUnitFire::direct;
}
NumericValue DirectFireUnitsId::Extract( const sword::SimToClient& wrapper ) const
{
    return wrapper.message().start_unit_fire().firing_unit().id();
}

bool IndirectFireUnitsId::HasValue( const sword::SimToClient& wrapper ) const
{
    return wrapper.message().has_start_unit_fire() &&
        wrapper.message().start_unit_fire().type() == sword::StartUnitFire::indirect;
}
NumericValue IndirectFireUnitsId::Extract( const sword::SimToClient& wrapper ) const
{
    return wrapper.message().start_unit_fire().firing_unit().id();
}

// Power
bool PowerExtractor::HasFlag( const sword::UnitAttributes& message ) const
{
    return message.has_equipment_dotations();
}
NumericValue PowerExtractor::Extract( const sword::UnitAttributes& message )
{
    for( int i = 0; i < message.equipment_dotations().elem_size(); ++i )
    {
        int index = message.equipment_dotations().elem( i ).type().id();
        const auto& value = message.equipment_dotations().elem( i );
        equipments_[ index ] = value;
    }
    return NumericValue( model_->ComputePower( equipments_, *this ) );
}

unsigned int DirectFirePower::GetPowerValue( const aar::PowerIndicator& powerIndicator ) const
{
    return powerIndicator.GetDirectFire();
}

unsigned int IndirectFirePower::GetPowerValue( const aar::PowerIndicator& powerIndicator ) const
{
    return powerIndicator.GetIndirectFire();
}

unsigned int CloseCombatPower::GetPowerValue( const aar::PowerIndicator& powerIndicator ) const
{
    return powerIndicator.GetCloseCombat();
}

unsigned int EngineeringPower::GetPowerValue( const aar::PowerIndicator& powerIndicator ) const
{
    return powerIndicator.GetEngineering();
}

} // namespace extractor

