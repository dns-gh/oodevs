// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __frontend_Config_h_
#define __frontend_Config_h_

#include "tools/GeneralConfig.h"

namespace frontend
{
// =============================================================================
/** @class  Config
    @brief  Config
*/
// Created: SBO 2008-03-14
// =============================================================================
class Config : public tools::GeneralConfig
{
public:
    //! @name Constructors/Destructor
    //@{
             Config();
    virtual ~Config();
    //@}

    //! @name Operations
    //@{
    const tools::Path& GetPackageFile() const;
    unsigned short GetLauncherPort() const;
    bool IsTestMode() const;
    //@}

private:
    //! @name Member data
    //@{
    tools::Path package_;
    unsigned short launcherPort_;
    //@}
};
}

#endif // __frontend_Config_h_
