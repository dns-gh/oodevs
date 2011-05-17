// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Extractors_h_
#define __Extractors_h_

#include "Position.h"
#include "PowerIndicator.h"
#include "PowerIndicators.h"
#include "StaticModel.h"
#include "Types.h"

// =============================================================================
/** @namespace Extractors
    @brief     Extractors
*/
// Created: AGE 2007-08-30
// =============================================================================
namespace extractors
{
    template< typename T >
    struct Extractor
    {
        enum { has_parameter = false };
        typedef T Type;
    };

    // Attributes
    struct OperationalState : public Extractor< NumericValue >
    {
        bool HasFlag( const sword::UnitAttributes& attributes ) const
        {
            return attributes.has_raw_operational_state();
        }
        NumericValue Extract( const sword::UnitAttributes& attributes ) const
        {
            return NumericValue( attributes.raw_operational_state() * 0.01f );
        }
    };

    struct Position : public Extractor< ::Position >
    {
        bool HasFlag( const sword::UnitAttributes& attributes ) const
        {
            return attributes.has_position();
        }
        ::Position Extract( const sword::UnitAttributes& attributes ) const
        {
            return ::Position( attributes.position() );
        }
    };

    struct Mounted : public Extractor< NumericValue >
    {
        bool HasFlag( const sword::UnitAttributes& attributes ) const
        {
            return attributes.has_embarked();
        }
        NumericValue Extract( const sword::UnitAttributes& attributes ) const
        {
            return NumericValue( attributes.embarked() ? 0.f : 1.f );
        }
    };

    // Existences
    struct MaintenanceHandlingUnitId : public Extractor< NumericValue >
    {
        bool IsCreation( const sword::SimToClient& wrapper ) const
        {
            return wrapper.message().has_log_maintenance_handling_creation();
        }
        NumericValue Extract( const sword::SimToClient& wrapper ) const
        {
            return NumericValue( wrapper.message().log_maintenance_handling_creation().unit().id() );
        }
        bool IsDestruction( const sword::SimToClient& wrapper ) const
        {
            return wrapper.message().has_log_maintenance_handling_destruction();
        }
    };

    struct DirectFireUnitId : public Extractor< NumericValue >
    {
        bool IsCreation( const sword::SimToClient& wrapper ) const
        {
            return wrapper.message().has_start_unit_fire() &&
                wrapper.message().start_unit_fire().type() == sword::StartUnitFire::direct;
        }
        NumericValue Extract( const sword::SimToClient& wrapper ) const
        {
            return NumericValue( wrapper.message().start_unit_fire().firing_unit().id() );
        }
        bool IsDestruction( const sword::SimToClient& wrapper ) const
        {
            return wrapper.message().has_stop_unit_fire();
        }
    };

    struct IndirectFireUnitId : public Extractor< NumericValue >
    {
        bool IsCreation( const sword::SimToClient& wrapper ) const
        {
            return wrapper.message().has_start_unit_fire() &&
                   wrapper.message().start_unit_fire().type() == sword::StartUnitFire::indirect;
        }
        NumericValue Extract( const sword::SimToClient& wrapper ) const
        {
            return NumericValue( wrapper.message().start_unit_fire().firing_unit().id() );
        }
        bool IsDestruction( const sword::SimToClient& wrapper ) const
        {
            return wrapper.message().has_stop_unit_fire();
        }
    };

    // Power
    struct PowerExtractor_ABC
    {
    public:
        virtual unsigned int GetPowerValue( const aar::PowerIndicator& powerIndicator ) const = 0;

    protected:
        std::map< int, sword::EquipmentDotations_EquipmentDotation > equipments_;
    };

    struct PowerExtractor : public Extractor< NumericValue >
                          , public PowerExtractor_ABC
    {
        explicit PowerExtractor( const aar::StaticModel_ABC& model ) : model_( &model ) {}
        const aar::StaticModel_ABC* model_;

        bool HasFlag( const sword::UnitAttributes& message ) const
        {
            return message.has_equipment_dotations();
        }
        NumericValue Extract( const sword::UnitAttributes& message )
        {
            for( int i = 0; i < message.equipment_dotations().elem_size(); ++i )
            {
                int index = message.equipment_dotations().elem( i ).type().id();
                const sword::EquipmentDotations_EquipmentDotation& value = message.equipment_dotations().elem( i );
                equipments_[ index ] = value;
            }
            return NumericValue( model_->ComputePower( equipments_, *this ) );
        }
    };

    struct DirectFirePower : public PowerExtractor
    {
        explicit DirectFirePower( const aar::StaticModel_ABC& model ) : PowerExtractor( model ) {}

    private:
        virtual unsigned int GetPowerValue( const aar::PowerIndicator& powerIndicator ) const
        {
            return powerIndicator.GetDirectFire();
        }
    };

    struct IndirectFirePower : public PowerExtractor
    {
        explicit IndirectFirePower( const aar::StaticModel_ABC& model ) : PowerExtractor( model ) {}

    private:
        virtual unsigned int GetPowerValue( const aar::PowerIndicator& powerIndicator ) const
        {
            return powerIndicator.GetIndirectFire();
        }
    };

    struct CloseCombatPower : public PowerExtractor
    {
        explicit CloseCombatPower( const aar::StaticModel_ABC& model ) : PowerExtractor( model ) {}

    private:
        virtual unsigned int GetPowerValue( const aar::PowerIndicator& powerIndicator ) const
        {
            return powerIndicator.GetCloseCombat();
        }
    };

    struct EngineeringPower : public PowerExtractor
    {
        explicit EngineeringPower( const aar::StaticModel_ABC& model ) : PowerExtractor( model ) {}

    private:
        virtual unsigned int GetPowerValue( const aar::PowerIndicator& powerIndicator ) const
        {
            return powerIndicator.GetEngineering();
        }
    };

} // namespace extractor

#endif // __Extractors_h_
