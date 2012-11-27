// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ModeController_h_
#define __ModeController_h_

#include "clients_kernel/ModeController.h"
#include "ENT/ENT_Tr.h"

// =============================================================================
/** @class  ModeController
    @brief  Mode controller
*/
// Created: LGY 2012-06-21
// =============================================================================
class ModeController : public kernel::ModeController< E_GamingMode >
{
public:
    //! @name Constructors/Destructor
    //@{
             ModeController() : kernel::ModeController< E_GamingMode >( eGamingMode_Default, "Gaming" ) {}
    virtual ~ModeController() {}
    //@}
};

#endif // __ModeController_h_
