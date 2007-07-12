// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UnitIdExtractor_h_
#define __UnitIdExtractor_h_

#include "SimpleValueExtractor_ABC.h"

// =============================================================================
/** @class  UnitIdExtractor
    @brief  UnitId extractor
*/
// Created: AGE 2007-07-12
// =============================================================================
class UnitIdExtractor : public SimpleValueExtractor_ABC
{

public:
    //! @name Operations
    //@{
#define RETURN_OID( tag ) case( T_MsgsSimToClient_msg_##tag ) : return float( message.msg.u.tag->oid )
    virtual float ExtractValue( const ASN1T_MsgsSimToClient& message )
    {
        switch( message.msg.t )
        {
        RETURN_OID( msg_unit_order_ack );
        RETURN_OID( msg_unit_change_superior_ack );
        RETURN_OID( msg_unit_creation );
        RETURN_OID( msg_unit_attributes );
        RETURN_OID( msg_unit_pathfind );
//        RETURN_OID( msg_unit_destruction );
        RETURN_OID( msg_unit_environment_type );
        RETURN_OID( msg_unit_change_superior );
        RETURN_OID( msg_decisional_state );
        RETURN_OID( msg_debug_points );
        RETURN_OID( msg_unit_vision_cones );
        RETURN_OID( msg_unit_detection );
        RETURN_OID( msg_object_detection );
        RETURN_OID( msg_population_concentration_detection );
        RETURN_OID( msg_population_flow_detection );
        RETURN_OID( msg_unit_order );
        default:
            return Invalid();
        };
    }
#undef RETURN_OID
    //@}
};

#endif // __UnitIdExtractor_h_
