// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_LogDefines_h_
#define __MT_LogDefines_h_

#include "MT_LogManager.h"
#include <sstream>

//=============================================================================
// Internal macros
//=============================================================================
#define _MT_LOG_FATAL_ERROR( msg, code, ctx ) MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_FatalError, msg, ctx, code );
#define _MT_LOG_FATAL_ERROR_MSG( msg )        MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_FatalError, msg );
#define _MT_LOG_ERROR( msg, code, ctx )       MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_Error,      msg, ctx, code );
#define _MT_LOG_ERROR_MSG( msg )              MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_Error,      msg );
#define _MT_LOG_WARNING( msg, code, ctx )     MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_Warning,    msg, ctx, code );
#define _MT_LOG_WARNING_MSG( msg )            MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_Warning,    msg );
#define _MT_LOG_MESSAGE( msg, code, ctx )     MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_Message,    msg, ctx, code );
#define _MT_LOG_MESSAGE_MSG( msg )            MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_Message,    msg );
#define _MT_LOG_INFO( msg, code, ctx )        MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_Info,       msg, ctx, code );
#define _MT_LOG_INFO_MSG( msg )               MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_Info,       msg );
#define _MT_LOG_VERBOSE( msg, code, ctx )     MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_Verbose,    msg, ctx, code );
#define _MT_LOG_VERBOSE_MSG( msg )            MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_Verbose,    msg );
#define _MT_LOG_DEBUG( msg, code, ctx )       MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_Debug,      msg, ctx, code );
#define _MT_LOG_DEBUG_MSG( msg )              MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_Debug,      msg );
#define _MT_LOG_STARTUP_MESSAGE( msg )        MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_None,       msg );

// Temporary string stream
#define _MT_CREATE_SSTREAM( sstr, msg ) std::stringstream sstr; sstr << msg

//=============================================================================
// LOG MACROS
//=============================================================================
// Full message
#define MT_LOG_FATAL_ERROR( msg, code, ctx ) { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_FATAL_ERROR( sstr.str().c_str(), code, ctx ) }
#define MT_LOG_ERROR( msg, code, ctx )       { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_ERROR( sstr.str().c_str(), code, ctx ) }
#define MT_LOG_WARNING( msg, code, ctx )     { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_WARNING( sstr.str().c_str(), code, ctx ) }
#define MT_LOG_MESSAGE( msg, code, ctx )     { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_MESSAGE( sstr.str().c_str(), code, ctx ) }
#define MT_LOG_INFO( msg, code, ctx )        { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_INFO( sstr.str().c_str(), code, ctx ) }
#define MT_LOG_VERBOSE( msg, code, ctx )     { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_VERBOSE( sstr.str().c_str(), code, ctx ) }
#ifdef MT_DEBUG
#   define MT_LOG_DEBUG( msg, code, ctx )    { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_DEBUG( sstr.str().c_str(), code, ctx ) }
#else
#   define MT_LOG_DEBUG( msg, code, ctx )
#endif

// Message only
#define MT_LOG_FATAL_ERROR_MSG( msg ) { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_FATAL_ERROR_MSG( sstr.str().c_str() ) }
#define MT_LOG_ERROR_MSG( msg )       { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_ERROR_MSG( sstr.str().c_str() ) }
#define MT_LOG_WARNING_MSG( msg )     { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_WARNING_MSG( sstr.str().c_str() ) }
#define MT_LOG_MESSAGE_MSG( msg )     { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_MESSAGE_MSG( sstr.str().c_str() ) }
#define MT_LOG_INFO_MSG( msg )        { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_INFO_MSG( sstr.str().c_str() ) }
#define MT_LOG_VERBOSE_MSG( msg )     { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_VERBOSE_MSG( sstr.str().c_str() ) }
#ifdef MT_DEBUG
#   define MT_LOG_DEBUG_MSG( msg )    { _MT_CREATE_SSTREAM( sstr, msg ); _MT_LOG_DEBUG_MSG( sstr.str().c_str() ) }
#else
#   define MT_LOG_DEBUG_MSG( msg )
#endif

// Misc
#define MT_LOG_STARTUP_MESSAGE( msg )          MT_LogManager::Instance().Log( 0, 0, MT_Logger_ABC::eLogLevel_None, msg );
#define MT_LOG_REGISTER_LOGGER( logger )       MT_LogManager::Instance().RegisterLogger( logger ); 
#define MT_LOG_UNREGISTER_LOGGER( logger )     MT_LogManager::Instance().UnregisterLogger( logger ); 


#endif // __MT_LogDefines_h_
