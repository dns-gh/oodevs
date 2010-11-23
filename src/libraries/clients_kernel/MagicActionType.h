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
    explicit MagicActionType();
    explicit MagicActionType( const std::string& name );
    explicit MagicActionType( const std::string& name, unsigned long id );
    virtual ~MagicActionType();
    //@}

private:
    //! @name Operations
    //@{
    void Clean();
    OrderParameter* CreateOrderParameter( const std::string& name, const std::string& type );
    virtual void Initialize();
    //@}
};

}

#endif // __MagicActionType_h_
