// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SymbolHierarchy_ABC_h_
#define __SymbolHierarchy_ABC_h_

#include "Extension_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
// =============================================================================
/** @class  SymbolHierarchy_ABC
    @brief  Symbol hierarchy declaration
*/
// Created: LGY 2011-07-27
// =============================================================================
class SymbolHierarchy_ABC : public Extension_ABC
                          , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SymbolHierarchy_ABC() {}
    virtual ~SymbolHierarchy_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetValue() const = 0;
    virtual void SetValue( const std::string& value ) = 0;
    //@}
};
}

#endif // __SymbolHierarchy_ABC_h_
