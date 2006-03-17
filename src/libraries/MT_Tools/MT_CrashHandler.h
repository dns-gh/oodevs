// *****************************************************************************
//
// $Created: NLD 2006-03-17 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __MT_CrashHandler_h_
#define __MT_CrashHandler_h_

#include <ostream>

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
