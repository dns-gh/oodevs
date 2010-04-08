// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MagicActionMoveToType_h_
#define __MagicActionMoveToType_h_

#include "MagicActionType.h"

namespace kernel
{

// =============================================================================
/** @class  MagicActionMoveToType
    @brief  MagicActionMoveToType
*/
// Created: JSR 2010-04-08
// =============================================================================
class MagicActionMoveToType : public MagicActionType
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MagicActionMoveToType();
    virtual ~MagicActionMoveToType();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Initialize();
    //@}
};

}

#endif // __MagicActionMoveToType_h_
