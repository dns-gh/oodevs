// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LauncherFacade_h_
#define __LauncherFacade_h_

#include "DllConfig.h"
#include <memory>
#include <string>

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace launcher
{
    class Config;
    class Launcher;
}

class MT_FileLogger;

// =============================================================================
/** @class  LauncherFacade
    @brief  Launcher facade
*/
// Created: SBO 2010-11-03
// =============================================================================
class LAUNCHER_DLL_API LauncherFacade
{
public:
    //! @name Constructors/Destructor
    //@{
             LauncherFacade();
    explicit LauncherFacade( const std::string& path );
    virtual ~LauncherFacade();
    //@}

    //! @name Operations
    //@{
    void Initialize( int argc, char** argv );
    bool Update();
    void SetRootDir( const std::string& directory );
    //@}

    //! @name Accessors
    //@{
    bool IsInitialized() const;

    std::string GetLastError() const;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< launcher::Config > config_;
    std::auto_ptr< launcher::Launcher > launcher_;
    std::auto_ptr< MT_FileLogger > logger_;
    std::string lastError_;
    //@}
};

#pragma warning( pop )

#endif // __DispatcherFacade_h_
