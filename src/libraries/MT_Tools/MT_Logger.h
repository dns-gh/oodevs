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
#define _MT_LOG_FATAL_ERROR( msg, code, ctx ) MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_FatalError, msg, ctx, code )
#define _MT_LOG_FATAL_ERROR_MSG( msg )        MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_FatalError, msg )
#define _MT_LOG_ERROR( msg, code, ctx )       MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_Error,      msg, ctx, code )
#define _MT_LOG_ERROR_MSG( msg )              MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_Error,      msg )
#define _MT_LOG_WARNING( msg, code, ctx )     MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_Warning,    msg, ctx, code )
#define _MT_LOG_WARNING_MSG( msg )            MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_Warning,    msg )
#define _MT_LOG_MESSAGE( msg, code, ctx )     MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_Message,    msg, ctx, code )
#define _MT_LOG_MESSAGE_MSG( msg )            MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_Message,    msg )
#define _MT_LOG_INFO( msg, code, ctx )        MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_Info,       msg, ctx, code )
#define _MT_LOG_INFO_MSG( msg )               MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_Info,       msg )
#define _MT_LOG_VERBOSE( msg, code, ctx )     MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_Verbose,    msg, ctx, code )
#define _MT_LOG_VERBOSE_MSG( msg )            MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_Verbose,    msg )
#define _MT_LOG_DEBUG( msg, code, ctx )       MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_Debug,      msg, ctx, code )
#define _MT_LOG_DEBUG_MSG( msg )              MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_Debug,      msg )
#define _MT_LOG_STARTUP_MESSAGE( msg )        MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_None,       msg )

// Temporary string stream
#define _MT_STREAM( msg ) static_cast< std::stringstream& >( std::stringstream() << msg ).str().c_str()

//=============================================================================
// LOG MACROS
//=============================================================================
// Full message
#define MT_LOG_FATAL_ERROR( msg, code, ctx ) _MT_LOG_FATAL_ERROR( _MT_STREAM( msg ), code, ctx )
#define MT_LOG_ERROR( msg, code, ctx )       _MT_LOG_ERROR( _MT_STREAM( msg ), code, ctx )
#define MT_LOG_WARNING( msg, code, ctx )     _MT_LOG_WARNING( _MT_STREAM( msg ), code, ctx )
#define MT_LOG_MESSAGE( msg, code, ctx )     _MT_LOG_MESSAGE( _MT_STREAM( msg ), code, ctx )
#define MT_LOG_INFO( msg, code, ctx )        _MT_LOG_INFO( _MT_STREAM( msg ), code, ctx )
#define MT_LOG_VERBOSE( msg, code, ctx )     _MT_LOG_VERBOSE( _MT_STREAM( msg ), code, ctx )
#ifdef MT_DEBUG
#   define MT_LOG_DEBUG( msg, code, ctx )    _MT_LOG_DEBUG( _MT_STREAM( msg ), code, ctx )
#else
#   define MT_LOG_DEBUG( msg, code, ctx )
#endif

// Message only
#define MT_LOG_FATAL_ERROR_MSG( msg ) _MT_LOG_FATAL_ERROR_MSG( _MT_STREAM( msg ) )
#define MT_LOG_ERROR_MSG( msg )       _MT_LOG_ERROR_MSG( _MT_STREAM( msg ) )
#define MT_LOG_WARNING_MSG( msg )     _MT_LOG_WARNING_MSG( _MT_STREAM( msg ) )
#define MT_LOG_MESSAGE_MSG( msg )     _MT_LOG_MESSAGE_MSG( _MT_STREAM( msg ) )
#define MT_LOG_INFO_MSG( msg )        _MT_LOG_INFO_MSG( _MT_STREAM( msg ) )
#define MT_LOG_VERBOSE_MSG( msg )     _MT_LOG_VERBOSE_MSG( _MT_STREAM( msg ) )
#ifdef MT_DEBUG
#   define MT_LOG_DEBUG_MSG( msg )    _MT_LOG_DEBUG_MSG( _MT_STREAM( msg ) )
#else
#   define MT_LOG_DEBUG_MSG( msg )
#endif

// Misc
#define MT_LOG_STARTUP_MESSAGE( msg )      MT_LogManager::Instance().Log( MT_Logger_ABC::eLogLevel_None, _MT_STREAM( msg ) )
#define MT_LOG_REGISTER_LOGGER( logger )   MT_LogManager::Instance().RegisterLogger( logger )
#define MT_LOG_UNREGISTER_LOGGER( logger ) MT_LogManager::Instance().UnregisterLogger( logger )


#endif // __MT_LogDefines_h_
