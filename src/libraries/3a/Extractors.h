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
#include "Types.h"
#include <map>

namespace aar
{
    class PowerIndicator;
    class StaticModel_ABC;
}

namespace sword
{
    class EquipmentDotations_EquipmentDotation;
    class SimToClient;
    class UnitAttributes;
}

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
        bool HasFlag( const sword::UnitAttributes& attributes ) const;
        NumericValue Extract( const sword::UnitAttributes& attributes ) const;
    };

    struct Positions : public Extractor< ::Position >
    {
        bool HasFlag( const sword::UnitAttributes& attributes ) const;
        ::Position Extract( const sword::UnitAttributes& attributes ) const;
    };

    struct Mounted : public Extractor< NumericValue >
    {
        bool HasFlag( const sword::UnitAttributes& attributes ) const;
        NumericValue Extract( const sword::UnitAttributes& attributes ) const;
    };

    struct Contaminated : public Extractor< NumericValue >
    {
        bool HasFlag( const sword::UnitAttributes& attributes ) const;
        NumericValue Extract( const sword::UnitAttributes& attributes ) const;
    };

    struct NbcDose : public Extractor< NumericValue >
    {
        bool HasFlag( const sword::UnitAttributes& attributes ) const;
        NumericValue Extract( const sword::UnitAttributes& attributes ) const;
    };

    // Existences
    struct MaintenanceHandlingUnitId : public Extractor< NumericValue >
    {
        bool IsCreation( const sword::SimToClient& wrapper ) const;
        NumericValue Extract( const sword::SimToClient& wrapper ) const;
        bool IsDestruction( const sword::SimToClient& wrapper ) const;
    };

    struct DirectFireTargetsId : public Extractor< NumericValue >
    {
        bool HasValue( const sword::SimToClient& wrapper ) const;
        NumericValue Extract( const sword::SimToClient& wrapper ) const;
    };

    struct IndirectFireTargetsPositions : public Extractor< ::Position >
    {
        bool HasValue( const sword::SimToClient& wrapper ) const;
        ::Position Extract( const sword::SimToClient& wrapper ) const;
    };

    struct DamageIndirectFire : public Extractor< NumericValue >
    {
        bool HasValue( const sword::SimToClient& wrapper ) const;
        NumericValue Extract( const sword::SimToClient& wrapper ) const;
    };

    struct DirectFireUnitsId : public Extractor< NumericValue >
    {
        bool HasValue( const sword::SimToClient& wrapper ) const;
        NumericValue Extract( const sword::SimToClient& wrapper ) const;
    };

    struct IndirectFireUnitsId : public Extractor< NumericValue >
    {
        bool HasValue( const sword::SimToClient& wrapper ) const;
        NumericValue Extract( const sword::SimToClient& wrapper ) const;
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

        bool HasFlag( const sword::UnitAttributes& message ) const;
        NumericValue Extract( const sword::UnitAttributes& message );
    };

    struct DirectFirePower : public PowerExtractor
    {
        explicit DirectFirePower( const aar::StaticModel_ABC& model ) : PowerExtractor( model ) {}

    private:
        virtual unsigned int GetPowerValue( const aar::PowerIndicator& powerIndicator ) const;
    };

    struct IndirectFirePower : public PowerExtractor
    {
        explicit IndirectFirePower( const aar::StaticModel_ABC& model ) : PowerExtractor( model ) {}

    private:
        virtual unsigned int GetPowerValue( const aar::PowerIndicator& powerIndicator ) const;
    };

    struct CloseCombatPower : public PowerExtractor
    {
        explicit CloseCombatPower( const aar::StaticModel_ABC& model ) : PowerExtractor( model ) {}

    private:
        virtual unsigned int GetPowerValue( const aar::PowerIndicator& powerIndicator ) const;
    };

    struct EngineeringPower : public PowerExtractor
    {
        explicit EngineeringPower( const aar::StaticModel_ABC& model ) : PowerExtractor( model ) {}

    private:
        virtual unsigned int GetPowerValue( const aar::PowerIndicator& powerIndicator ) const;
    };

} // namespace extractor

#endif // __Extractors_h_
