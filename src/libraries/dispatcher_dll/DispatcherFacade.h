// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DispatcherFacade_h_
#define __DispatcherFacade_h_

#include "config.h"
#include <memory>

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace dispatcher
{
    class Config;
    class Dispatcher;
}

// =============================================================================
/** @class  DispatcherFacade
    @brief  Dispatcher facade
*/
// Created: AGE 2008-05-21
// =============================================================================
class DISPATCHER_DLL_API DispatcherFacade
{

public:
    //! @name Constructors/Destructor
    //@{
             DispatcherFacade( int argc, char** argv );
    virtual ~DispatcherFacade();
    //@}

    //! @name Operations
    //@{
    void Update();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< dispatcher::Config >     config_;
    std::auto_ptr< dispatcher::Dispatcher > dispatcher_;
    //@}
};

#pragma warning( pop )

#endif // __DispatcherFacade_h_
