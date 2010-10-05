// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Launcher_h_
#define __Launcher_h_

#include "Launcher_ABC.h"

namespace MsgsAdminToLauncher
{
    class MsgConnectionRequest;
    class MsgExercicesListRequest;
}

namespace kernel
{
    class Controllers;
}

namespace frontend
{
    class Config;
    class LauncherService;
    class ProcessService;

// =============================================================================
/** @class  Launcher
    @brief  Launcher
*/
// Created: SBO 2010-09-29
// =============================================================================
class Launcher : public Launcher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Launcher( kernel::Controllers& controllers, const frontend::Config& config );
    virtual ~Launcher();
    //@}

    //! @name Operations
    //@{
    virtual void Update();
    virtual void HandleAdminToLauncher( const std::string& endpoint, const MsgsAdminToLauncher::MsgAdminToLauncher& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Launcher( const Launcher& );            //!< Copy constructor
    Launcher& operator=( const Launcher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void HandleConnectionRequest( const std::string& endpoint, const MsgsAdminToLauncher::MsgConnectionRequest& message );
    void HandleExerciseListRequest( const std::string& endpoint, const MsgsAdminToLauncher::MsgExercicesListRequest& message );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< LauncherService > server_;
    std::auto_ptr< ProcessService > processes_;
    //@}
};

}

#endif // __Launcher_h_
