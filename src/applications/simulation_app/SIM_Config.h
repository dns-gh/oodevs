// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SIM_Config_h_
#define __SIM_Config_h_

#include "dispatcher/Config.h"
#include "simulation_kernel/Tools/MIL_Config.h"

// =============================================================================
/** @class  SIM_Config
    @brief  SIM_Config
*/
// Created: NLD 2007-01-15
// =============================================================================
class SIM_Config : public MIL_Config
                 , public dispatcher::Config
{
public:
    //! @name Constructors/Destructor
    //@{
             SIM_Config( int argc, char** argv );
    virtual ~SIM_Config();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Parse( int argc, char** argv );
    //@}
};

#endif // __SIM_Config_h_
