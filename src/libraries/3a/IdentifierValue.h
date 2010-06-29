// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IdentifierValue_h_
#define __IdentifierValue_h_

#include "Values.h"
#include "Types.h"

// =============================================================================
/** @class  IdentifierValue
    @brief  IdentifierValue
*/
// Created: AGE 2007-08-28
// =============================================================================
struct IdentifierValue : public InstantValue< NumericValue >
{

    enum { has_parameter = false };

#define SET_OID( tag )                                  \
    if( wrapper.message().has_##tag() )                 \
        Set( wrapper.message().tag().oid() );
#define SET_CONSIGN_OID( tag )                          \
    if( wrapper.message().has_##tag() )                 \
        Set( wrapper.message().tag().oid_consigne() );
#define SET_FIRE_OID( tag )                             \
    if( wrapper.message().has_##tag() )                 \
        Set( wrapper.message().tag().fire_oid() );

    //! @name Operations
    //@{
    void Receive( const MsgSimToClient& wrapper )
    {
            SET_OID         ( unit_order_ack );
            SET_OID         ( unit_creation );
            SET_OID         ( unit_attributes );
            SET_OID         ( unit_pathfind );
            SET_OID         ( unit_environment_type );
            SET_OID         ( unit_change_superior );
            SET_OID         ( decisional_state );
            SET_OID         ( debug_points );
            SET_OID         ( unit_vision_cones );
            SET_OID         ( unit_detection );
            SET_OID         ( object_detection );
            SET_OID         ( population_concentration_detection );
            SET_OID         ( population_flow_detection );
            SET_OID         ( unit_order );
            SET_CONSIGN_OID ( log_medical_handling_creation );
            SET_CONSIGN_OID ( log_medical_handling_update );
            SET_CONSIGN_OID ( log_medical_handling_destruction );
            SET_CONSIGN_OID ( log_maintenance_handling_creation );
            SET_CONSIGN_OID ( log_maintenance_handling_update );
            SET_CONSIGN_OID ( log_maintenance_handling_destruction );
            SET_CONSIGN_OID ( log_supply_handling_creation );
            SET_CONSIGN_OID ( log_supply_handling_update );
            SET_CONSIGN_OID ( log_supply_handling_destruction );
            SET_FIRE_OID    ( start_unit_fire );
            SET_FIRE_OID    ( stop_unit_fire );
            SET_FIRE_OID    ( start_population_fire );
            SET_FIRE_OID    ( stop_population_fire );
            // $$$$ AGE 2007-08-29: objets, ...
    }
    //@}
#undef SET_OID
#undef SET_CONSIGN_OID
};

#endif // __IdentifierValue_h_
