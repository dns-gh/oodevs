// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Launcher_ABC_h_
#define __Launcher_ABC_h_

#include <boost/noncopyable.hpp>
#include <string>

namespace sword
{
    class AdminToLauncher;
}

namespace launcher
{
// =============================================================================
/** @class  Launcher_ABC
    @brief  Launcher_ABC
*/
// Created: SBO 2010-09-29
// =============================================================================
class Launcher_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Launcher_ABC() {}
    virtual ~Launcher_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Update() = 0;
    virtual void HandleConnectionToAdmin( const std::string& endpoint ) = 0;
    virtual void HandleAdminToLauncher( const std::string& endpoint, const sword::AdminToLauncher& message ) = 0;
    //@}
};

}

#endif // __Launcher_ABC_h_
