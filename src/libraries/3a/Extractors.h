// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Extractors_h_
#define __Extractors_h_

#include "Position.h"

// =============================================================================
/** @namespace Extractors
    @brief     Extractors
*/
// Created: AGE 2007-08-30
// =============================================================================
namespace extractors
{
    template< typename T >
    struct Extractor
    {
        typedef T Type;
    };

    // Attributes
    struct OperationalState : public Extractor< float >
    {
        bool  HasFlag( const ASN1T_MsgUnitAttributes& attributes ) const
        { return attributes.m.etat_operationnel_brutPresent; }
        float Extract( const ASN1T_MsgUnitAttributes& attributes ) const
        { return float( attributes.etat_operationnel_brut ) * 0.01f; }
    };

    struct Position : public Extractor< ::Position >
    {
        bool  HasFlag( const ASN1T_MsgUnitAttributes& attributes ) const
        { return attributes.m.positionPresent; }
        ::Position Extract( const ASN1T_MsgUnitAttributes& attributes ) const
        { return ::Position( attributes.position ); }
    };

    // Existences
    struct MaintenanceHandlingUnitId : public Extractor< unsigned long >
    {
        bool IsCreation( const ASN1T_MsgsSimToClient& message ) const
        { return message.msg.t == T_MsgsSimToClient_msg_msg_log_maintenance_handling_creation; }
        unsigned long Extract( const ASN1T_MsgsSimToClient& message ) const
        { return message.msg.u.msg_log_maintenance_handling_creation->oid_pion; }
        bool IsDestruction( const ASN1T_MsgsSimToClient& message ) const
        { return message.msg.t == T_MsgsSimToClient_msg_msg_log_maintenance_handling_destruction; }
    };

    struct DirectFireUnitId : public Extractor< unsigned long >
    {
        bool IsCreation( const ASN1T_MsgsSimToClient& message ) const
        { return message.msg.t == T_MsgsSimToClient_msg_msg_start_unit_fire
              && message.msg.u.msg_start_unit_fire->type == MsgStartUnitFire_type::direct; }
        unsigned long Extract( const ASN1T_MsgsSimToClient& message ) const
        { return message.msg.u.msg_start_unit_fire->firer_oid; }
        bool IsDestruction( const ASN1T_MsgsSimToClient& message ) const
        { return message.msg.t == T_MsgsSimToClient_msg_msg_stop_unit_fire; }
    };

    // Events
    struct FireComponentDamage : public Extractor< float >
    {
        bool HasValue( const ASN1T_MsgsSimToClient& message ) const
        { return message.msg.t == T_MsgsSimToClient_msg_msg_stop_unit_fire; }
        float Extract( const ASN1T_MsgsSimToClient& message ) const
        { 
            const ASN1T_MsgStopUnitFire& stop = *message.msg.u.msg_stop_unit_fire;
            float result = 0;
            for( unsigned u = 0; u < stop.units_damages.n; ++u )
            {
                const ASN1T_UnitFireDamages& damages = stop.units_damages.elem[u];
                for( unsigned e = 0; e < damages.equipments.n; ++e )
                {
                    const ASN1T_UnitEquipmentFireDamage& damage = damages.equipments.elem[e];
                    result += damage.unavailable_nbr;
                }
            }
            return result;
        }
    };
}

#endif // __Extractors_h_
