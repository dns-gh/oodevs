// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_CrashHandler_h_
#define __MT_CrashHandler_h_

// =============================================================================
// Created: NLD 2006-03-17
// =============================================================================
class MT_CrashHandler
{
public:
    //! @name Handlers
    //@{
    static int ContinueSearch   ( const void* exceptionInfos );
    static int ExecuteHandler   ( const void* exceptionInfos );
    static int ContinueExecution( const void* exceptionInfos );
    //@}
};

#endif // __MT_CrashHandler_h_
