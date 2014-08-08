// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogTools_h_
#define __LogTools_h_

#include <protocol/Protocol.h>

namespace kernel
{
    class Logger_ABC;
    class Entity_ABC;
}

// =============================================================================
/** @namespace log_tools
    @brief  log tools...
*/
// Created: AGE 2006-07-06
// =============================================================================
namespace log_tools
{
    template< typename T >
    const std::string& GetErrorName( const T& error )
    {
        const google::protobuf::EnumDescriptor* enumDesc = google::protobuf::GetEnumDescriptor< T >();
        return google::protobuf::internal::NameOfEnum( enumDesc, error );
    }

    template< typename T >
    const std::string& GetErrorName( int32_t error )
    {
        const google::protobuf::EnumDescriptor* enumDesc = google::protobuf::GetEnumDescriptor< T >();
        return google::protobuf::internal::NameOfEnum( enumDesc, error );
    }

    void LogAcknowledge( kernel::Logger_ABC& logger, const std::string& messageName );
    void LogAcknowledge( kernel::Logger_ABC& logger, const char* messageName );
    bool CheckAcknowledge( kernel::Logger_ABC& logger, int errorCode, const std::string& errorMessage, const std::string& messageName, bool display );
    bool CheckAcknowledge( kernel::Logger_ABC& logger, const kernel::Entity_ABC& entity, int errorCode,
                           const std::string& errorMessage, const std::string& messageName, bool display );

    template< typename T >
    bool LogAcknowledge( kernel::Logger_ABC& logger, const T& logMessage )
    {
        return LogAcknowledge( logger, 0, logMessage );
    }

    template< typename T >
    bool CheckAcknowledge( kernel::Logger_ABC& logger, const T& message, bool display = true )
    {
        const std::string& errorMessage = GetErrorName( message.error_code() );
        const std::string& messageName = message.descriptor()->name();
        return CheckAcknowledge( logger, message.error_code(), errorMessage, messageName, display );
    }

    template< typename Enum, typename T >
    bool CheckAcknowledge( kernel::Logger_ABC& logger, const T& message, bool display = true )
    {
        const std::string& errorMessage = GetErrorName< Enum >( message.error_code() );
        const std::string& messageName = message.descriptor()->name();
        return CheckAcknowledge( logger, message.error_code(), errorMessage, messageName, display );
    }

    template< typename T >
    bool CheckAcknowledge( kernel::Logger_ABC& logger, const kernel::Entity_ABC& entity, const T& message, bool display = true )
    {
        const std::string& errorMessage = GetErrorName( message.error_code() );
        const std::string& messageName = message.descriptor()->name();
        return CheckAcknowledge( logger, entity, message.error_code(), errorMessage, messageName, display );
    }

    std::string Convert( sword::OrderAck_ErrorCode code );

} // namespace

#endif // __LogTools_h_
