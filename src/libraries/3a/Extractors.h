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
}

#endif // __Extractors_h_
