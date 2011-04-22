// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MainFactory_ABC_h_
#define __MainFactory_ABC_h_

#include <memory>
#include <boost/noncopyable.hpp>

class SwordMessageHandler_ABC;

namespace mission_tester
{
   class Client;
   class Facade;
   class SchedulerFactory;
   class Timeout;

// =============================================================================
/** @class  MainFactory_ABC
    @brief  Main factory definition
*/
// Created: HBD 2011-04-20
// =============================================================================
class MainFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MainFactory_ABC() {}
    virtual ~MainFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< Client > CreateClient( SwordMessageHandler_ABC& handler ) const = 0;
    virtual std::auto_ptr< SchedulerFactory > CreateSchedulerFactory() const = 0;
    virtual void ConfigureLogging( Facade& facade ) const = 0;
    virtual std::auto_ptr< Timeout > CreateTimeout() const = 0;
    //@}
};
}

#endif // __MainFactory_ABC_h_
