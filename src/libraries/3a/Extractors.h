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
#include "Types.h"

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
        enum { has_parameter = false };
        typedef T Type;
    };

    // Attributes
    struct OperationalState : public Extractor< NumericValue >
    {
        bool HasFlag( const MsgsSimToClient::MsgUnitAttributes& attributes ) const
        {
            return attributes.has_etat_operationnel_brut();
        }
        NumericValue Extract( const MsgsSimToClient::MsgUnitAttributes& attributes ) const
        {
            return NumericValue( attributes.etat_operationnel_brut() * 0.01f );
        }
    };

    struct Position : public Extractor< ::Position >
    {
        bool HasFlag( const MsgsSimToClient::MsgUnitAttributes& attributes ) const
        {
            return attributes.has_position();
        }
        ::Position Extract( const MsgsSimToClient::MsgUnitAttributes& attributes ) const
        {
            return ::Position( attributes.position() );
        }
    };

    struct Mounted : public Extractor< NumericValue >
    {
        bool HasFlag( const MsgsSimToClient::MsgUnitAttributes& attributes ) const
        {
            return attributes.has_embarque();
        }
        NumericValue Extract( const MsgsSimToClient::MsgUnitAttributes& attributes ) const
        {
            return NumericValue( attributes.embarque() ? 0.f : 1.f );
        }
    };

    // Existences
    struct MaintenanceHandlingUnitId : public Extractor< NumericValue >
    {
        bool IsCreation( const MsgsSimToClient::MsgSimToClient& wrapper ) const
        {
            return wrapper.message().has_log_maintenance_handling_creation();
        }
        NumericValue Extract( const MsgsSimToClient::MsgSimToClient& wrapper ) const
        { 
            return NumericValue( wrapper.message().log_maintenance_handling_creation().unit().id() ); 
        }
        bool IsDestruction( const MsgsSimToClient::MsgSimToClient& wrapper ) const
        {
            return wrapper.message().has_log_maintenance_handling_destruction();
        }
    };

    struct DirectFireUnitId : public Extractor< NumericValue >
    {
        bool IsCreation( const MsgsSimToClient::MsgSimToClient& wrapper ) const
        {
            return wrapper.message().has_start_unit_fire() &&
                   wrapper.message().start_unit_fire().type() == Common::direct;
        }
        NumericValue Extract( const MsgsSimToClient::MsgSimToClient& wrapper ) const
        { 
            return NumericValue( wrapper.message().start_unit_fire().firing_unit().id() );
        }
        bool IsDestruction( const MsgsSimToClient::MsgSimToClient& wrapper ) const
        {
            return wrapper.message().has_stop_unit_fire();
        }
    };

    struct IndirectFireUnitId : public Extractor< NumericValue >
    {
        bool IsCreation( const MsgsSimToClient::MsgSimToClient& wrapper ) const
        {
            return wrapper.message().has_start_unit_fire() &&
                   wrapper.message().start_unit_fire().type() == Common::indirect;
        }
        NumericValue Extract( const MsgsSimToClient::MsgSimToClient& wrapper ) const
        { 
            return NumericValue( wrapper.message().start_unit_fire().firing_unit().id() );
        }
        bool IsDestruction( const MsgsSimToClient::MsgSimToClient& wrapper ) const
        {
            return wrapper.message().has_stop_unit_fire();
        }
    };

}

#endif // __Extractors_h_
