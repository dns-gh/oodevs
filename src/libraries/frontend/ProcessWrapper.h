// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __frontend_ProcessWrapper_h_
#define __frontend_ProcessWrapper_h_

#include "Process_ABC.h"
#include <boost/noncopyable.hpp>

namespace boost
{
    class thread;
}

namespace frontend
{
    class ProcessObserver_ABC;
    class SpawnCommand;

// =============================================================================
/** @class  ProcessWrapper
    @brief  ProcessWrapper
*/
// Created: SBO 2008-10-14
// =============================================================================
class ProcessWrapper : public Process_ABC
                     , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ProcessWrapper( ProcessObserver_ABC& observer, boost::shared_ptr< SpawnCommand > process );
    virtual ~ProcessWrapper();
    //@}

    //! @name Operations
    //@{
    virtual unsigned int GetPercentage() const;
    virtual QString      GetStatus() const;
    virtual std::string  GetStartedExercise() const;
    virtual std::string  GetExercise() const;
    virtual std::string  GetSession() const;
    void                 StartAndBlockMainThread();
    void                 Start();
    void                 Attach();
    void                 Stop( bool forceProcessStop = true );
    const SpawnCommand*  GetCommand() const;
    void                 SetEndpoint( const std::string& endpoint );
    //@}

private:
    //! @name Helpers
    //@{
    void Run( bool start );
    void RunBlockingMainThread();
    //@}

private:
    //! @name Member data
    //@{
    ProcessObserver_ABC& observer_;
    boost::shared_ptr< SpawnCommand > process_;
    std::auto_ptr< boost::thread > thread_;
    //@}
};

}

#endif // __frontend_ProcessWrapper_h_
