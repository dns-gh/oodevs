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
#include <boost/noncopyable.hpp>

class DispatcherFacade;

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: NLD 2006-10-10
// =============================================================================
class Application : private boost::noncopyable
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
    //! @name Types
    //@{
    typedef void* ( *T_FacadeCreator )( int argc, char** argv, int maxConnections );
    typedef void ( *T_FacadeDestructor )( void* facade );
    typedef void ( *T_FacadeUpdator )( void* facade );
    //@}

private:
    //! @name Member data
    //@{
    void* dispatcher_;
    T_FacadeCreator facadeCreator_;
    T_FacadeDestructor facadeDestructor_;
    T_FacadeUpdator facadeUpdator_;
    //@}
};

#endif // __Application_h_
