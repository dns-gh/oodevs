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

class DispatcherFacade;

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
             Application( int argc, char** argv, int maxConnections );
    virtual ~Application();
    //@}

    //! @name Main
    //@{
    int Execute();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Application( const Application& );            //!< Copy constructor
    Application& operator=( const Application& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< DispatcherFacade > dispatcher_;
    //@}
};

#endif // __Application_h_
