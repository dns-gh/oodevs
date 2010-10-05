// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ProcessService_h_
#define __ProcessService_h_

#include "tools/ElementObserver_ABC.h"
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_map.hpp>

namespace kernel
{
    class Controllers;
}

namespace MsgsLauncherToAdmin
{
    class MsgExercicesListResponse;
}

namespace frontend
{
    class Config;
    class Process_ABC;

// =============================================================================
/** @class  ProcessService
    @brief  ProcessService
*/
// Created: SBO 2010-09-30
// =============================================================================
class ProcessService : public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< boost::shared_ptr< Process_ABC > >
{

public:
    //! @name Constructors/Destructor
    //@{
             ProcessService( kernel::Controllers& controllers, const Config& config );
    virtual ~ProcessService();
    //@}

    //! @name Operations
    //@{
    void SendExerciseList( MsgsLauncherToAdmin::MsgExercicesListResponse& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProcessService( const ProcessService& );            //!< Copy constructor
    ProcessService& operator=( const ProcessService& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const boost::shared_ptr< Process_ABC >& process );
    virtual void NotifyDeleted( const boost::shared_ptr< Process_ABC >& process );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, unsigned int > T_ExercicePortList;
    typedef T_ExercicePortList::const_iterator    CIT_ExercicePortList;;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Config& config_;
    T_ExercicePortList exerciseList_;
    boost::ptr_map< std::string, boost::shared_ptr< Process_ABC > > runningProcesses_;
    //@}
};

}

#endif // __ProcessService_h_
