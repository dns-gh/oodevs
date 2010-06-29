// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "Equipments.h"

using namespace extractors;

using namespace Common;

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
    typedef int (EquipmentDotations_EquipmentDotation::* EquipmentDotationsMemberFn)() const;
    EquipmentDotationsMemberFn equipmentData[5] =
    {
        &EquipmentDotations_EquipmentDotation::nb_disponibles,
        &EquipmentDotations_EquipmentDotation::nb_indisponibles,
        &EquipmentDotations_EquipmentDotation::nb_reparables,
        &EquipmentDotations_EquipmentDotation::nb_dans_chaine_maintenance,
        &EquipmentDotations_EquipmentDotation::nb_prisonniers
    };
    const char* equipmentStates[5] =
    {
        "available",
        "unavailable",
        "repairable",
        "repairing",
        "prisoner"
    };
    int ReadMask( xml::xistream& xis )
    {
        if( xml::attribute< std::string >( xis, "states" ).empty() )
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
    : filter_( xis, "equipments" )
    , stateMask_( ReadMask( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Equipments::Extract
// Created: AGE 2007-10-29
// -----------------------------------------------------------------------------
int Equipments::Extract( const MsgUnitAttributes& attributes )
{
    unsigned size = attributes.dotation_eff_materiel().elem_size();
    while( size > 0 )
    {
        --size;
        const EquipmentDotations_EquipmentDotation& equipment = attributes.dotation_eff_materiel().elem( size );
        if( filter_.IsAllowed( equipment.type_equipement() ) )
        {
            int quantity = 0;
            EquipmentDotationsMemberFn* data = equipmentData;
            for( unsigned i = 0; i < nEquipmentStates; ++i, ++data )
                if( ( stateMask_ & ( 1 << i ) ) != 0 )
                    quantity += (equipment.**data)();
            equipments_[ equipment.type_equipement() ] = quantity;
        }
    }
    int result = 0;
    for( std::map< int, int >::const_iterator it = equipments_.begin(); it != equipments_.end(); ++it )
        result += it->second;
    return result;
}
