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

// =============================================================================
/** @namespace log_tools
    @brief  log tools...
*/
// Created: AGE 2006-07-06
// =============================================================================
namespace log_tools
{
    bool CheckAcknowledge( int errorCode, const char* logMessage );

    inline
    bool CheckAcknowledge( const char* logMessage ) {
        return CheckAcknowledge( 1, logMessage );
    }

    template< typename T >
    bool CheckAcknowledge( const T& message, const char* logMessage, void(T::*)(void) = 0 )
    {
        return CheckAcknowledge( message.error_code, logMessage );
    }

} // namespace

#endif // __LogTools_h_
