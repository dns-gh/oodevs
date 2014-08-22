// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Dotations_ABC_h_
#define __Dotations_ABC_h_

#include "Extension_ABC.h"

namespace kernel {

class DotationType;

// =============================================================================
/** @class  Dotations_ABC
    @brief  Dotations_ABC
*/
// Created: LDC 2010-06-07
// =============================================================================
class Dotations_ABC : public kernel::Extension_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Dotations_ABC() {}
    virtual ~Dotations_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Accept( const kernel::DotationType& type ) const = 0;
    virtual bool IsEmptyGasTank() const = 0;
    //@}
};

} // end namespace

#endif // __Dotations_ABC_h_
