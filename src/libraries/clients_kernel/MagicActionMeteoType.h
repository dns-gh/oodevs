// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MagicActionMeteoType_h_
#define __MagicActionMeteoType_h_

#include "MagicActionType.h"

namespace kernel
{

// =============================================================================
/** @class  MagicActionMeteoType
    @brief  MagicActionMeteoType
*/
// Created: JSR 2010-04-09
// =============================================================================
class MagicActionMeteoType : public MagicActionType
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MagicActionMeteoType();
    virtual ~MagicActionMeteoType();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Initialize();
    //@}
};

}

#endif // __MagicActionMeteoType_h_
