// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SIM_Dispatcher_h_
#define __SIM_Dispatcher_h_

#include "dispatcher_dll/DispatcherFacade.h"

class SIM_Config;

// =============================================================================
/** @class  SIM_Dispatcher
    @brief  SIM_Dispatcher
*/
// Created: NLD 2006-10-04
// =============================================================================
class SIM_Dispatcher 
{
public:
    //! @name Constructors/Destructor
    //@{
             SIM_Dispatcher( int argc, char** argv );
    virtual ~SIM_Dispatcher();
    //@}

    //! @name Operations 
    //@{
    void Run();
    void Stop(); 
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SIM_Dispatcher( const SIM_Dispatcher& );            //!< Copy constructor
    SIM_Dispatcher& operator=( const SIM_Dispatcher& ); //!< Assignement operator
    //@}

private:

    //! @name 
    //@{
    DispatcherFacade               dispatcher_;    //!< embedded dispatcher 
    bool                           bRunning_ ;  //!< running status 
    //@}
};


#endif // __SIM_Dispatcher_h_
