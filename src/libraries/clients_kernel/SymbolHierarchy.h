// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SymbolHierarchy_h_
#define __SymbolHierarchy_h_

#include "SymbolHierarchy_ABC.h"

namespace kernel
{
// =============================================================================
/** @class  SymbolHierarchy
    @brief  SymbolHierarchy
*/
// Created: LGY 2011-07-28
// =============================================================================
class SymbolHierarchy : public SymbolHierarchy_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SymbolHierarchy();
    virtual ~SymbolHierarchy();
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetValue() const;
    virtual void SetValue( const std::string& value );
    //@}

protected:
    //! @name Member data
    //@{
    std::string symbol_;
    //@}
};

}

#endif // __SymbolHierarchy_h_
