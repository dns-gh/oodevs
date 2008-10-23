// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseService_h_
#define __ExerciseService_h_

#include "clients_kernel/ElementObserver_ABC.h"

namespace frontend
{
    class Process_ABC;
}

namespace kernel
{
    class Controllers;
}

namespace tools
{
    class GeneralConfig;
}

// =============================================================================
/** @class  ExerciseService
    @brief  ExerciseService
*/
// Created: LDC 2008-10-23
// =============================================================================
class ExerciseService : public kernel::Observer_ABC
                      , public kernel::ElementObserver_ABC< frontend::Process_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseService( kernel::Controllers& controllers, tools::GeneralConfig& config );
    virtual ~ExerciseService();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const frontend::Process_ABC& process );
    virtual void NotifyUpdated( const frontend::Process_ABC& process );
    virtual void NotifyDeleted( const frontend::Process_ABC& process );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExerciseService( const ExerciseService& );            //!< Copy constructor
    ExerciseService& operator=( const ExerciseService& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, unsigned int > T_ExercicePortList;
    //@}

private:
    //! @name Member data
    //@{
    T_ExercicePortList    exerciseList_;
    kernel::Controllers&  controllers_;
    tools::GeneralConfig& config_;
    //@}
};

#endif // __ExerciseService_h_
