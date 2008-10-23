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

namespace boost
{
    class thread;
}

namespace kernel
{
    class Controller;
}

namespace frontend
{
    class SpawnCommand;
}

// =============================================================================
/** @class  ProcessWrapper
    @brief  ProcessWrapper
*/
// Created: SBO 2008-10-14
// =============================================================================
class ProcessWrapper : public frontend::Process_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ProcessWrapper( kernel::Controller& controller, boost::shared_ptr< frontend::SpawnCommand > process );
    virtual ~ProcessWrapper();
    //@}

    //! @name Operations
    //@{
    virtual unsigned int GetPercentage() const;
    virtual QString GetStatus() const;
    virtual std::string GetStartedExercise() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProcessWrapper( const ProcessWrapper& );            //!< Copy constructor
    ProcessWrapper& operator=( const ProcessWrapper& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ThreadStart();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    boost::shared_ptr< frontend::SpawnCommand > process_;
    std::auto_ptr< boost::thread > thread_;
    //@}
};

#endif // __ProcessWrapper_h_
