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

#include "IdentifierValue_ABC.h"

// =============================================================================
/** @class  IdentifierValue
    @brief  IdentifierValue
*/
// Created: AGE 2007-08-28
// =============================================================================
struct IdentifierValue : public IdentifierValue_ABC
{

    enum { has_parameter = false };

#define SET_OID( tag, var )                             \
    if( wrapper.message().has_##tag() )                 \
        Set( wrapper.message().tag().##var() );

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& wrapper )
    {
        if( wrapper.message().has_order_ack() && wrapper.message().order_ack().tasker().has_unit() )
            Set( wrapper.message().order_ack().tasker().unit().id() );
        SET_OID( unit_creation, unit().id );
        SET_OID( unit_attributes, unit().id );
        SET_OID( unit_pathfind, unit().id );
        SET_OID( unit_environment_type, unit().id );
        SET_OID( unit_change_superior, unit().id );
        SET_OID( unit_vision_cones, unit().id );
        SET_OID( unit_detection, observer().id );
        SET_OID( object_detection, observer().id );
        SET_OID( crowd_concentration_detection, observer().id );
        SET_OID( crowd_flow_detection, observer().id );
        SET_OID( unit_order, tasker().id );
        SET_OID( log_medical_handling_creation, request().id );
        SET_OID( log_medical_handling_update, unit().id );
        SET_OID( log_medical_handling_destruction, request().id );
        SET_OID( log_medical_state, unit().id );
        SET_OID( log_maintenance_state, unit().id );
        SET_OID( log_supply_state, unit().id );
        SET_OID( log_maintenance_handling_creation, request().id );
        SET_OID( log_maintenance_handling_update, request().id );
        SET_OID( log_maintenance_handling_destruction, request().id );
        SET_OID( log_supply_handling_creation, request().id );
        SET_OID( log_supply_handling_update, request().id );
        SET_OID( log_supply_handling_destruction, request().id );
        SET_OID( start_unit_fire, fire().id );
        SET_OID( stop_unit_fire, fire().id );
        SET_OID( start_crowd_fire, fire().id );
        SET_OID( stop_crowd_fire, fire().id );
        SET_OID( population_update, id().id );
        SET_OID( crowd_concentration_update, crowd().id );
        SET_OID( urban_update, object().id );
        // $$$$ AGE 2007-08-29: objets, ...
    }
    //@}
#undef SET_OID
};

#endif // __IdentifierValue_h_
