// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Equipments.h"

using namespace extractors;
using namespace sword;

// -----------------------------------------------------------------------------
// Name: Equipments constructor
// Created: AGE 2007-10-29
// -----------------------------------------------------------------------------
Equipments::Equipments()
{
    // NOTHING
}

namespace
{
    const unsigned nEquipmentStates = 5;
    typedef int( sword::EquipmentDotations_EquipmentDotation::* EquipmentDotationsMemberFn )() const;
    EquipmentDotationsMemberFn equipmentData[5] =
    {
        &sword::EquipmentDotations_EquipmentDotation::available,
        &sword::EquipmentDotations_EquipmentDotation::unavailable,
        &sword::EquipmentDotations_EquipmentDotation::repairable,
        &sword::EquipmentDotations_EquipmentDotation::repairing,
        &sword::EquipmentDotations_EquipmentDotation::captured
    };
    const char* equipmentStates[5] =
    {
        "available",
        "unavailable",
        "repairable",
        "repairing",
        "prisoner" // $$$$ MCO rename to captured
    };
    int ReadMask( xml::xistream& xis )
    {
        if( xis.attribute< std::string >( "states" ).empty() )
            return 1;
        FilterHelper< std::string > states( xis, "states" );
        int result = 0;
        for( unsigned i = 0; i < nEquipmentStates; ++i )
            if( states.IsAllowed( equipmentStates[i] ) )
                result |= ( 1 << i );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: Equipments constructor
// Created: AGE 2007-10-29
// -----------------------------------------------------------------------------
Equipments::Equipments( xml::xistream& xis )
    : filter_   ( xis, "equipments" )
    , stateMask_( ReadMask( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Equipments::Extract
// Created: AGE 2007-10-29
// -----------------------------------------------------------------------------
int Equipments::Extract( const sword::UnitAttributes& attributes )
{
    unsigned int size = attributes.equipment_dotations().elem_size();
    while( size > 0 )
    {
        --size;
        const sword::EquipmentDotations_EquipmentDotation& equipment = attributes.equipment_dotations().elem( size );
        if( filter_.IsAllowed( equipment.type().id() ) )
        {
            int quantity = 0;
            EquipmentDotationsMemberFn* data = equipmentData;
            for( unsigned i = 0; i < nEquipmentStates; ++i, ++data )
                if( ( stateMask_ & ( 1 << i ) ) != 0 )
                    quantity += (equipment.**data)();
            equipments_[ equipment.type().id() ] = quantity;
        }
    }
    int result = 0;
    for( std::map< int, int >::const_iterator it = equipments_.begin(); it != equipments_.end(); ++it )
        result += it->second;
    return result;
}
