// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: MT_LogManager::Instance
/**
    @return The unique instance of the log manager
*/
// Created: NLD 2002-03-25
//-----------------------------------------------------------------------------
inline
MT_LogManager& MT_LogManager::Instance()
{
    if( pInstance_ == 0 )
        pInstance_ = new MT_LogManager();
    return *pInstance_;
}
