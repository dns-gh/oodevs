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

// =============================================================================
/** @class  IdentifierValue
    @brief  IdentifierValue
*/
// Created: AGE 2007-08-28
// =============================================================================
struct IdentifierValue : public InstantValue< unsigned long >
{

#define SET_OID( tag )                                  \
    case( T_MsgsSimToClient_msg_##tag ):                \
        Set( message.msg.u.tag->oid ); break;

    //! @name Operations
    //@{
    void Receive( const ASN1T_MsgsSimToClient& message )
    {
        switch( message.msg.t )
        {
            SET_OID( msg_unit_order_ack );
            SET_OID( msg_unit_change_superior_ack );
            SET_OID( msg_unit_creation );
            SET_OID( msg_unit_attributes );
            SET_OID( msg_unit_pathfind );
            SET_OID( msg_unit_environment_type );
            SET_OID( msg_unit_change_superior );
            SET_OID( msg_decisional_state );
            SET_OID( msg_debug_points );
            SET_OID( msg_unit_vision_cones );
            SET_OID( msg_unit_detection );
            SET_OID( msg_object_detection );
            SET_OID( msg_population_concentration_detection );
            SET_OID( msg_population_flow_detection );
            SET_OID( msg_unit_order );
            // $$$$ AGE 2007-08-29: objets, ...
        };
    }
    //@}
#undef SET_OID
};

#endif // __IdentifierValue_h_
