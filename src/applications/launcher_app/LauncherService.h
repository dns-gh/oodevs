// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LauncherService_h_
#define __LauncherService_h_

#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <windows.h>
#include <string>

class LauncherFacade;

// =============================================================================
/** @class  LauncherService
    @brief  Launcher service
*/
// Created: JSR 2011-05-10
// =============================================================================
class LauncherService : private boost::noncopyable
{
public:
    //! @name Instance
    //@{
    static void Initialize( const boost::filesystem::path& path );
    static void Terminate();
    static LauncherService& Instance();
    //@}

    //! @name Operations
    //@{
    void Install() const;
    void RunService();
    void KillService();
    void UnInstall();
    void ExecuteSubProcess();
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    explicit LauncherService( const boost::filesystem::path& path );
    virtual ~LauncherService();
    //@}

    //! @name Services
    //@{
    static void WINAPI ServiceMain( DWORD dwArgc, LPTSTR *lpszArgv );
    static void WINAPI ServiceHandler( DWORD fdwControl );
    //@}

private:
    //! @name Member data
    //@{
    static LauncherService* pInstance_;
    std::auto_ptr< LauncherFacade > pFacade_;
    SERVICE_STATUS_HANDLE hServiceStatusHandle_;
    SERVICE_STATUS ServiceStatus_;
    const boost::filesystem::path& path_;
    const std::string name_;
    bool isRunning_;
    bool isPaused_;
    //@}
};

#endif // __LauncherService_h_
