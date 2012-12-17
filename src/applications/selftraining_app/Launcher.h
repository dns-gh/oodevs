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

#pragma warning( push, 0 )
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#pragma warning( pop )

class LauncherFacade;

namespace tools
{
    class WaitEvent;
}

// =============================================================================
/** @class  Launcher
    @brief  Launcher
*/
// Created: SBO 2010-11-03
// =============================================================================
class Launcher : private boost::enable_shared_from_this< Launcher >
{

public:
    //! @name Constructors/Destructor
    //@{
             Launcher( int argc, char** argv );
    virtual ~Launcher();
    //@}

    //! @name Operations
    //@{
    bool IsInitialized() const;
    std::string GetLastError() const;
    void SetRootDir( const std::string& directory );
    //@}

private:
    //! @name Operations
    //@{
    void Run();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< LauncherFacade > launcher_;
    std::auto_ptr< tools::WaitEvent > quit_;
    std::auto_ptr< boost::thread > thread_;
    //@}
};

#endif // __Launcher_h_
