// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MagicActionType_h_
#define __MagicActionType_h_

#include "OrderType.h"

namespace kernel
{
    class OrderContext;

// =============================================================================
/** @class  MagicActionType
    @brief  MagicActionType
*/
// Created: JSR 2010-04-02
// =============================================================================
class MagicActionType : public OrderType
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MagicActionType( );
    virtual ~MagicActionType();
    //@}

public:
    //! @name Operations
    //@{
    void Initialize();
    void Clean();
    //@}
};

}

#endif // __MagicActionType_h_
