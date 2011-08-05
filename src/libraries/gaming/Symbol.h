// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Symbol_h_
#define __Symbol_h_

#include "clients_kernel/SymbolHierarchy.h"

// =============================================================================
/** @class  Symbol
    @brief  Symbol
*/
// Created: JSR 2011-08-04
// =============================================================================
class Symbol : public kernel::SymbolHierarchy
{

public:
    //! @name Constructors/Destructor
    //@{
             Symbol( const std::string& symbol );
    virtual ~Symbol();
    //@}
};

#endif // __Symbol_h_
