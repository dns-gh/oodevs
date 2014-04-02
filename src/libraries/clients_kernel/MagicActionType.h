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

typedef std::string (*T_Stringifier)( int );
T_Stringifier GetEnumStringifier( const std::string& name );

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
             MagicActionType();
             MagicActionType( const std::string& name, unsigned long id );
    explicit MagicActionType( const std::string& name );
    virtual ~MagicActionType();
    //@}

private:
    //! @name Operations
    //@{
    void Initialize();
    //@}
};

}

#endif // __MagicActionType_h_
