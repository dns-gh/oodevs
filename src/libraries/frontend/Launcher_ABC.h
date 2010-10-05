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

namespace MsgsAdminToLauncher
{
    class MsgAdminToLauncher;
}

namespace frontend
{

// =============================================================================
/** @class  Launcher_ABC
    @brief  Launcher_ABC
*/
// Created: SBO 2010-09-29
// =============================================================================
class Launcher_ABC
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
    virtual void HandleAdminToLauncher( const std::string& endpoint, const MsgsAdminToLauncher::MsgAdminToLauncher& message ) = 0;
    //@}
};

}

#endif // __Launcher_ABC_h_
