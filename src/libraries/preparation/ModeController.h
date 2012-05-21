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
    @brief  ModeController
*/
// Created: ABR 2012-05-10
// =============================================================================
class ModeController : public kernel::ModeController< E_PreparationMode >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ModeController( QMainWindow* parent )
        : kernel::ModeController< E_PreparationMode >( parent, ePreparationMode_Exercise, "Preparation" ) {}
    virtual ~ModeController() {}
    //@}
};

#endif // __ModeController_h_
