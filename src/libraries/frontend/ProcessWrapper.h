// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ProcessWrapper_h_
#define __ProcessWrapper_h_

#include "frontend/Process_ABC.h"
#include <boost/shared_ptr.hpp>

namespace boost
{
    class mutex;
    class thread;
}

namespace frontend
{
    class ProcessObserver_ABC;
    class SpawnCommand;
}

namespace frontend
{
// =============================================================================
/** @class  ProcessWrapper
    @brief  ProcessWrapper
*/
// Created: BAX 2013-04-18
// =============================================================================
class ProcessWrapper : public Process_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ProcessWrapper( ProcessObserver_ABC& observer );
    virtual ~ProcessWrapper();
    //@}

    //! @name Helpers
    //@{
    typedef boost::shared_ptr< SpawnCommand > T_Spawn;
    typedef std::vector< T_Spawn > T_Spawns;
    //@}

    //! @name Operations
    //@{
    virtual void         Add( const T_Spawn& spawn );
    virtual unsigned int GetPercentage() const;
    virtual QString      GetStatus() const;
    virtual tools::Path  GetStartedExercise() const;
    virtual tools::Path  GetExercise() const;
    virtual tools::Path  GetSession() const;
    void                 Stop();
    bool                 IsRunning() const;
    T_Spawns             GetSpawns() const;
    void                 Join() const;
    //@}

    //! @name Static method
    //@{
    static void Start( const boost::shared_ptr< ProcessWrapper >& process );
    //@}

private:
    //! @name Helpers
    //@{
    void Run();
    void SetCurrent( const T_Spawn& spawn );

    void Apply( const std::function< void( SpawnCommand& ) >& operand );
    //@}

private:
    //! @name Member data
    //@{
    ProcessObserver_ABC& observer_;
    T_Spawns spawns_;
    T_Spawn current_;
    std::unique_ptr< boost::thread > thread_;
    std::unique_ptr< boost::mutex > mutex_;
    //@}
};
}

#endif // __ProcessWrapper_h_
