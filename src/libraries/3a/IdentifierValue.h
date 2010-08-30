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

#define SET_OID( tag, var )                             \
    if( wrapper.message().has_##tag() )                 \
        Set( wrapper.message().tag().##var() );

    //! @name Operations
    //@{
    void Receive( const MsgsSimToClient::MsgSimToClient& wrapper )
    {
            SET_OID( unit_order_ack, tasker().id );
            SET_OID( unit_creation, id().id );
            SET_OID( unit_attributes, id().id );
            SET_OID( unit_pathfind, id().id );
            SET_OID( unit_environment_type, id().id );
            SET_OID( unit_change_superior, unit().id );
            SET_OID( decisional_state, id().unit().id );
            SET_OID( debug_points, id().unit().id );
            SET_OID( unit_vision_cones, id().id );
            SET_OID( unit_detection, observer().id );
            SET_OID( object_detection, observer().id );
            SET_OID( population_concentration_detection, id().id );
            SET_OID( population_flow_detection, id().id );
            SET_OID( unit_order, tasker().id );
            SET_OID( log_medical_handling_creation, id().id );
            SET_OID( log_medical_handling_update, id().id );
            SET_OID( log_medical_handling_destruction, id().id );
            SET_OID( log_maintenance_handling_creation, id().id );
            SET_OID( log_maintenance_handling_update, id().id );
            SET_OID( log_maintenance_handling_destruction, id().id );
            SET_OID( log_supply_handling_creation, id().id );
            SET_OID( log_supply_handling_update, id().id );
            SET_OID( log_supply_handling_destruction, id().id );
            SET_OID( start_unit_fire, id().id );
            SET_OID( stop_unit_fire, id().id );
            SET_OID( start_population_fire, id().id );
            SET_OID( stop_population_fire, id().id );
            // $$$$ AGE 2007-08-29: objets, ...
    }
    //@}
#undef SET_OID
};

#endif // __IdentifierValue_h_
