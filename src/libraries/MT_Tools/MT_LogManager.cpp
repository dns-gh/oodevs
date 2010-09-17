// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Tools_pch.h"
#include "MT_LogManager.h"

//=============================================================================
// STATICS
//=============================================================================
MT_LogManager* MT_LogManager::pInstance_ = 0;


//-----------------------------------------------------------------------------
// Name: MT_LogManager constructor
// Created: NLD 2002-03-25
//-----------------------------------------------------------------------------
MT_LogManager::MT_LogManager()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MT_LogManager destructor
// Created: NLD 2002-03-25
//-----------------------------------------------------------------------------
MT_LogManager::~MT_LogManager()
{
    if( pInstance_ == this )
        pInstance_ = 0;
}

//=============================================================================
// LOGGER MANAGEMENT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MT_LogManager::RegisterLogger
/**
    @param  logger Logger to register to the log manager
    @return The state of the operation
*/
// Created: NLD 2002-03-25
//-----------------------------------------------------------------------------
bool MT_LogManager::RegisterLogger( MT_Logger_ABC& logger )
{
    return loggerSet_.insert( &logger ).second;    
}


//-----------------------------------------------------------------------------
// Name: MT_LogManager::UnregisterLogger
/**
    @param  logger Logger to unregister from the log manager
    @return The state of the operation
*/
// Created: NLD 2002-03-25
//-----------------------------------------------------------------------------
bool MT_LogManager::UnregisterLogger( MT_Logger_ABC& logger )
{
    return loggerSet_.erase( &logger ) == 1;
}

//=============================================================================
// LOG
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MT_LogManager::Log
/**
    @param  nLayer       Log layer
    @param  strLayerName Log layer name
    @param  nLevel       Log level
    @param  strMessage   Message
    @param  strContext   Context
    @param  nCode        Code
*/
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
void MT_LogManager::Log( uint nLayer, const char* strLayerName, MT_Logger_ABC::E_LogLevel nLevel, const char* strMessage, const char* strContext, int nCode )
{
    for( IT_LoggerSet itLogger = loggerSet_.begin(); itLogger != loggerSet_.end(); ++itLogger )
        (*itLogger)->Log( nLayer, strLayerName, nLevel, strMessage, strContext, nCode );
}
