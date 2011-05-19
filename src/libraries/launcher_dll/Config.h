// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Config_h_
#define __Config_h_

#include "tools/GeneralConfig.h"
#include <boost/noncopyable.hpp>

namespace launcher
{

// =============================================================================
/** @class  Config
    @brief  Config
*/
// Created: SBO 2010-11-03
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
    unsigned short GetLauncherPort() const;
    bool           GetTestMode() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned short launcherPort_;
    //@}
};

}

#endif // __Config_h_
