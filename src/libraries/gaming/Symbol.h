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

#include "clients_kernel/SymbolHierarchy_ABC.h"

// =============================================================================
/** @class  Symbol
    @brief  Symbol
*/
// Created: LGY 2011-07-28
// =============================================================================
class Symbol : public kernel::SymbolHierarchy_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Symbol();
    virtual ~Symbol();
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetValue() const;
    virtual void SetValue( const std::string& value );
    //@}

private:
    //! @name Member data
    //@{
    std::string symbol_;
    //@}
};

#endif // __Symbol_h_
