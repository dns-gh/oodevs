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
#include "clients_kernel/Serializable_ABC.h"

namespace xml
{
    class xistream;
    class xostream;
}

// =============================================================================
/** @class  Symbol
    @brief  Symbol
*/
// Created: LGY 2011-07-28
// =============================================================================
class Symbol : public kernel::SymbolHierarchy
             , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Symbol();
             Symbol( xml::xistream& xis );
    virtual ~Symbol();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}
};

#endif // __Symbol_h_
