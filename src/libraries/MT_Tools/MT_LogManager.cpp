// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_LogManager.h"

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

//-----------------------------------------------------------------------------
// Name: MT_LogManager::Instance()
// Created: NLD 2002-03-25
//-----------------------------------------------------------------------------
MT_LogManager& MT_LogManager::Instance()
{
    if( pInstance_ == 0 )
        pInstance_ = new MT_LogManager();
    return *pInstance_;
}

//-----------------------------------------------------------------------------
// Name: MT_LogManager::Cleanup
// Created: BAX 2012-11-20
//-----------------------------------------------------------------------------
void MT_LogManager::Cleanup()
{
    delete pInstance_;
    pInstance_ = 0;
}

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

//-----------------------------------------------------------------------------
// Name: MT_LogManager::Log
/**
    @param  nLevel       Log level
    @param  strMessage   Message
    @param  strContext   Context
    @param  nCode        Code
*/
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
void MT_LogManager::Log( MT_Logger_ABC::E_LogLevel nLevel, const char* strMessage, const char* strContext, int nCode )
{
    for( IT_LoggerSet itLogger = loggerSet_.begin(); itLogger != loggerSet_.end(); ++itLogger )
        (*itLogger)->Log( nLevel, strMessage, strContext, nCode );
}
