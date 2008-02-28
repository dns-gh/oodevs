// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Application_h_
#define __Application_h_

#include <string>
#include <memory>
#include "dispatcher/Config.h"

namespace dispatcher
{
    class Dispatcher;
}

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: NLD 2006-10-10
// =============================================================================
class Application
{

public:
    //! @name Constructors/Destructor
    //@{
             Application( int argc, char** argv );
    virtual ~Application();
    //@}

    //! @name Main
    //@{
    int Execute(); 
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Application( const Application& );            //!< Copy constructor
    Application& operator=( const Application& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Config      config_;
    std::auto_ptr< dispatcher::Dispatcher > dispatcher_;
    //@}
};

#endif // __Application_h_
