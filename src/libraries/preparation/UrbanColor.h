// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UrbanColor_h_
#define __UrbanColor_h_

#include "clients_kernel/UrbanColor_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  UrbanColor
    @brief  UrbanColor
*/
// Created: LGY 2011-04-19
// =============================================================================
class UrbanColor : public kernel::UrbanColor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanColor( xml::xistream& xis );
    virtual ~UrbanColor();
    //@}
};

#endif // __UrbanColor_h_
