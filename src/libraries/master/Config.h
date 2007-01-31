// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Config_h_
#define __Config_h_

#include "tools/GeneralConfig.h"

namespace master
{

// =============================================================================
/** @class  Config
    @brief  Config
*/
// Created: NLD 2007-01-10
// =============================================================================
class Config : public virtual tools::GeneralConfig
{
public:
    //! @name Constructors/Destructor
    //@{
             Config();
    virtual ~Config();
    //@}

    //! @name Accessors
    //@{
    std::string GetMasterFile() const;
    //@}
};

}

#endif // __Config_h_
