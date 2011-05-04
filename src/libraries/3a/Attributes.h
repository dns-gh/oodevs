// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Attributes_h_
#define __Attributes_h_

#include "Equipments.h"
#include "Extractors.h"
#include "Humans.h"
#include "Resources.h"
#include "UnitDetection.h"
#include "Values.h"
#include "LogMedicalEquipments.h"
#include "LogMaintenanceEquipments.h"
#include "LogSupplyStocks.h"

// =============================================================================
/** @namespace  Attributes
    @brief      Attributes
*/
// Created: AGE 2007-08-28
// =============================================================================

namespace attributes
{

// =============================================================================
/** @class  UnitAttribute
    @brief  UnitAttribute function
*/
// Created: AGE 2007-08-28
// =============================================================================
template< typename Extractor >
struct UnitAttribute : public ContinuousValue< typename Extractor::Type >
{
    enum { has_parameter = Extractor::has_parameter };

    UnitAttribute()
        : extractor_() {}
    explicit UnitAttribute( xml::xistream& xis )
        : extractor_( xis ) {}
    explicit UnitAttribute( const aar::StaticModel_ABC& model )
        : extractor_( model ) {}

    void Receive( const sword::SimToClient& wrapper )
    {
        if( wrapper.message().has_unit_attributes() )
        {
            const sword::UnitAttributes& attributes = wrapper.message().unit_attributes();
            if( extractor_.HasFlag( attributes ) )
                Set( extractor_.Extract( attributes ) );
        }
    }

    Extractor extractor_;
};

template< typename Extractor >
struct LogisticAttribute : public ContinuousValue< typename Extractor::Type >
{
    enum { has_parameter = Extractor::has_parameter };
    LogisticAttribute()
        : extractor_() {}
    explicit LogisticAttribute( xml::xistream& xis )
        : extractor_( xis ) {}
    explicit LogisticAttribute( const aar::StaticModel_ABC& model )
        : extractor_( model ) {}

    void Receive( const sword::SimToClient& wrapper )
    {
        if( extractor_.HasValue( wrapper ) )
            Set( extractor_.Extract( wrapper ) );
    }

Extractor extractor_;
};

typedef UnitAttribute< extractors::OperationalState >    OperationalState;
typedef UnitAttribute< extractors::Position >            Position;
typedef UnitAttribute< extractors::Resources >           Resources;
typedef UnitAttribute< extractors::Equipments >          Equipments;
typedef UnitAttribute< extractors::Humans >              Humans;
typedef UnitAttribute< extractors::Mounted >             Mounted;
typedef UnitAttribute< extractors::DirectFirePower >     DirectFirePower;
typedef UnitAttribute< extractors::IndirectFirePower >   IndirectFirePower;
typedef UnitAttribute< extractors::CloseCombatPower >    CloseCombatPower;
typedef UnitAttribute< extractors::EngineeringPower >    EngineeringPower;

typedef LogisticAttribute< extractors::LogMedicalEquipments > LogMedicalEquipments;
typedef LogisticAttribute< extractors::LogMaintenanceEquipments > LogMaintenanceEquipments;
typedef LogisticAttribute< extractors::LogSupplyStocks > LogSupplyStocks;

struct Detections : public ContinuousValue< extractors::UnitDetection::Type >
{
    enum { has_parameter = extractors::UnitDetection::has_parameter };
    explicit Detections( const extractors::UnitDetection& extractor = extractors::UnitDetection() )
        : extractor_( extractor ) {}
    void Receive( const sword::SimToClient& wrapper )
    {
        if( extractor_.HasValue( wrapper ) )
            Set( extractor_.Extract( wrapper ) );
    }

    extractors::UnitDetection extractor_;
};

} // namespace attributes


#endif // __Attributes_h_
