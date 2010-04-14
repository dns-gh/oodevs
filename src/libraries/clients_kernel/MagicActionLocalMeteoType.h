// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MagicActionLocalMeteoType_h_
#define __MagicActionLocalMeteoType_h_

#include "MagicActionMeteoType.h"

namespace kernel
{

// =============================================================================
/** @class  MagicActionLocalMeteoType
    @brief  MagicActionLocalMeteoType
*/
// Created: JSR 2010-04-09
// =============================================================================
class MagicActionLocalMeteoType : public MagicActionMeteoType
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MagicActionLocalMeteoType();
    virtual ~MagicActionLocalMeteoType();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Initialize();
    //@}
};

}

#endif // __MagicActionLocalMeteoType_h_
