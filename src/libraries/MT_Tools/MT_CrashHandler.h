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

#include <windows.h>

// =============================================================================
// Created: NLD 2006-03-17
// =============================================================================
class MT_CrashHandler
{
public:
    //! @name Handlers
    //@{
    static int ContinueSearch   ( EXCEPTION_POINTERS* pExceptionPointers );
    static int ExecuteHandler   ( EXCEPTION_POINTERS* pExceptionPointers );
    static int ContinueExecution( EXCEPTION_POINTERS* pExceptionPointers );
    //@}
};

#endif // __MT_CrashHandler_h_
