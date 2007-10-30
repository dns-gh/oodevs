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
    ASN1INT ASN1T_EquipmentDotations::*equipmentData[5] = 
    {
        &ASN1T_EquipmentDotations::nb_disponibles, 
        &ASN1T_EquipmentDotations::nb_indisponibles, 
        &ASN1T_EquipmentDotations::nb_reparables, 
        &ASN1T_EquipmentDotations::nb_dans_chaine_maintenance, 
        &ASN1T_EquipmentDotations::nb_prisonniers
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
int Equipments::Extract( const ASN1T_MsgUnitAttributes& attributes )
{
    unsigned size = attributes.dotation_eff_materiel.n;
    while( size > 0 )
    {
        --size;
        const ASN1T_EquipmentDotations& equipment = attributes.dotation_eff_materiel.elem[ size ];
        if( filter_.IsAllowed( equipment.type_equipement ) )
        {
            int quantity = 0;
            ASN1INT ASN1T_EquipmentDotations::** data = equipmentData;
            for( unsigned i = 0; i < nEquipmentStates; ++i, ++data )
                if( ( stateMask_ & ( 1 << i ) ) != 0 )
                    quantity += equipment.**data;
            equipments_[ equipment.type_equipement ] = quantity;
        }
    }
    int result = 0;
    for( std::map< int, int >::const_iterator it = equipments_.begin(); it != equipments_.end(); ++it )
        result += it->second;
    return result;
}
