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
#include <memory>

namespace sword
{
    class ConnectionRequest;
    class ControlStartExercise;
    class ControlStopExercise;
    class ExercicesListRequest;
    class ProfilesListRequest;
}

namespace tools
{
    class Loader_ABC;
    class RealFileLoaderObserver_ABC;
}

namespace launcher
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
    explicit Launcher( const Config& config );
    virtual ~Launcher();
    //@}

    //! @name Operations
    //@{
    virtual void Update();
    virtual void HandleAdminToLauncher( const std::string& endpoint, const sword::AdminToLauncher& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Launcher( const Launcher& );            //!< Copy constructor
    Launcher& operator=( const Launcher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void HandleRequest( const std::string& endpoint, const sword::ConnectionRequest& message );
    void HandleRequest( const std::string& endpoint, const sword::ExercicesListRequest& message );
    void HandleRequest( const std::string& endpoint, const sword::ControlStartExercise& message );
    void HandleRequest( const std::string& endpoint, const sword::ControlStopExercise& message );
    void HandleRequest( const std::string& endpoint, const sword::ProfilesListRequest& message );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< tools::RealFileLoaderObserver_ABC > fileLoaderObserver_;
    std::auto_ptr< tools::Loader_ABC > fileLoader_;
    std::auto_ptr< LauncherService > server_;
    std::auto_ptr< ProcessService > processes_;
    //@}
};

}

#endif // __Launcher_h_
