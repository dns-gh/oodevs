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

namespace boost
{
    class thread;
}

namespace frontend
{
    class ProcessObserver_ABC;
    class SpawnCommand;
}

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
             CompositeProcessWrapper( frontend::ProcessObserver_ABC& observer, boost::shared_ptr< frontend::SpawnCommand > process1, boost::shared_ptr< frontend::SpawnCommand > process2 );
    virtual ~CompositeProcessWrapper();
    //@}

    //! @name Operations
    //@{
    virtual unsigned int GetPercentage() const;
    virtual QString      GetStatus() const;
    virtual std::string  GetStartedExercise() const;
    virtual std::string  GetExercise() const;
    virtual std::string  GetSession() const;
    void                 Start();
    void                 Stop();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CompositeProcessWrapper( const CompositeProcessWrapper& );            //!< Copy constructor
    CompositeProcessWrapper& operator=( const CompositeProcessWrapper& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Run();
    //@}

private:
    //! @name Member data
    //@{
    frontend::ProcessObserver_ABC& observer_;
    boost::shared_ptr< frontend::SpawnCommand > current_;
    boost::shared_ptr< frontend::SpawnCommand > first_;
    boost::shared_ptr< frontend::SpawnCommand > second_;
    std::auto_ptr< boost::thread > thread_;
    //@}
};

#endif // __CompositeProcessWrapper_h_
