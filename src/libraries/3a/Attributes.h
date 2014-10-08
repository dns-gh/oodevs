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
#include "PopulationStates.h"
#include "LogMedicalWaitingAttention.h"
#include "DPRESatisfaction.h"
#include "PopulationSatisfaction.h"
#include "PopulationResourcesSatisfaction.h"
#include "PopulationUrbanOccupation.h"
#include "StructuralStates.h"
#include "ResourcesNetworksFunctionalStates.h"
#include "CrowdStates.h"
#include "ForceRatioValue.h"

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
struct Attribute : public ContinuousValue< typename Extractor::Type >
{
    enum { has_parameter = Extractor::has_parameter };
    Attribute()
        : extractor_() {}
    explicit Attribute( xml::xistream& xis )
        : extractor_( xis ) {}
    explicit Attribute( const aar::StaticModel_ABC& model )
        : extractor_( model ) {}

    void Receive( const sword::SimToClient& wrapper )
    {
        if( extractor_.HasValue( wrapper ) )
            Set( extractor_.Extract( wrapper ) );
    }
Extractor extractor_;
};

typedef UnitAttribute< extractors::OperationalState >              OperationalState;
typedef UnitAttribute< extractors::Positions >                     Positions;
typedef UnitAttribute< extractors::Resources >                     Resources;
typedef UnitAttribute< extractors::Equipments >                    Equipments;
typedef UnitAttribute< extractors::Humans >                        Humans;
typedef UnitAttribute< extractors::Mounted >                       Mounted;
typedef UnitAttribute< extractors::DirectFirePower >               DirectFirePower;
typedef UnitAttribute< extractors::IndirectFirePower >             IndirectFirePower;
typedef UnitAttribute< extractors::CloseCombatPower >              CloseCombatPower;
typedef UnitAttribute< extractors::EngineeringPower >              EngineeringPower;
typedef UnitAttribute< extractors::ForceRatioValue >               ForceRatioValue;
typedef UnitAttribute< extractors::Contaminated>                   Contaminated;
typedef UnitAttribute< extractors::NbcDose >                       NbcDose;

typedef Attribute< extractors::LogMedicalEquipments >              LogMedicalEquipments;
typedef Attribute< extractors::LogMedicalWaitingAttention >        LogMedicalWaitingAttention;
typedef Attribute< extractors::LogMaintenanceEquipments >          LogMaintenanceEquipments;
typedef Attribute< extractors::LogSupplyStocks >                   LogSupplyStocks;
typedef Attribute< extractors::PopulationStates >                  PopulationStates;
typedef Attribute< extractors::PopulationSatisfaction >            PopulationSatisfaction;
typedef Attribute< extractors::PopulationResourcesSatisfaction >   PopulationResourcesSatisfaction;
typedef Attribute< extractors::DPRESatisfaction >                  DPRESatisfaction;
typedef Attribute< extractors::PopulationUrbanOccupation >         PopulationUrbanOccupation;
typedef Attribute< extractors::CrowdStates >                       CrowdStates;
typedef Attribute< extractors::StructuralStates >                  StructuralStates;
typedef Attribute< extractors::ResourcesNetworksFunctionalStates > ResourcesNetworksFunctionalStates;

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
