// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __CompositeProcessWrapper_h_
#define __CompositeProcessWrapper_h_

#include "frontend/Process_ABC.h"
#include <boost/function.hpp>

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

typedef boost::shared_ptr< frontend::SpawnCommand > T_Spawn;
typedef std::vector< T_Spawn > T_Spawns;

// =============================================================================
/** @class  CompositeProcessWrapper
    @brief  CompositeProcessWrapper
*/
// Created: SBO 2008-10-15
// =============================================================================
class CompositeProcessWrapper : public frontend::Process_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CompositeProcessWrapper( frontend::ProcessObserver_ABC& observer );
    virtual ~CompositeProcessWrapper();
    //@}

    //! @name Operations
    //@{
    virtual void         Add( const T_Spawn& spawn );
    virtual unsigned int GetPercentage() const;
    virtual QString      GetStatus() const;
    virtual tools::Path  GetStartedExercise() const;
    virtual tools::Path  GetExercise() const;
    virtual tools::Path  GetSession() const;
    void                 Start();
    void                 Stop();
    //@}

private:
    //! @name Helpers
    //@{
    void Run();
    void SetCurrent( const T_Spawn& spawn );
    T_Spawns GetSpawns() const;
    void Apply( const boost::function< void( frontend::SpawnCommand& ) >& operand );
    //@}

private:
    //! @name Member data
    //@{
    frontend::ProcessObserver_ABC& observer_;
    T_Spawns spawns_;
    T_Spawn current_;
    std::auto_ptr< boost::thread > thread_;
    std::auto_ptr< boost::mutex > mutex_;
    //@}
};

#endif // __CompositeProcessWrapper_h_
